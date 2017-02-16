#include <stdio.h>
#include <stdlib.h>
#include "list_hw.h"

/* Builds and returns a list with integers read from standard input.
 * Note that it assumes the Item is int.
 * Notice that all the list access is done through it's interface (functions).
 */ 
list buildListOfInts() {
   list the_list = newList();

   link current_link = NULL;
   while(1)
   {
      Item number;
      printf("please enter an integer: ");     
      int items_read = scanf("%d", &number);
      if (items_read != 1)
      {
         break;
      }
     
      /* allocate memory for the next link */
      link next_item = newLink(number, NULL);
      insertLink(the_list, current_link, next_item);
      current_link = next_item;
   }   
   return the_list;
}


list arrayToList(int arr[], int sz)  {
	int i; 		
	link p, new_p;
	list A = newList();
	
	p = newLink(arr[0], NULL);
	insertLink(A, NULL, p);

	for (i = 1; i < sz; i++)
	{
		new_p = newLink(arr[i], NULL);
		insertLink(A, p, new_p);
		p = new_p;
	}
	
	return A;
}


void sublist_helper(int* a, int s1, int* o, int s2)
{
	list orig = arrayToList(a, s1); 
	list pos = arrayToList(o, s2);

	printf("\nList A:");
	printListHoriz(orig);
	
	printf("\nPOS List:");
	printListHoriz(pos);

	list r = sublist(orig, pos);
		
	printf("\nList Result:");
	printListHoriz(r);
		
	if(r)
	{
		printf("\nA is unchanged as:");
		printListHoriz(orig);
	}

	destroyList(r);
	destroyList(pos);
	destroyList(orig);
}

int test_sublist()  
{
	printf("\n\n--------------------------------------\n");            
	printf("Testing sublist.\n\n"); 


	printf("Out of Bounds\n--------"); 

	// Out of Bounds
	{
		int a[] = {10,10,40,20};
		int p[] = {3,80000,-2,3};	
		sublist_helper(a,4,p,4);
	}


	printf("Null A\n--------"); 

	// Null A
	{
		int p[] = { 0, 0, 0, 0 };

		list orig = 0;
		list pos = arrayToList(p, 4); 

		printf("\nList A:");
		printListHoriz(orig);
	
		printf("\nList POS:");
		printListHoriz(pos);

		list r = sublist(orig, pos);
		
		printf("\nList Result:");
		printListHoriz(r);
		
		destroyList(pos);
	}

	printf("Null pos_list\n--------"); 
	
	// Null pos_list
	{
		int a[] = { 0, 0, 0, 0 };

		list pos = 0;
		list orig = arrayToList(a, 4); 
		
		printf("\nList A:");
		printListHoriz(orig);

		printf("\nList POS:");
		printListHoriz(pos);

		list r = sublist(orig, pos);
		
		printf("\nList Result:");
		printListHoriz(r);
	
		destroyList(orig);	
	}


	printf("Not Modified and Original\n--------"); 

	// Check if A is not modified.
	// Also normal writeup. 
	{
		int a[] = {15,100,7,5,100};
		int p[] = {3,0,2};
		
		
		sublist_helper(a,5,p,3);
	}

	printf("Repeated Positions\n--------"); 
	
	// Repeated Position
	{
		int a[] = { 15, 100, 7, 5, 100 };
		int p[] = {0,0,0};

		sublist_helper(a,5,p,3); 
	}
	


	printf("\n----------  end test function -------------------\n");
	return 0;
}


int test_insertAtPosition(list A) {
	printf("\n\n--------------------------------------\n");            
	printf("Test insertAtPosition (for lists of ints).\n\n");
	printf("LIST A:");
	printListHoriz(A);

	printf("\nNormal Data\n-----------\n");
	insertAtPosition(A, 12, 0);

	printf("After Insert:\n");
	printListHoriz(A);


	printf("\nTrying to insert at null\n---------\n");
	insertAtPosition(0, 12, 0);
	
	printf("\nIt didn't segfault crash.\n");


	printf("\nTrying to insert into an empty list\n------------\n");
	list l = newList();
	printListHoriz(l);
	insertAtPosition(l, 12, 0);
	
	printf("After Insert:\n");
	printListHoriz(l);


	printf("\nPosition Greater than Length of A.\n----------\n");
	printListHoriz(l);
	printf("Inserting at Position 20\n");
	insertAtPosition(l, 15, 20);

	printf("After Insert:\n");
	printListHoriz(l);


	printf("\nInserting at negative position\n---------\n");
	
	printListHoriz(l);
	printf("Inserting at Position -50\n");
	insertAtPosition(l, 50, -50);

	printf("After Insert:\n");
	printListHoriz(l);




	
	destroyList(l);
	printf("\n----------  end test function -------------------\n");
	return 0;
}

