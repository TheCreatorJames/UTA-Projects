/*
 * Name: Jesse Daniel Mitchell
 * Id #: 1001449163
 * Programming Assignment 1 Part 2
 * Description: This is the mav shell assignment, where the goal is to create
 * a working shell.
 */

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#include <pthread.h>
#include <unistd.h>


using namespace std;

// Let's use a few globals for simplicity here, since the 
// scope of the program is extremely small.
long long gSleepValue, gSectors, gContexts, gCreations, gOriginalCreationCount; 
double gUserMode, gSystemMode, gIdle, gMemoryFree, gMemoryTotal;
string gMemoryType;
long long gCount;
pthread_mutex_t gMutex = PTHREAD_MUTEX_INITIALIZER;

void *printData(void* a);



string getWord(string str, int num)
{
    int count(-1), off(-1), off2(-1);
    while (count < num)
    {
        off = off2 + 1; 
        off2 = str.find(" ", off);
        count++;
    }

    if(off2 == -1)
    {
        if(off == -1)
        {
            return string("");
        }
        return str.substr(off);
    }

    return str.substr(off, off2-off);
}

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
    if(argc == 1)
    {
        ifstream file("/proc/cpuinfo");
        string w;

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
        
        file.open("/proc/version");

        getline(file, w);
        cout << "Linux Kernel Version: " << getWord(w, 2) << endl;

        file.close();

        
        file.close();
        file.open("/proc/meminfo");

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
        file.open("/proc/uptime");

        getline(file, w);
        cout << "Uptime: " << getWord(w, 0) << " seconds." << endl;

        file.close();
    } else if (argc == 3)
    {
        int read_rate = atoi(args[1]);
        gSleepValue = atoi(args[2]);

        pthread_t thread;
        int errorCode;

        if((errorCode = pthread_create(&thread, NULL, &printData, NULL)))
        {
            cout << "Thread Creation Failed: " << errorCode << endl;
        }
        else
        {
            string w; 
            ifstream file;
            while(true)
            {

                pthread_mutex_lock(&gMutex);
                {
                    file.open("/proc/stat");
                    
                    int flag = 0;
                    while(!file.eof())
                    {
                        if(flag == 7) break;

                        getline(file, w);

                        if(w.find("cpu ") == 0)
                        {
                            flag = flag ^ 1;

                            int userMode    = atoi(getWord(w, 2).c_str()),
                                userModeLow = atoi(getWord(w, 3).c_str()),
                                systemMode  = atoi(getWord(w, 4).c_str()),
                                idle        = atoi(getWord(w, 5).c_str());
                           
                            gUserMode += userMode + userModeLow;
                            gSystemMode += systemMode;
                            gIdle += idle; 
                        }
                        else
                        if(w.find("ctxt") == 0)
                        {
                            flag = flag ^ 2;
                            gContexts += atoi(getWord(w, 1).c_str());
                        }
                        else
                        if(w.find("processes") == 0)
                        {
                            flag = flag ^ 4;
                            if(!gOriginalCreationCount) 
                            {
                                gOriginalCreationCount = atoi(getWord(w, 1).c_str());
                            }
                            else
                            {
                                gCreations += (atoi(getWord(w, 1).c_str()) - gOriginalCreationCount);
                            }
                        }
                    }
                    file.close();

                  

                    gCount++;


                    file.open("/proc/meminfo");
                    flag = 0;

                    while(!file.eof())
                    {
                        if(flag == 3)
                        {
                            break;
                        }
                
                        getline(file, w);
                
                        if(w.find("MemTotal") != -1)
                        {
                            flag = flag ^ 2;
                            gMemoryTotal += atoi(getWord(parseInfo(w),0).c_str());                        
                        }
                        else
                        if(w.find("MemFree") != -1)
                        {
                            flag = flag ^ 1;
                            gMemoryFree += atoi(getWord(parseInfo(w),0).c_str());
                            gMemoryType = getWord(parseInfo(w),1);
                        }
                    }
                    file.close();

                    file.open("/proc/diskstats");

                    while(!file.eof())
                    {
                        getline(file, w);

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
                pthread_mutex_unlock(&gMutex);

                sleep(read_rate);
            }
        }



    } else
    {
        // This is just wrong.
    }

    return 0;
}

void *printData(void* a)
{
    sleep(gSleepValue);

    pthread_mutex_lock(&gMutex);
    {
        double total = gUserMode + gSystemMode + gIdle;

        cout << "User Time (%) : " << gUserMode/total * 100 << endl;
        cout << "System Time (%) : "  << gSystemMode/total * 100 << endl;
        cout << "Idle Time (%) : " << gIdle/total * 100 << endl;
        cout << endl;

        cout << "Memory Free (%) : " << gMemoryFree / gMemoryTotal * 100 << endl;
        cout << "Memory Free : " << gMemoryFree / gCount << " " << gMemoryType << endl;
        cout << endl;

        cout << "Sectors Read/Written : " << gSectors / gCount << endl;
        cout << endl;

        cout << "Context Switches : " << gContexts / gCount << endl;
        cout << endl;
    
        cout << "Process Creations: " << gCreations / gCount << endl;
        cout << endl; 

        cout << "-------------" << endl;
        gCount = gCreations = gOriginalCreationCount = gSectors = 0;
        gUserMode = gIdle = gUserMode = gSystemMode = gMemoryTotal = gMemoryFree =  0;
        
    }
    pthread_mutex_unlock(&gMutex);

    printData(a);
}