/*
 * Name: Jesse Daniel Mitchell
 * Id #: 1001449163
 * Programming Assignment 1 Part 2
 * Description: This is the proc assignment. This reads data from /proc/ to give
 * information about the system. 
 * This code needs to be compiled with the -lpthread flag.  
 */

#include <iostream>
#include <string>
#include <fstream>

#include <pthread.h>
#include <unistd.h>

using namespace std;

// Let's use a few globals for simplicity here, since the 
// scope of the program is extremely small.
long long gSleepValue, gReadValue, gSectors, gContexts, gCreations, gOriginalCreationCount,
          gOriginalContextCount, gCount;
double gUserMode, gSystemMode, gIdle, gMemoryFree, gMemoryTotal;
string gMemoryType;
pthread_mutex_t gMutex = PTHREAD_MUTEX_INITIALIZER;

void *printData(void* a);


/*
 * Function : getWord
 * Parameter(s): 
 * str - The string to be parsed through.
 * num - The word (from 0) that you wish to have extracted.
 * returns : a string that is an extracted substring.
 * Description: Extracts the "num"th word in the string.
 */
string getWord(string str, int num)
{
    int count(-1), off(-1), off2(-1);

    // Parses through the spaces.
    while (count < num)
    {
        off = off2 + 1; 
        off2 = str.find(" ", off);
        count++;
    }

    // Checks if there is another space following.
    if(off2 == -1)
    {
        // If there isn't, check if there's a space to begin with
        if(off == -1)
        {
            // This means it went out of bounds.
            return string("");
        }
        // Returns the substring to the end of the string.
        return str.substr(off);
    }

    // Returns the targeted word.
    return str.substr(off, off2-off);
}


/*
 * Function : parseInfo
 * Parameter(s): 
 * str - The string to be parsed through.
 * returns : a string that is an extracted substring.
 * Description: Extracts information after the colon. This is useful for some of the "info" files.
 */
string parseInfo(string str)
{
    str = str.substr(str.find(":") + 1);
    while(str[0] == ' ' || str[0] == '\t')
    {
        str = str.substr(1);
    }
    return str;
}

