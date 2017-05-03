#include <iostream>
#include <string>
#include <pthread.h>
#include <queue>

// Windows Specific Code //
#ifdef _WIN32
#include <windows.h>

void sleep(int x)
{
	Sleep(x*1000);
}
#endif
// End Windows Code //


using namespace std;

// Constants //
const int ASSEMBLY_TIME = 15;
const int LAUNCH_TIME = 10;
const int FLIGHT_TIME = 5;
const int DISASSEMBLY_TIME = 5;
// End Constants //


typedef unsigned char byte;

// This was added for fun. It's a simple shuffle cipher which will make the 
// components more randomized.
// Compiled with -D RANDOMIZE if you want it to be a part of the code.
#ifdef RANDOMIZE
#include <time.h>
#include <stdlib.h>
class ShuffleCipher
{
	private:
	int size;
	unsigned short *c, *c2;
	ShuffleCipher();
	public:
	~ShuffleCipher()
	{
		delete[] c;
		delete[] c2;
	}

	ShuffleCipher(int size) : size(size)
	{
		c = new unsigned short[size];
		c2 = new unsigned short[size];

		for(int i = 0; i < size; i++)
		{
			c[i] = (unsigned short)i;
		}

		unsigned short temp;

		// Shuffle the positions.
		for(int x = 0; x < 10; x++)
		for(int i = 0; i < size; i++)
		{
			int r = rand() % size;
			temp = c[i];
			c[i] = c[r];
			c[r] = temp;
		}

		for(int i = 0; i < size; i++)
		{
			c2[c[i]] = i;
		}
	}

	void Right(int &i)
	{
		i = c[i];
	}

	void Left(int &i)
	{
		i = c2[i];
	}
};
#endif

// This tracking structure is used for keeping track of which specific items are available and which aren't.
// Individual items, rather than clusters. Each "item" is represented by a single bit.
// This has been made obsolete by the queue I implemented later on, but I don't want to bother refractoring it out.
// It also has "cipher" capabilities, which anonymizes some of the internal positions.
// That isn't to claim it's secure or anything (not that it needs to be), the cipher stuff is just for output purposes..
class TrackingStructure
{
	private:
	byte* data;
	int size;
	int _available;
	int arraySize; 

	#ifdef RANDOMIZE
	ShuffleCipher *ciph;
	#endif

	void mutateRight(int& x)
	{
		#ifdef RANDOMIZE
		ciph->Right(x);
		#endif
	}

	void mutateLeft(int& x)
	{
		#ifdef RANDOMIZE
		ciph->Left(x);
		#endif
	}

	public:
	TrackingStructure() {}
	TrackingStructure(int size) : size(size), _available(size)
	{
		#ifdef RANDOMIZE
		ciph = new ShuffleCipher(size);
		#endif

		arraySize = size / 8;
		int offset = size % 8;
		byte offsetByte = 0;
		if(offset)
		{
			arraySize++;	
			for(int i = 0; i < offset; i++)
				offsetByte |= (1 << i);
		}

		data = new byte[arraySize];
		for(int i = 0; i < arraySize; i++)
		{
			data[i] = (byte)255;
		}
		
		if(offset)
			data[arraySize - 1] = offsetByte;
	}

	~TrackingStructure()
	{
		#ifdef RANDOMIZE
		delete ciph;
		#endif
		delete[] data;
	}

	int available()
	{
		return _available;
	}


	void release(int pos)
	{
		mutateLeft(pos);
		int p = pos / 8;
		pos %= 8;

		data[p] |= (1 << pos); 
		_available++;
	}

	int getItem()
	{		
		if(available()) 
		for(int i = 0; i < arraySize; i++)
		{
			// Could theoretically be an optimization for larger arrays by quickly checking 
			// if it has any available in that byte.
			// I can expand this to use integers later on.
			if(data[i])
			for(int j = 0; j < 8; j++)
			{
				if(data[i] & (1 << j))
				{
					data[i] ^= (1 << j);
					_available--;
					int x = i*8 + j;
					mutateRight(x);
					return x;
				}
			}
		}

		return -1;
	}

};


// This is our Resource Broker. 
// It manages pretty much everything that happens. 
// It has a few nice features, like being able to queue up resource requests 
// and immediately give the resources to the requesting thread. 

// I would've done some asynchronous programming, but meh :P.
// An async programming lib for C++ would be an interesting project though...
class Resources
{
	private:
	pthread_mutex_t fuelTankMutex, fuselageMutex, engineMutex; 
	pthread_mutex_t launchPad, queueLock;
	TrackingStructure engines, fuselages, fuelTanks;

