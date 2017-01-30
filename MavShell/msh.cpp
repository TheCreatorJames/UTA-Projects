/*
 * Name: Jesse Daniel Mitchell
 * Id #: 1001449163
 * Programming Assignment 1
 * Description: This is the mav shell assignment, where the goal is to create
 * a working shell.
 */

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <sys/wait.h>

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
  // Creates storage for all the input and process ids.
  string line;
  char *buf = new char[100];
  char *exec_args[11];
  int   pids[10], pid_pos(0);
  vector<string> tokens;

  while (true)
  {
    // Clears the tokens from the previous time we parsed a line of input, then prompts the user for another line of input.
    tokens.clear();
    cout << "msh> ";
    getline(cin, line);

    // Parses the line of input.
    StringSplitFunctions::splitString(tokens, line, " ");

    // Checks if the has any content.
    if (tokens.size() > 0)
    {
      //Allows the user to exit.
      if (tokens[0] == "exit")
      {

        break;
      }
      // Might be a small easter egg later on.
      else if (tokens[0] == "help");
      // Allows you to change directories.
      else if (tokens[0] == "cd")
      {
        // For my convenience, I got a substring of the original line typed in, so that I didn't have to parse through the tokens.
        string dir = line.substr(3,line.size());
        int suc = chdir(dir.c_str());

        // Commented out but originally written for debugging purposes
        //	getwd(buf);
        //	cout << "Current Directory is Now: " << buf << endl;
      }
      // Shows the past ten process ids. Uses modular arithmetic to help.
      else if (tokens[0] == "showpid")
      {
        // Loops through the past ten process ids using modular arithmetic.
        for(int i = 1; i <= 10 && pid_pos - i >= 0; i++)
        {
          cout << pids[(pid_pos-i) % 10] << endl;
        }
      }
      // If it isn't a command, try to execute a program.
      else
      {
        // Copies the tokens into a char* array buffer. Yes, I know that this is extra work,
        // And that committing to not using "strings" might've helped, but for the sake of simplicity
        // as the shell is extended, I've committed to using strings, in case I want to add scripting later on.
        for (int i = 0; i < tokens.size(); i++)
        {
          exec_args[i] = strdup(tokens[i].c_str());
        }

        // Places a null pointer so that the method knows when to stop looking for args.
        exec_args[tokens.size()] = NULL;

        // Creates a status variable and forks.
        int *stat;
        int pid = fork();

        if(!pid)
        {
          // Executes the program.
          execvp(exec_args[0], exec_args);

          // If there was an error, print it out and exit.
          perror(exec_args[0]);
          exit(1);
        }
        else
        {
          // Adds the new PID to my list
          pids[pid_pos++ % 10] = pid;
        }

        // Waits for the other process to exit.
        waitpid(pid, stat, 0);
      }
    }
    else
    {
      // well, this was an empty line, so go on.
    }
  }

  // Not technically necessary, but let's remember to free up our memory.
  for(int i = 0; i < 11; i++)
  {
    delete[] exec_args[i];
  }

  delete[] buf;

  return 0;
}