int main(int argc, char **args)
{
    // Checks the number of arguments to see what mode to run in.
    if(argc == 1)
    {
        // This is the default mode. 

        // Processor Type Section //
        ifstream file("/proc/cpuinfo");
        string w;

        // Scans for the model name line. 
        // This is used to extract the processor name.
        while(!file.eof())
        {
            getline(file, w);

            if(w.find("model name") == 0)
            {
                cout << "Processor Type: " << parseInfo(w) << endl;
                break;
            }
        }
        
        file.close();
        // End Processor Type Section //

        // Linux Kernel Version Section //
        file.open("/proc/version");

        // Grabs the third "word" from this file. 
        // This will be the kernel version.
        getline(file, w);
        cout << "Linux Kernel Version: " << getWord(w, 2) << endl;

        file.close();
        // End Linux Kernel Version Section //
        
        // Total Memory Section //
        file.open("/proc/meminfo");

        // Scans for the "MemTotal" Line. 
        while(!file.eof())
        {
            getline(file, w);

            if(w.find("MemTotal") == 0)
            {
                cout << "Total Memory: " << parseInfo(w) << endl;
                break;
            }
        }

        file.close();
        // End Total Memory Section //
        
        // Uptime Section //
        file.open("/proc/uptime");

        getline(file, w);
        cout << "Uptime: " << getWord(w, 0) << " seconds." << endl;

        file.close();
        // End Uptime Section //
    } else if (argc == 3)
    {
        // The "dynamic" version of the program.

        // Reads the read rate and the output rate.
        gReadValue = atoi(args[1]);
        gSleepValue = atoi(args[2]);

        // Creates the thread for output.
        pthread_t thread;
        int errorCode;

        if((errorCode = pthread_create(&thread, NULL, &printData, NULL)))
        {
            cout << "Thread Creation Failed: " << errorCode << endl;
        }
        else
        {
            // creates a string for parsing lines and a file variable 
            string w; 
            ifstream file;
            // loops infinitely
            while(true) 
            {

                //Locks access to the global variables, for reading data.
                pthread_mutex_lock(&gMutex);
                {
                    // Increment the counter for number of reads.
                    gCount++;


                    // This section is for cpu time stats,
                    // context switches,
                    // and process creations.
                    file.open("/proc/stat");
                    
                    int flag = 0;
                    while(!file.eof())
                    {
                        // This line ensures that all the requested data is
                        // before it attempts to terminate early. 
                        if(flag == 7) break;

                        getline(file, w);

                        // Gets cpu stats
                        if(w.find("cpu ") == 0)
                        {
                            flag = flag ^ 1;

                            // extracts the various cpu times
                            int userMode    = atoi(getWord(w, 2).c_str()),
                                userModeLow = atoi(getWord(w, 3).c_str()),
                                systemMode  = atoi(getWord(w, 4).c_str()),
                                idle        = atoi(getWord(w, 5).c_str());
                           
                            // Combines user mode and user mode low priority
                            gUserMode += userMode + userModeLow;

                            gSystemMode += systemMode;
                            gIdle += idle; 
                        }
                        else
                        // Gets context switch stats
                        if(w.find("ctxt") == 0)
                        {
                            flag = flag ^ 2;
                            
                             //If this variable isn't set, set it.
                            if(!gOriginalContextCount) 
                            {
                                gOriginalContextCount = atoi(getWord(w, 1).c_str());
                            }
                            else
                            {
                                // Start counting up the number of context switches each second.
                                gContexts += (atoi(getWord(w, 1).c_str()) - gOriginalContextCount);
                            }

                        }
                        else
                        // Gets process stats
                        if(w.find("processes") == 0)
                        {
                            flag = flag ^ 4;
                            
                            //If this variable isn't set, set it.
                            if(!gOriginalCreationCount) 
                            {
                                gOriginalCreationCount = atoi(getWord(w, 1).c_str());
                            }
                            else
                            {
                                // Start counting up the number of new processes created each second.
                                gCreations += (atoi(getWord(w, 1).c_str()) - gOriginalCreationCount);
                            }
                        }
                    }
                    file.close();

                    // This section is for memory information, like how much memory 
                    // is free.
                    file.open("/proc/meminfo");
                    flag = 0;

                    while(!file.eof())
                    {
                        // This flag allows the parsing to terminate early.
                        if(flag == 3)
                        {
                            break;
                        }
                
                        getline(file, w);
                
                        // Acquires the total memory.
                        if(w.find("MemTotal") != -1)
                        {
                            flag = flag ^ 2;
                            gMemoryTotal += atoi(getWord(parseInfo(w),0).c_str());                        
                        }
                        else
                        // Acquires the memory that is free.
                        if(w.find("MemFree") != -1)
                        {
                            flag = flag ^ 1;
                            gMemoryFree += atoi(getWord(parseInfo(w),0).c_str());
                            gMemoryType = getWord(parseInfo(w),1);
                        }
                    }
                    file.close();


                    // This section is for getting successful reads/writes from/to sectors.
                    file.open("/proc/diskstats");

                    while(!file.eof())
                    {
                        getline(file, w);

                        // Finds the disk stats line.
                        if(w.find("sda ") != -1)
                        {
                            int sectors_written = atoi(getWord(w, 11).c_str());
                            int sectors_read    = atoi(getWord(w, 15).c_str());   
                            gSectors += (long long)sectors_written + (long long)sectors_read;            
                            break;         
                        }
                       
                    }
                    file.close();


                }
                // Unlocks the global variables for the output 
                pthread_mutex_unlock(&gMutex);

                // Sleeps.
                sleep(gReadValue);
            }
        }



    } else
    {
        // This is just wrong.
    }

    return 0;
}


/*
 * Function : printData
 * Parameter(s): 
 * a - Used by the pthread system
 * returns : n/a
 * Description: Used to print the data every now and then.
 */
void *printData(void* a)
{
    while(true)
    {
        sleep(gSleepValue);

        pthread_mutex_lock(&gMutex);
        {
            double total = gUserMode + gSystemMode + gIdle;

            // Prints out the cpu time percentages.
            cout << "User Time (%) : " << gUserMode/total * 100 << endl;
            cout << "System Time (%) : "  << gSystemMode/total * 100 << endl;
            cout << "Idle Time (%) : " << gIdle/total * 100 << endl;
            cout << endl;

            // Prints out the memory stats
            cout << "Memory Free (%) : " << gMemoryFree / gMemoryTotal * 100 << endl;
            cout << "Memory Free : " << gMemoryFree / gCount << " " << gMemoryType << endl;
            cout << endl;

            // Prints out the sector stats.
            cout << "Sectors Read/Written (per second): " << gSectors / gCount << endl;
            cout << endl;

            // Prints out the context switch stats.
            // I use gReadValue here because the gContexts value isn't pre-sampled by the os
            // per second. It merely measures change. If I used gCount, it would be measuring
            // average change per sample, not per second.
            cout << "Context Switches (per second): " << (gContexts / gCount) / gReadValue << endl;
            cout << endl;
        
            // Prints out the process creation stats (how many processes created per sec)
            // I use gReadValue here because the gCreations value isn't pre-sampled by the os
            // per second. It merely measures change. If I used gCount, it would be measuring
            // average change per sample, not per second.
            cout << "Process Creations (per second): " << (gCreations / gCount) / gReadValue << endl;
            cout << endl; 

            cout << "-------------" << endl;
            // Clears the variables for the next output averages. 
            gOriginalContextCount = gCount = gCreations = gOriginalCreationCount = gSectors = 0;
            gUserMode = gIdle = gUserMode = gSystemMode = gMemoryTotal = gMemoryFree =  0;
            
        }
        pthread_mutex_unlock(&gMutex);
    }
}