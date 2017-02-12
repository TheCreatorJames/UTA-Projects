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
#include <sys/wait.h>
#include <errno.h>


extern int errno; 

using namespace std;

namespace StringSplitFunctions
{
  //Not the most efficient but a working implementation.

  /*
   * Function : replacePart
   * Parameters:
   * original - The original string variable to be modified.
   * delim - what is supposed to be replaced.
   * replaceWith - what you are replacing delim with.
   * Description: Replaces part of the string with another string.
   * Pass in the original, and the delimiter.
   * Replaces delimiter with the 'replaceWith' parameter.
   */
  void replacePart(string& original, string delim, string replaceWith)
  {
    while (original.find(delim) != -1)
    {
      original.replace(original.find(delim), delim.size(), replaceWith);
    }
  }



  /*
   * Function : splitString
   * Parameters:
   * returner - a vector to place the split strings into.
   * delim - what string you wish to split upon.
   * op - the actual delimeter character used internally to split.
   * There probably isn't a reason to change it, but it is allowed.
   * Description : Splits a string into tokens.
   */
  void splitString(vector<string>& returner, string str, string delim, char op = 0)
  {
    string op2("");
    op2 += op;
    replacePart(str, delim, op2);
    istringstream stream(str);

    // While there is content in the stream, it gets processed and split up.
    while (stream)
    {
      string n;

      // Abuses the getline method to split the string into tokens.
      getline(stream, n, op);

      //avoids returning empty strings.
      if (n.size() == 0 || (n[0] == 0 && n.size() == 1))
      {
        //nothing
      }
      else
      {
        //something.
        returner.push_back(n);
      }
    }


  }
}

int main()
{
    ifstream file("/proc/cpuinfo");

    if(file.is_open())
    {
        
        string w;
        while(!file.eof())
        {
            getline(file, w);

            if(w.find("model name") == 0)
            {
                cout << w << endl;
            }
        }
    }

    return 0;
}