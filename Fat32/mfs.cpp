# include <iostream> 
#include <string> 
#include <string.h> 
#include <vector> 
#include <sstream> 
#include <fstream> 
#include <stdint.h>

using namespace std;

typedef unsigned char byte;

struct DirectoryEntry
{
    char dir_name[12];
    string parsed_name;
    uint8_t dir_attr;
    uint8_t unused[8];
    uint16_t dir_cluster_high;
    uint8_t unused2[4];
    uint16_t dir_cluster_low;
    uint32_t dir_filesize;
};

// This is not properly encapsulated, but that's fine. 
// This is a hybrid C/C++ - style thing anyway. 
struct FileSystemInfo
{
    char BS_OEMName[9];
    int16_t BPB_BytsPerSec;
    int8_t BPB_SecPerClus;
    int16_t BPB_RsvdSecCnt;
    int8_t BPB_NumFATs;
    int16_t BPB_RootEntCnt;
    char BS_VolLab[12];
    int32_t BPB_FATSz32;
    int32_t BPB_RootClus;

    int cur_address;

    int root_address()
    {
        return BPB_NumFATs * fat_size() + BPB_RsvdSecCnt * BPB_BytsPerSec;
    }

    int cluster_address(int i)
    {
        return (i - 2) * BPB_BytsPerSec + BPB_BytsPerSec * BPB_NumFATs * BPB_FATSz32 + (BPB_BytsPerSec * BPB_RsvdSecCnt);
    }

    int fat_size()
    {
        return BPB_BytsPerSec * BPB_FATSz32;
    }

    // get the fat position based on i.
    // i starts from 0, so if you want Fat #1, pass in 0. 
    int fat_pos(int i)
    {
        int offset = BPB_RsvdSecCnt * BPB_BytsPerSec;
        return offset + fat_size() * i;
    }
};

// These are simply used to convert.
namespace NumberFunctions
{
    inline int16_t ToInt16(byte a, byte b)
    {
        return (int16_t)((a << 8) | b);
    }

    inline int8_t ToInt8(byte a)
    {
        return (int8_t) a;
    }

    inline int32_t ToInt32(byte a, byte b, byte c, byte d)
    {
        return (int32_t)((a << 24) | (b << 16) | (c << 8) | d);
    }
}

using namespace NumberFunctions;


namespace StringFunctions
{

