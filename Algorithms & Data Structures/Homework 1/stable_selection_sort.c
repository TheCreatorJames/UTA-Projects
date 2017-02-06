
//Written By: Jesse Daniel Mitchell
//Date Started: January 26, 2016
//Dates Modified: 
//Additional Notes: N/A

#include <stdio.h>

//These two lines are technically unnecessary, but I include them so that the compiler will stop throwing warnings
//for 'compatible' code. 
#include <stdlib.h>
#include <string.h>


const int LINE_SIZE = 120; 

//This is the simple data structure.
struct DataStructure
{
	char* text;
	int num;	
};

//Creates and Allocates Memory for the Data Structure. Copies the string. 
//You should pass in a char buffer that is at least 120 bytes. 
struct DataStructure* MakeDataStructure(char* t, int num)
{
	struct DataStructure* r = (struct DataStructure*)malloc(sizeof(struct DataStructure)); 

	r->num = num; 
	r->text = malloc(LINE_SIZE);
	memcpy(r->text, t, LINE_SIZE); 
	return r;
}

//Returns a positive value if the first input has a larger number value than the second (greater than)
//Returns a zero value if they are equal.
//Returns a negative value if the second input is larger. (less than)
int compare(struct DataStructure* info, struct DataStructure* info2)
{
	if(!info || !info2) return 0; 
	return info->num - info2->num; 
}

//Destroys the data structure.
void DestroyDataStructure(struct DataStructure* info)
{
	if(!info) return; 
	free(info->text); 
	free(info); 
}

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
void printArray(struct DataStructure** arr, int count)
{
	int s;
	for(s  = 0; s < count; s++)
	{
		if(!arr[s]) continue;
		struct DataStructure* info = arr[s];
		printf("%i, %s\n", info->num, info->text); 
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
	char *text, *text2;
		
	text = malloc(LINE_SIZE);  
	text2 = malloc(LINE_SIZE); 

	printf("Enter the number of records:\n");
	scanf("%[^\n]", text2);
	int count = atoi(text2);
	
	//Create Values for Parsing Help
	int val, mode, lm, offset, iter;

	//Creates a Data Structure Array to store the information brought in.
	struct DataStructure** arr = (struct DataStructure**)malloc(sizeof(struct DataStructure*)*count);

	
	//Query for the data pair input.
	printf("Enter the data pairs(number string):\n");	
	
	//Take in the selected number of lines.
	for(iter = 0; iter < count; iter++)
	{
		scanf("\n%[^\n]", text);
		val = mode = lm = offset = 0;

		// I could have used sscanf, but this was easier to customize.
		while(text[offset] != 0)
		{
			//ignores return carriage
			if (text[offset] == '\r')
			{
				text2[lm] = 0; 	
			}
			//Parses the integer if there is a comma. I swear the prompt was changed on us at some point, so I have support for both file formats.
			else if(text[offset] == ',' || (text[offset] == ' ' && mode == 0))
			{
				if(text[offset+1] == ' ') offset++; 
				text2[lm] = 0;
				val = atoi(text2);  		
				mode = 1;
				lm = -1;
			}
			else
			//Copies into our second line buffer.
			text2[lm] = text[offset];


			//Increment through our buffers.
			lm++;
			offset++;
		}

		//Ensures that the input line will be consumed. 
		if(lm != -1)
		{
			text2[lm] = 0;
			struct DataStructure* info = MakeDataStructure(text2, val);
			arr[iter] = info;
		}

	}

	//Create a Second Data Structure Array, and copy it from the first. These are merely pointers, and don't have much overhead.
	struct DataStructure** arr2 = (struct DataStructure**)malloc(sizeof(struct DataStructure*)*count);
	memcpy(arr2, arr, sizeof(struct DataStructure*) * count); 

	//Prints out the original loaded data.
	printf("Loaded data:\n");
	printArray(arr, count); 
	
	//Sort and print out the array (unstable)
	printf("\nData sorted with UNSTABLE selection sort:\n");
	selectionSort(arr, count); 
	printArray(arr, count);
	
	//Sort and print out the array (stable)
	printf("\nA copy of the original data, sorted with STABLE selection sort:\n");
	insertionSort(arr2, count); 
	printArray(arr2, count);
	
	//free our memory
	DestroyDataArray(arr, count);
	free(arr2);  // this does not call DestroyDataArray, since we do not need to free the same variables twice. 
	free(text);
	free(text2);
	
	return 0;
}