void deleteOHelp(int* a, int s1, int v)
{	
	printf("\nV is : %i\n", v);
	
	list l;

	if(s1 > 0)
	{
		l  = arrayToList(a,s1); 
	}
	else if(s1 == 0)
	{
		l = newList();
	}
	else
	{
		l = 0; 
	}
	
	printf("\nBefore Deletion:");
	printListHoriz(l);
				
	deleteOccurrences(l, v);
	printf("\nAfter Deletion:");

	if(l) 	
	{
		printListHoriz(l);
	}


	destroyList(l);
}

int test_deleteOccurrences() {
	
	printf("\n\n--------------------------------------\n");            
	printf("Test deleteOccurrences (for lists of ints).\n\n");


	printf("\nNormal Data\n--------"); 
	// Normal Data
	{
		int a[] = {15,100,7,5,100,7,30};
		deleteOHelp(a, 7, 7);
	

	
		printf("\nWhere V does not occurs\n--------"); 
		// V does not occur
		deleteOHelp(a, 7, 9);
	}	


	printf("\nRepeated Positions\n--------"); 
	// Repeated Consecutive
	{
		int a[] = {15,7,7,7,5,7,7,2};
		deleteOHelp(a, 8, 7);
	}

	
	printf("\nOne item list, removed.\n--------"); 
	// One item
	{
		int a[] = {7};
		deleteOHelp(a,1,7);
	}



	printf("\nHas all removed items.\n--------"); 
	//Only has items.
	{
		int a[] = {7,7,7};
		deleteOHelp(a, 3, 7);
	}


	printf("\nA is null.\n--------"); 
	//A is null.
	{
		deleteOHelp(0, -1, 5); 	
	}


	printf("\nA is empty\n--------"); 
	//A is empty
	{
		int a[] = {};
		deleteOHelp(a, 0, 5);
	}

	



	printf("\n----------  end test function -------------------\n");
	return 0;
}

int test_moveAllMaxAtEnd(list A) {
	printf("\n\n--------------------------------------\n");            

	printf("Test moveAllMaxAtEnd (for lists of ints).\n\n");

	// Test "Normal Data"
	printf("Test Normal Data\n-------\n");
	{	
		printf("LIST A:");
		printListHorizPointer(A);

		moveAllMaxAtEnd(A);
			
		printf("After calling moveAllMaxAtEnd.\n");
		printListHorizPointer(A);
	}

	printf("\nTest A is NULL\n--------\n");
	{
		moveAllMaxAtEnd(0);
		printf("It didn't crash.\n\n");
	}

	printf("\nTest A is empty\n-------\n");
	{
		printf("LIST A:");
		A = newList();
		printListHorizPointer(A);

		moveAllMaxAtEnd(A);
		
		printf("After calling moveAllMaxAtEnd.\n");
		printListHorizPointer(A);
		destroyList(A);
	}

	printf("\nA has one item\n------\n");
	{
		int a[] = {7};
		A = arrayToList(a, 1);
			
		printf("LIST A:");
		printListHorizPointer(A);

		moveAllMaxAtEnd(A);
			
		printf("After calling moveAllMaxAtEnd.\n");
		printListHorizPointer(A);
		destroyList(A);
	}


	printf("\nA has only max items\n------\n");
	{
		int a[] = {7,7,7};
		A = arrayToList(a, 3);	
	
		printf("LIST A:");
		printListHorizPointer(A);

		moveAllMaxAtEnd(A);
			
		printf("After calling moveAllMaxAtEnd.\n");
		printListHorizPointer(A);
		destroyList(A);
	}
	
	printf("\nMAX is first position\n-----\n");
	{
		int a[] = {100, 7, 20};
		A = arrayToList(a,3);
	
		printf("LIST A:");
		printListHorizPointer(A);

		moveAllMaxAtEnd(A);
			
		printf("After calling moveAllMaxAtEnd.\n");
		printListHorizPointer(A);
		destroyList(A);
	}

	printf("\nMAX is last position\n-----\n");
	{
		int a[] = {10, 7, 200};
		A = arrayToList(a,3);
	
		printf("LIST A:");
		printListHorizPointer(A);

		moveAllMaxAtEnd(A);
			
		printf("After calling moveAllMaxAtEnd.\n");
		printListHorizPointer(A);
		destroyList(A);
	}

	printf("\n----------  end test function -------------------\n");
	return 0;
}

int main() {	
    list A = NULL;
    int arr[] = {15, 100, 7, 5, 100, 7, 30};	
    A = arrayToList(arr, 7);
    //A = buildListOfInts() ;  // use this line and redirect input to run test cases from data files. See posted example data file.
    printf("LIST A:");
    printList(A);
    
    test_sublist();   
    test_deleteOccurrences();     
    test_insertAtPosition(A); 
    
    destroyList(A);
    A = arrayToList(arr, 7);
   
    test_moveAllMaxAtEnd(A);   	
    
    destroyList(A);  
    return 0;    
}