    // This is used to check if the volumes are empty.
    inline bool checkEmpty(char * p, int size)
    {
        bool result = true;
        for (int i = 0; i < size; i++)
        {
            if (!(p[i] == ' ' || p[i] == 0))
            {
                result = false;
            }
        }

        if (!result)
        {
            char m[] = "NO NAME    ";
            result = true;
            for (int i = 0; i < size; i++)
            {
                if (p[i] != m[i])
                {
                    result = false;
                    break;
                }
            }

        }

        return result;
    }


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
    void replacePart(string & original, string delim, string replaceWith)
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
    void splitString(vector < string > & returner, string str, string delim, char op = 0)
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


void printEntries(vector < DirectoryEntry > & entries)
{
    for (int i = 0; i < entries.size(); i++)
    {

        if (entries[i].dir_name[0] == -27) continue;

        if (entries[i].dir_name[0] == 0x05) entries[i].parsed_name[0] = -27;

        cout << entries[i].parsed_name << " ";
    }
    cout << endl;
}


vector < DirectoryEntry > GetEntries(ifstream & file, char * buf, int address)
{
    vector < DirectoryEntry > a;


    while (1)
    {
        DirectoryEntry entry;


        file.seekg(address);

        file.read(entry.dir_name, 11);
        string x(entry.dir_name);
        vector < string > sp;

        StringFunctions::splitString(sp, x, " ");

        if (sp.size() == 2)
        {
            x = sp[0] + "." + sp[1];
        }

        if (sp.size() == 1)
        {
            x = sp[0];

            if (sp[0].length() == 11)
            {
                x = sp[0].substr(0, 8) + "." + sp[0].substr(8);
            }
        }

        entry.parsed_name = x;

        file.read(buf, 1);
        entry.dir_attr = buf[0];

        file.read((char * ) entry.unused, 8);

        file.read(buf, 2);
        entry.dir_cluster_high = ToInt16(buf[1], buf[0]);

        file.read((char * ) entry.unused2, 4);

        file.read(buf, 2);
        entry.dir_cluster_low = ToInt16(buf[1], buf[0]);

        file.read(buf, 4);
        entry.dir_filesize = ToInt32(buf[3], buf[2], buf[1], buf[0]);

        if (entry.dir_name[0] == 0)
        {
            break;
        }

        a.push_back(entry);
        address += 32;
    }

    return a;
}



int main()
{
    string inp;
    ifstream file;
    vector < string > tokens;
    FileSystemInfo information;

    // Use these to make it more convenient to print.
    information.BS_OEMName[8] = 0;
    information.BS_VolLab[11] = 0;

    char * buf = new char[32];

    bool errored = false;

    // input loop.
    while (1)
    {
        if (errored)
        {
            cout << "Error: Filesystem not open." << endl;
            errored = false;
        }

        cout << "mfs> ";
        getline(cin, inp);

        if (inp.length())
        {
            tokens.clear();
            StringFunctions::splitString(tokens, inp, " ");

            // this is re-used, so we might as well, right?
            bool moreThanOne = tokens.size() > 1;
            bool lengthOfTwo = tokens.size() == 2;

            
            if (tokens[0] == "exit")
            {
                break;
            }
            else
            if (tokens[0] == "open")
            {
                if (lengthOfTwo)
                {

                    // closes previous filesystem automatically.
                    if (file.is_open())
                    {
                        file.close();
                        cout << "Ejecting previous file system." << endl;
                    }

                    file.open(tokens[1].c_str(), ios::binary);

                    if (file.good())
                    {
                        // Yay. It loaded. Now we can parse for data.
                        file.seekg(3);

                        //BS_OEMName
                        file.read(buf, 8);
                        memcpy(information.BS_OEMName, buf, 8);

                        //BPB_BytsPerSec
                        file.read(buf, 2);
                        information.BPB_BytsPerSec = ToInt16(buf[1], buf[0]);

                        //BPB_SecPerClus
                        file.read(buf, 1);
                        information.BPB_SecPerClus = ToInt8(buf[0]);

                        //BPB_RsvdSecCnt
                        file.read(buf, 2);
                        information.BPB_RsvdSecCnt = ToInt16(buf[1], buf[0]);

                        //BPB_NumFATs
                        file.read(buf, 1);
                        information.BPB_NumFATs = ToInt8(buf[0]);

                        //BPB_RootEntCnt
                        file.read(buf, 2);
                        information.BPB_RootEntCnt = ToInt16(buf[1], buf[0]);

                        //BPB_FATSz32
                        file.seekg(36);
                        file.read(buf, 4);
                        information.BPB_FATSz32 = ToInt32(buf[3], buf[2], buf[1], buf[0]);

                        //BPB_RootClus
                        file.seekg(44);
                        file.read(buf, 4);
                        information.BPB_RootClus = ToInt32(buf[3], buf[2], buf[1], buf[0]);

                        //BS_VolLab
                        file.seekg(71);
                        file.read(buf, 11);
                        memcpy(information.BS_VolLab, buf, 11);

                        information.cur_address = information.root_address();

                    }
                    else
                    {
                        cout << "Error: File system image not found." << endl;
                        file.close();
                    }

                }
                else
                {
                    cout << "open <filename>" << endl;
                }
            
            }
            else 
            if (tokens[0] == "stat")
            {
                if (!file.is_open())
                {
                    errored = true;
                    continue;
                }

                if (lengthOfTwo)
                {
                    bool found = false;

                    vector < DirectoryEntry > entries = GetEntries(file, buf, information.cur_address);
                    for (int i = 0; i < entries.size(); i++)
                    {


                        if (entries[i].parsed_name == tokens[1])
                        {
                            found = true;

                            int address2 = (entries[i].dir_cluster_high << 16) | entries[i].dir_cluster_low;
                            int attr = entries[i].dir_attr;

                            cout << "Attributes: ";

                            if (attr == (0x01 | 0x02 | 0x04 | 0x08))
                            {
                                cout << "Long-Name ";
                            }
                            else
                            {
                                if ((attr & 0x01) == 0x01)
                                {
                                    cout << "Read-Only ";
                                }

                                if ((attr & 0x02) == 0x02)
                                {
                                    cout << "Hidden ";
                                }

                                if ((attr & 0x04) == 0x04)
                                {
                                    cout << "System-File ";
                                }

                                if ((attr & 0x08) == 0x08)
                                {
                                    cout << "Volume ";
                                }
                                
                                if ((attr & 0x10) == 0x10)
                                {
                                    cout << "Sub-Directory ";
                                }

                                if ((attr & 0x20) == 0x20)
                                {
                                    cout << "Archive ";
                                }

                            }
                            
                          
                            cout << endl;
                            cout << "Starting Cluster: " << address2 << endl;
                            break;
                        }
                    }


                    if (!found)
                    {
                        cout << "Error: File not found." << endl;
                    }

                }
                else
                {
                    cout << "stat <directory name> or <filename>" << endl;
                }
            }
            else
            if (tokens[0] == "read")
            {
                if (!file.is_open())
                {
                    errored = true;
                    continue;
                }

                if (tokens.size() == 4)
                {
                    bool found = false;
                    vector < DirectoryEntry > entries = GetEntries(file, buf, information.cur_address);
                    for (int i = 0; i < entries.size(); i++)
                    {
                        if (entries[i].parsed_name == tokens[1] && (entries[i].dir_attr & 0x10) != 0x10)
                        {
                            int address2 = (entries[i].dir_cluster_high << 16) | entries[i].dir_cluster_low;
                            address2 = information.cluster_address(address2);

                            address2 += atoi(tokens[2].c_str());
                            int size = atoi(tokens[3].c_str());
                            char * b2 = new char[size + 1]();


                            file.seekg(address2);
                            file.read(b2, size);

                            cout << b2 << endl;
                            found = true;
                            delete[] b2;
                            break;
                        }
                    }

                    if (!found)
                    {
                        cout << "Error: File not found." << endl;
                    }


                }
                else
                {
                    cout << "read <filename> <position> <number of bytes>" << endl;
                }
            }
            else
            if (tokens[0] == "export")
            {
                if (!file.is_open())
                {
                    errored = true;
                    continue;
                }

                if (lengthOfTwo)
                {
                    bool found = false;
                    vector < DirectoryEntry > entries = GetEntries(file, buf, information.cur_address);
                    for (int i = 0; i < entries.size(); i++)
                    {
                        if (entries[i].parsed_name == tokens[1] && (entries[i].dir_attr & 0x10) != 0x10)
                        {
                            int address2 = (entries[i].dir_cluster_high << 16) | entries[i].dir_cluster_low;
                            address2 = information.cluster_address(address2);


                            ofstream o(entries[i].parsed_name.c_str(), ios::binary);

                            file.seekg(address2);

                            for (int j = 0; j < entries[i].dir_filesize; j++)
                            {
                                file.read(buf, 1);
                                o.write(buf, 1);
                            }
                            o.close();
                            found = true;
                            break;
                        }
                    }

                    if (!found)
                    {
                        cout << "Error: File not found." << endl;
                    }



                }
                else
                {

                }
            }
            else
            if (tokens[0] == "close")
            {

                if (lengthOfTwo)
                {
                    if (file.is_open())
                    {
                        file.close();
                    }
                    else
                    {
                        cout << "Error: No file open." << endl;
                    }
                }
                else
                {
                    cout << "close <filename>" << endl;
                }

            }
            else
            if (tokens[0] == "cd")
            {
                if (!file.is_open())
                {
                    errored = true;
                    continue;
                }

                int address = information.cur_address;

                if (lengthOfTwo)
                {
                    bool found = false;
                    bool file_found = false;
                    vector < DirectoryEntry > entries = GetEntries(file, buf, address);
                    for (int i = 0; i < entries.size(); i++)
                    {


                        if (entries[i].parsed_name == tokens[1])
                        {
                            found = true;
                            if ((entries[i].dir_attr & 0x10) != 0x10)
                            {
                                file_found = true;
                                break;
                            }

                            int address2 = (entries[i].dir_cluster_high << 16) | entries[i].dir_cluster_low;

                            if (!address2)
                            {
                                information.cur_address = information.root_address();
                                break;
                            }

                            address2 = information.cluster_address(address2);
                            information.cur_address = address2;
                            break;
                        }
                    }

                    if (file_found)
                    {
                        cout << "Error: Not a directory." << endl;
                    }

                    if (!found)
                    {
                        cout << "Error: File not found." << endl;
                    }
                }
                else
                {
                    cout << "cd <directory name>" << endl;
                }


            }
            else
            if (tokens[0] == "ls")
            {
                if (!file.is_open())
                {
                    errored = true;
                    continue;
                }


                int address = information.cur_address;

                if (lengthOfTwo)
                {
                    bool found = false;
                    bool file_found = false;
                    vector < DirectoryEntry > entries = GetEntries(file, buf, address);
                    for (int i = 0; i < entries.size(); i++)
                    {
                        if (entries[i].parsed_name == tokens[1])
                        {
                            found = true;

                            if ((entries[i].dir_attr & 0x10) == 0x10)
                            {
                                file_found = true;
                                break;
                            }

                            int address2 = (entries[i].dir_cluster_high << 16) | entries[i].dir_cluster_low;
                            address2 = information.cluster_address(address2);
                            vector < DirectoryEntry > ent2 = GetEntries(file, buf, address2);
                            printEntries(ent2);
                            break;
                        }
                    }

                    if (file_found)
                    {
                        cout << "Error: Not a directory." << endl;
                    }


                    if (!found)
                    {
                        cout << "Error: Directory not found." << endl;
                    }

                }
                else if (tokens.size() == 1)
                {
                    vector < DirectoryEntry > entries = GetEntries(file, buf, address);
                    printEntries(entries);
                }
                else
                {
                    cout << "ls <directory name>" << endl;
                }
            }
            else
            if (tokens[0] == "volume")
            {
                if (!file.is_open())
                {
                    errored = true;
                    continue;
                }

                if (!moreThanOne)
                {
                    if (StringFunctions::checkEmpty(information.BS_VolLab, 11))
                    {
                        cout << "Error: Volume Name Not Found." << endl;
                    }
                    else
                    {
                        cout << information.BS_VolLab << endl;
                    }
                }
                else
                {
                    cout << "volume doesn't accept parameters." << endl;
                }
            }
            else
            if (tokens[0] == "info")
            {
                if (!file.is_open())
                {
                    errored = true;
                    continue;
                }


                if (!moreThanOne)
                {
                    cout << "BPB_BytsPerSec: " << information.BPB_BytsPerSec << endl;
                    cout << "BPB_SecPerClus: " << (int) information.BPB_SecPerClus << endl;
                    cout << "BPB_RsvdSecCnt: " << information.BPB_RsvdSecCnt << endl;
                    cout << "BPB_NumFATs: " << (int) information.BPB_NumFATs << endl;
                    cout << "BPB_FATSz32: " << information.BPB_FATSz32 << endl;
                }
                else
                {
                    cout << "info doesn't accept parameters." << endl;
                }
            }
            else
            {
                cout << "Error: Command not found." << endl;
            }
        }



    }


    delete buf;
}