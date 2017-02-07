
//Written By: Jesse Daniel Mitchell
//Date Started: February 6, 2016
//Dates Modified: February 6, 2016
//Additional Notes: N/A


//This following line is included because of an ambiguity in the prompt.
//I wanted to trim the punctuation before the sort, but here is a mode to
//determine the algorithm's behavior.
#define REMOVE_PUNCTUATION false

#include <stdio.h>
//These two lines are technically unnecessary, but I include them so that the compiler will stop throwing warnings
//for 'compatible' code.
#include <stdlib.h>
#include <string.h>

// A constant for the line size.
const int LINE_SIZE = 128;

//This is the simple data structure.
struct DataStructure
{
  char* text;
};

//Creates and Allocates Memory for the Data Structure.
//Unlike the previous assignment, this does not make a copy of the text. It merely uses a pointer to point at the text. ;)
struct DataStructure* MakeDataStructure(char* t)
{
  struct DataStructure* r = (struct DataStructure*)malloc(sizeof(struct DataStructure));
  r->text = t;
  return r;
}

//Compares the two data structures by using their string pointers, and calling strcmp.
int compare(struct DataStructure* info, struct DataStructure* info2)
{
  //checks to see if both pointers are valid.
  if(!info || !info2) return 0;
  return strcmp(info->text, info2->text);
}

#if !REMOVE_PUNCTUATION
// Tries to find out if the two strings are the same even with punctuation
int samestringcheck(char* p, char* p2)
{
  int count  = 0;
  while(p[count] && (p[count] == p2[count]))
  {
    count++;
  }

  if(p[count] == p2[count]) return 0;

  if(p[count] == '.' || p[count] == ',' || p[count] == ':' || p[count] == ';' || p[count] == '!' || p[count] == '?')
  {
    if(p[count+1] == 0) return 0;
  }

  if(p2[count] == '.' || p2[count] == ',' || p2[count] == ':' || p2[count] == ';' || p2[count] == '!' || p2[count] == '?')
  {
      if(p2[count+1] == 0) return 0;
  }

  return strcmp(p, p2);
}
#endif

//Destroys the data structure.
void DestroyDataStructure(struct DataStructure* info)
{
  free(info);
}

//A standard binary search algorithm.
int binarySearch(struct DataStructure** arr, char* txt, int count, int* iter)
{
  int low = 0, high = count - 1;

  while(low <= high)
  {
    *iter = *iter + 1;
    int cur = (low+high)/2;

    #if REMOVE_PUNCTUATION
    int s = strcmp(arr[cur]->text,txt);
    #else
    int s = samestringcheck(arr[cur]->text, txt);
    #endif

    if(!s)
    {
      return cur;
    }
    else if(s < 0)
    {
      low = cur + 1;
    }
    else if(s > 0)
    {
      high = cur - 1;
    }
  }

  return -1;
}


// This is the "Stable" Selection Sort
void insertionSort(struct DataStructure** arr, int count)
{
  struct DataStructure* lowest, *temp;
  int i,j;
  for(i = 1; i < count; i++)
  {
    j = i-1;
    lowest = arr[i];
    while (j >= 0 && compare(arr[j], lowest) > 0)
    {
      arr[j+1] = arr[j];
      j--;
    }

    arr[j+1] = lowest;
  }
}

// Print out the array.
void printArray(struct DataStructure** arr, char* text, int count)
{
  int s;

  printf(" i\t| index[i]\t| word\n");
  printf("--------|---------------|--------\n");

  for(s  = 0; s < count; s++)
  {
    struct DataStructure* info = arr[s];
    if(info)
    printf(" %i\t| %i\t\t| %s\n", s, (int)(info->text) - (int)text, info->text);
  }
}

// Go through the data structure array and free the memory.
void DestroyDataArray(struct DataStructure** arr, int count)
{
  int s;
  for(s  = 0; s < count; s++)
  {
    DestroyDataStructure(arr[s]);
  }
  free(arr);
}

