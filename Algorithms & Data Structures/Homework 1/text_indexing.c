
//Written By: Jesse Daniel Mitchell
//Date Started: February 6, 2016
//Dates Modified: February 6, 2016
//Additional Notes: N/A

#include <stdio.h>
//These two lines are technically unnecessary, but I include them so that the compiler will stop throwing warnings
//for 'compatible' code. 
#include <stdlib.h>
#include <string.h>

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

//Destroys the data structure.
void DestroyDataStructure(struct DataStructure* info)
{
	free(info); 
}

/* Leftover code from previous assignment. 
// This is the "Unstable" Selection Sort.
void selectionSort(struct DataStructure** arr, int count)
{
	struct DataStructure* lowest, *temp;
	int i,j, l;
	for(i = 0; i < count; i++)
	{
		
		lowest = arr[i];
		temp = arr[i];
		l = i;
		for(j=i; j < count; j++)
		{
			if(compare(lowest,arr[j]) > 0)
			{
				l = j;
				lowest = arr[j];
			}
		}	

		arr[i] = lowest;
		arr[l] = temp; 
	}
}
*/


//A standard binary search algorithm. 
int binarySearch(struct DataStructure** arr, char* txt, int count, int* iter)
{
	int low = 0, high = count - 1;
	
	while(low <= high)
	{
		*iter = *iter + 1;
		int cur = (low+high)/2;
		int s = strcmp(arr[cur]->text,txt);
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
	// Create a Buffer to Store the Entire File, and a Second Buffer for Parsing Lines.
	char *text, *text2, *text3;
	text = malloc(4096); 
	text2 = malloc(110);
	text3 = malloc(110); 

	
	scanf("%[^\n]", text); //Read in the Entire File. 
	
	int offset = 0, val = 1; //Create Values for Parsing Help
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
			
			//trims the string (if necessary)
			if(text[offset - 1] == '.' || text[offset -1] == ',' || text[offset-1] == '!' || text[offset-1] == ':') text[offset-1] = 0;
			
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
		//trims the final character, if necessary. 
		if(text[offset - 1] == '.' || text[offset -1] == ',' || text[offset-1] == '!') text[offset-1] = 0;	
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
	val = strcmp("-1", text2);

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
		
		//Trim the last character, as necessary.
		if(text2[offset-1] == '.' || text2[offset-1] ==',' || text2[offset-1] == '!' || text2[offset-1] == ' ' || text2[offset-1] == ':')
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
		val = strcmp("-1", text2);
	}
	
	
	//free our memory
	DestroyDataArray(arr, count);
	free(iter);
	free(text);
	free(text2);
	free(text3);	

	return 0;
}
