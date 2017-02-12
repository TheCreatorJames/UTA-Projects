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

string parseInfo(string str)
{
    str = str.substr(str.find(":") + 1);
    while(str[0] == ' ' || str[0] == '\t')
    {
        str = str.substr(1);
    }
    return str;
}

int main()
{
    ifstream file("/proc/cpuinfo");
    string w;
    
    while(!file.eof())
    {
        getline(file, w);

        if(w.find("model name") == 0)
        {
             cout << parseInfo(w) << endl;
        }
    }
    
    file.close();
    file.open("/proc/meminfo");

    while(!file.eof())
    {
        getline(file, w);

        if(w.find("MemTotal") == 0)
        {
            cout << parseInfo(w) << endl;
        }
    }

    file.close();

    return 0;
}