int main()
{
  // Create buffers to store the input and perform data manipulation in.
  char *text, *text2, *text3;
  text  = malloc(LINE_SIZE * 64);
  text2 = malloc(LINE_SIZE);
  text3 = malloc(LINE_SIZE);

  //Read in the first line.
  scanf("%[^\n]", text);

  //Create Values for Parsing Help
  int offset = 0, val = 1;
  int count = 1;

  //Cleans the input.
  while(text[offset] != 0)
  {
    //Tries to predict how much memory is needed for allocation.
    if(text[offset] == ' ' /*|| text[offset] == '\n'*/ || text[offset] == '\t') count++;

    //Makes all characters lower case.
    if(text[offset] >= 'A' && text[offset] <= 'Z')
    {
      text[offset] += 'a' - 'A';
    }

    offset++;
  }

  //sets the offset to zero for further parsing.
  offset = 0;

  //Creates a Data Structure Array to store the information brought in.
  struct DataStructure** arr = (struct DataStructure**)malloc(sizeof(struct DataStructure*)*count);

  //Creates a pointer to the current word.
  char *cur = text + offset;
  count = 0;

  //Parses through the original text, finding spaces and tabs.
  while(text[offset] != 0)
  {
    //Checks if the current character is a space, tab, or carriage return character and if it is
    //it will make a new structure.
    if(text[offset] == ' ' /*|| text[offset] == '\n' */|| text[offset] == '\r'  || text[offset] == '\t')
    {
      //sets it to zero to make it easy for my functions to utilize the pointer.
      text[offset] = 0;

      //this block is used to skip empty strings, and carry on.
      if(!cur)
      {
        offset++;
        continue;
      }

      //makes the new data structure.
      arr[count++] = MakeDataStructure(cur);


      #if REMOVE_PUNCTUATION
      //trims the string (if necessary)
      if(text[offset - 1] == '.' || text[offset -1] == ',' || text[offset-1] == '!' || text[offset-1] == ':' || text[offset-1] == ';' || text[offset-1] == '?') text[offset-1] = 0;
      #endif

      //sets the current pointer to null.
      cur = 0;
    }
    else if(cur == 0)
    {
      //sets the pointer to the current string.
      cur = text + offset;
    }

    offset++;
  }

  //If there is an existing current pointer, add it to the collection.
  if(cur)
  {
    #if REMOVE_PUNCTUATION
    //trims the final character, if necessary.
    if(text[offset - 1] == '.' || text[offset -1] == ',' || text[offset-1] == '!' || text[offset-1] == ':' || text[offset-1] == ';' || text[offset-1] == '?')
      text[offset-1] = 0;
    #endif
    arr[count++] = MakeDataStructure(cur);
  }


  //Print the collection of words.
  printf("Words and indexes (the printed words are 'cleaned'):\n");
  printArray(arr, text, count);

  //Print the sorted collection of words.
  printf("\nThe sorted data (the printed words are 'cleaned'):\n");
  insertionSort(arr, count);
  printArray(arr, text, count);

  //Inform the user of the binary search instructions.
  printf("\nBinary search. ---- Enter words to search for. Stop with -1.\n");

  //Start querying for data.
  scanf("\n%[^\n]", text2);
  val = 1;

  //Create a  pointer.
  int *iter = malloc(sizeof(int));

  while(val)
  {
    offset = 0;

    //Copy the string (so we can print out the original).
    memcpy(text3, text2, 110);

    //Clean the input.
    while(text2[offset] != 0)
    {
      // Makes uppercase characters lowercase
      if(text2[offset] >= 'A' && text2[offset] <= 'Z')
      {
        text2[offset] += 'a' - 'A';
      }

      //Removes any carriage return characters, just in case.
      if(text2[offset] == '\r')
      {
        text3[offset] = text2[offset] = 0;
      }

      offset++;
    }

    //shortcut bugfix, shortens code a lot by doing it this way.
    val = strcmp("-1", text3);
    if(!val) continue;

    //Trim the last character, as necessary.
    if(text2[offset-1] == '.' || text2[offset-1] ==',' || text2[offset-1] == '!' || text2[offset-1] == ' ' || text2[offset-1] == ':' ||  text2[offset-1] == '?' ||  text2[offset-1] == ';')
    {
      //If the last character is a space, go ahead and trim it off text3 as well.
      if(text2[offset-1] == ' ')
      {
        text3[offset-1] = 0;
      }
      text2[offset-1] = 0;
    }

    //Get the value of the binary search
    val = binarySearch(arr, text2, count, iter);

    //Print out the original and cleaned strings.
    printf("(original: %s)\ncleaned: %s - ",text3, text2);

    //Print out whether the value was found in the collection or not.
    if(val == -1)
    {
      printf("not found ");
    }
    else
    {
      printf("found ");
    }

    //Print out how many iterations it took to get there.
    printf("(%i iterations)\n", *iter);

    //Clear the iteration counter.
    *iter = 0;

    //Query for new input.
    scanf("\n%[^\n]", text2);
  }


  //free our memory
  DestroyDataArray(arr, count);
  free(iter);
  free(text);
  free(text2);
  free(text3);

  return 0;
}

#undef REMOVE_PUNCTUATION