	// This code is used to return the returned engine to the next person in the queue,
	// it makes the tracking structure obsolete I guess,
	// but oh well :p. 
	bool queueCheck(queue<int*> &x, int v)
	{
		bool skip = false;

		lockOrder();
		if(x.size() > 0)
		{
			int* res = x.front();
			x.pop();
			*res = v; 
			skip = true;
		}
		unlockOrder();
		return skip;
	}

	queue<int*> enginesQueue, fuselagesQueue, fuelTanksQueue;
	Resources() {} 
	public: 
	Resources(int fuelTanks, int fuselages, int engines) : 	fuelTanks(fuelTanks), fuselages(fuselages), engines(engines)  
	{
		pthread_mutex_init(&fuelTankMutex, NULL);
		pthread_mutex_init(&fuselageMutex, NULL);
		pthread_mutex_init(&engineMutex, NULL);
		pthread_mutex_init(&launchPad, NULL);
		pthread_mutex_init(&queueLock, NULL);
	}

	~Resources()
	{
		pthread_mutex_destroy(&fuelTankMutex);
		pthread_mutex_destroy(&fuselageMutex);
		pthread_mutex_destroy(&engineMutex);
		pthread_mutex_destroy(&launchPad);
		pthread_mutex_destroy(&queueLock);
	}

	int getLaunch()
	{
		return pthread_mutex_trylock(&launchPad); 
	}

	void lockOrder()
	{
		pthread_mutex_lock(&queueLock);
	}

	void unlockOrder()
	{
		pthread_mutex_unlock(&queueLock);
	}

	void releaseLaunch()
	{
		pthread_mutex_unlock(&launchPad); 
	}

	void requestEngine(int *result)
	{
		// Tells the simulation that an engine isn't available, and queues.
		if (!engines.available())
		{
			*result = 0;
			enginesQueue.push(result);
			return;
		}

		// This isn't necessary for the request call because another mutex is locked around it.
		// That's why the flags cull it out unless you remove the "line-blocking" feature.
		#ifdef Unsafe 
		pthread_mutex_lock(&engineMutex);
		#endif
		{
			*result = engines.getItem() + 1;
		}
		#ifdef Unsafe 
		pthread_mutex_unlock(&engineMutex);
		#endif
	}

	void requestFuelTank(int *result)
	{
		// Tells the simulation that a fuel tank isn't available, and queues.
		if (!fuelTanks.available())
		{
			*result = 0;
			fuelTanksQueue.push(result);
			return;
		}

		// This isn't necessary for the request call because another mutex is locked around it.
		#ifdef Unsafe 
		pthread_mutex_lock(&fuelTankMutex);
		#endif
		{
			*result = fuelTanks.getItem() + 1;
		}
		#ifdef Unsafe 
		pthread_mutex_unlock(&fuelTankMutex);
		#endif
	}

	void requestFuselage(int *result)
	{
		// Tells the simulation that a fuselage isn't available, and queues.
		if (!fuselages.available())
		{
			fuselagesQueue.push(result);
			*result = 0;
			return;
		}

		// This isn't necessary for the request call because another mutex is locked around it.
		#ifdef Unsafe 
		pthread_mutex_lock(&fuselageMutex);
		#endif
		{
			*result = fuselages.getItem() + 1;
		}
		#ifdef Unsafe 
		pthread_mutex_unlock(&fuselageMutex);
		#endif
	}

	void releaseFuelTank(int v)
	{
		if(queueCheck(fuelTanksQueue, v)) return;

		pthread_mutex_lock(&fuelTankMutex);
		{
			fuelTanks.release(v-1);
		}
		pthread_mutex_unlock(&fuelTankMutex);
	}

	void releaseFuselage(int v)
	{	
		if(queueCheck(fuselagesQueue, v)) return;

		pthread_mutex_lock(&fuselageMutex);
		{
			fuselages.release(v-1);
		}
		pthread_mutex_unlock(&fuselageMutex);
	}


	void releaseEngine(int v)
	{	
		if(queueCheck(enginesQueue, v)) return;
	
		pthread_mutex_lock(&engineMutex);
		{
			engines.release(v-1);
		}
		pthread_mutex_unlock(&engineMutex);
	}
	
};

// Globals // 
Resources *r;
pthread_mutex_t outputMutex;
// End Globals //


// I hate going with a C Style approach the output, but it's better to just embrace it temporarily.//
// Output Functions //
void olock()
{
	pthread_mutex_lock(&outputMutex);
}

void ofree()
{
	pthread_mutex_unlock(&outputMutex);
}

inline void startMessage(int id)
{
	cout << "Kerbal " << id << ": "; 
}

// Counts down the launch.
void countDown(int id, int val)
{
	for(int i = 0; i < val; i++)
	{
		olock();
		startMessage(id);
		cout << "Launch Countdown " << (val-i) << " ..." << endl;
		ofree();
		sleep(1);
	} 

	olock();
	startMessage(id);
	cout << "Liftoff!" << endl; 
	ofree();
}
// End Output Functions //


