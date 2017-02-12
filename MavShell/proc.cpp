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

using namespace std;

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
    }

    return 0;
}