void *kerbalLife(void *threadID)
{	
	long id = (long)threadID;

	#ifndef OneTime
	while(true) 
	#endif
	{
		int engine, fuselage, fuelTank;

	

		// Using the same mutex for all the queues will keep things in sync, and not allow Kerbal to get past another one. 
		// Since the kerbals all follow the same order of requesting parts, they can't end up with a deadlock.
		// The locking at this position is fine because the request operations are relatively cheap.

		// However, this will prevent the order of the parts being handed out from being "randomized". 
		#ifndef Unsafe
		r->lockOrder();
		#endif
		{
			r->requestEngine(&engine);
			r->requestFuselage(&fuselage);
			r->requestFuelTank(&fuelTank);
		}
		#ifndef Unsafe
		r->unlockOrder();
		#endif

		// If the parts haven't been acquired, wait for them.
		if (!engine || !fuselage || !fuelTank)
		{
			olock();
			startMessage(id);
			cout << "Waiting for the assembly facility." << endl;
			ofree();
			do
			{
				sleep(1);
			} while (!engine || !fuselage || !fuelTank);
		}

		
		// Enters the facility.
		olock();
		{
			startMessage(id);
			cout << "Enters into the assembly facility." << endl; 
			ofree();
		}
		
		// Allows the kerbal time to assemble the engine.
		sleep(ASSEMBLY_TIME);
		olock(); 
		{
			startMessage(id);
			cout << "Assembles Engine " << engine << ", Fuselage " << fuselage << ", Fuel Tank " << fuelTank << "." << endl;
		} 
		ofree();

		// Tries to acquire the Launch Pad, but if it can't, it waits for it.
		if(r->getLaunch())
		{
			olock();
			{
				startMessage(id);
				cout << "Waiting for launch pad." << endl;
			}
			ofree();

			while(r->getLaunch()) { sleep(1); }
		}

		// Notifies that the Kerbal arrived at the Launch pad
		olock();
		{
			startMessage(id);
			cout << "Arriving at Launch Pad." << endl; 
		}
		ofree();
		
		// Counts down
		countDown(id, LAUNCH_TIME);
		
		// If this flag isn't set, the launch pad is made free after launch.
		#ifndef releaseAtLanding
		r->releaseLaunch(); 
		#endif


		// Flies for a bit, then lands.
		sleep(FLIGHT_TIME);
		olock();
		{
			//Land
			startMessage(id);
			cout << "Landed." << endl;
			
			// Enter Facility (for disassembling)
			startMessage(id);
			cout << "Enters into the assembly facility." << endl;
		}
		ofree();

		// Releases after it's landed if the compile flag was set.
		#ifdef releaseAtLanding
		r->releaseLaunch(); 
		#endif


		// Allows the kerbal to disassemble the ship.
		sleep(DISASSEMBLY_TIME);
		olock();
		{
			startMessage(id);
			cout << "Disassembles Engine " << engine << ", Fuselage " << fuselage << ", Fuel Tank " << fuelTank << "." << endl;
		}
		ofree();

		// Releases all the parts.
		r->releaseEngine(engine);
		r->releaseFuselage(fuselage);
		r->releaseFuelTank(fuelTank);
	}

	pthread_exit(NULL);
}


int main(int argc, char **cargs)
{
	if(argc < 5) 
	{
		cout << "Not enough arguments." << endl; 
		return -1; 
	}

	#ifdef RANDOMIZE
	srand(time(NULL));
	#endif

	pthread_mutex_init(&outputMutex, NULL);
	
	// 1, 2, 3, 4
	// Engines, Fuselages, Fuel Tanks, Kerbals
	
	int kerbals = atoi(cargs[4]); 
	r = new Resources(atoi(cargs[1]), atoi(cargs[2]), atoi(cargs[3])); 

	pthread_t *threads = new pthread_t[kerbals]; 
	// Create The Threads and Launch The Kerbals.
	for(long i = 0; i < kerbals; i++)
	{
		long j = i+1;
		int rc = pthread_create(&threads[i], NULL, kerbalLife, (void*)j); 

		
		if(rc)
		{
			#ifdef OneTime
			i--; 
			sleep(15); 
			#else
			olock();
			{
				cout << "Error: " << "Can't acquire enough threads." << endl;
				cout << "You could attempt to correct this situation by compiling with -D OneTime." << endl;
				cout << "That option only allows a Kerbal to launch once." << endl;
				exit(0);
			}
			ofree(); // This will never be executed, I know.
			#endif
		}
	}


	for(int i = 0; i < kerbals; i++)
	{
		pthread_join(threads[i], NULL); 
	}

	pthread_mutex_destroy(&outputMutex);
	delete[] threads;
	delete r;
	return 0; 
}

