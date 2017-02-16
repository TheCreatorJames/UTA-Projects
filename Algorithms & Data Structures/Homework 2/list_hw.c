/* This program uses code from "Algorithms in C, Third Edition,"
 *               by Robert Sedgewick, Addison-Wesley, 1998.
 * 
 */

#include <stdio.h>
#include <stdlib.h>

#include "list_hw.h"


struct node_struct {
    Item item;
    link next;
};

struct list_struct {
    link first;
    int length;    
};




// ------------- link functions

/* Creates a new link, that contains the value specified in the argument, 
 * and that points to next_in. 
 */
link newLink(Item value, link next_in) {
    link result = (link) malloc(sizeof (struct node_struct));
    result->item = value;
    result->next = next_in;
    return result;
}

void destroyLink(link the_link)  {
    if (the_link != NULL) {
        free(the_link);
    }
}

Item getLinkItem(link the_link) {
    if (the_link != NULL) {
        return the_link->item;    
    }
    else {
        printf("\n getLinkItem: Empty link. Returned -1.");
        return -1;    // NOTE: This line can be improved.
    }
}


link getLinkNext(link the_link) {
    if (the_link != NULL) {
        return the_link->next;
    }
    else {
        return NULL;
    }
}

void setLinkItem(link the_link, Item value) {
    if (the_link != NULL) {
        the_link->item = value;
    }
}

void setLinkNext(link the_link, link next) {
    if (the_link != NULL) {
        the_link->next = next;
    }
}



/*    --------  LIST functions    */

int listIsNULL(list the_list) {
    if (the_list == NULL) {
        printf("\nThe list pointer is NULL)\n");
        return 1;
    }     
    else {
        return 0;
    }    
}

/* Creates and returns an empty list. */
list newList() {
    list result = (list) malloc(sizeof (*result));
    result->first = NULL;
    result->length = 0;
    return result;
}

/* Deallocates memory for all nodes in the list and the list object itself. */
void destroyList(list the_list) {
    if (listIsNULL(the_list) == 1) {        
        return;
    }
    
    link current = the_list->first;
    while (1) {
        if (current == NULL) {
            break;
        }

        link next = current->next;
        destroyLink(current);
        current = next;
    }
    free(the_list);
}

link getFirst(list the_list) {
    if (listIsNULL(the_list) == 1) {        
        return NULL;
    }
    
    return the_list->first;
}

int getLength(list the_list) {
    if (listIsNULL(the_list) == 1) {        
        return -1;
    }
    
    return the_list->length;
}


/* Sets the data to empty (first is null, length is 0,...)
 * It does NOT free any memory.
 */ 
void setEmpty(list the_list) {
    the_list->first = NULL;    
    the_list->length = 0;
}


/* Inserts new_link to the specified list, at the position right after
 * the link called "previous". */
void insertLink(list my_list, link previous, link new_link) {
    if (listIsNULL(my_list) == 1) {        
        return;
    }    
    
    /* We need a special case when we want to insert to the beginning.
     * In that case, the previous link is NULL. */
    if (previous == NULL) {
        new_link->next = my_list->first;
        my_list->first = new_link;
    } else {
        new_link->next = previous->next;
        previous->next = new_link;
    }
    
    my_list->length += 1;
}

/* Inserts new_link to the beginning of the list. */
void insertAtBeginning(list my_list, link new_link) {
    setLinkNext(new_link, getFirst(my_list));  // replaces: new_link->next = my_list->first;
    my_list->first = new_link;
    my_list->length += 1;
}

/* Removes from the list and returns the link coming AFTER link x. 
 * This function DOES NOT remove link x itself. 
 */
link removeNext(list my_list, link x) {
    if (listIsNULL(my_list) == 1) {        
        return NULL;
    }     
    
    link temp;
    if (x == NULL) // try to delete the first node
    {
        temp = my_list->first;
        if (my_list->first != NULL) // There is at least one node in the list.
        {
            my_list->first = my_list->first->next;
            my_list->length -= 1;
        }
    } else {
        temp = x->next;
        x->next = x->next->next;  // IS THIS CODE SAFE? JUSTIFY YOUR ANSWER.
        my_list->length -= 1;
    }
    return temp;
}

/* Removes from the list and returns the link at the beginning of the list.
It does not free the memory for that node. */
link removeFirst(list my_list) {
    if (listIsNULL(my_list) == 1) {        
        return NULL;
    }    
    
    link first_node = my_list->first;
    if (first_node != NULL) {
        my_list->first = first_node->next;
        my_list->length -= 1;
    }
    return first_node;
}

void reverse(list the_list) {
    if (listIsNULL(the_list) == 1) {        
        return;
    }    
    
    link current = the_list->first;
    link previous = NULL;
    while (current != NULL) {
        link temp = current->next;
        current->next = previous;
        previous = current;
        current = temp;
    }
    the_list->first = previous;
}

void printList(list my_list) {
    if (listIsNULL(my_list) == 1) {        
        return;
    }     

    
    int i = 0;
    int n = my_list->length;
    link curr;    
    printf("\n List length = %d\n List items:\n", my_list->length);
    for (i = 0, curr = my_list->first; (curr != NULL) && (i<n) ; curr = curr->next) {
        printf("item %d: %d\n", i, curr->item);
        i++;
    }
    printf("\n");
}


void printListHoriz(list my_list) {
    if (listIsNULL(my_list) == 1) {        
        return;
    }     

    
    int i = 0;
    int n = my_list->length;
    link curr;    
    printf("\n List length = %d\n List items:\n", my_list->length);
    for (i = 0, curr = my_list->first; (curr != NULL) && (i<n) ; curr = curr->next) {
        printf("%5d    ", curr->item);
        i++;
    }
    printf("\n");	
}

void printListHorizPointer(list my_list) {
    if (listIsNULL(my_list) == 1) {        
        return;
    }     

    
    int i = 0;
    int n = my_list->length;
    link curr;    
    printf("\n List length = %d\nList items:    ", my_list->length);
    for (i = 0, curr = my_list->first; (curr != NULL) && (i<n) ; curr = curr->next) {
        printf("%-11d  ", curr->item);
        i++;
    }
    printf("\n");
	printf("List pointers: ");
    for (i = 0, curr = my_list->first; (curr != NULL) && (i<n) ; curr = curr->next) {
        printf("%p ", curr);
        i++;
    }
    printf("\n");
}


int computeLength(list my_list) {
     if (listIsNULL(my_list) == 1) {        
        return -1;
    }     
    
    int counter = 0;
    link i;
    for (i = my_list->first; i != NULL; i = i->next) {
        counter++;
    }
    return counter;
}

list listDeepCopy(list input) {    
    list result = newList();
    link in = getFirst(input);
    link previous = NULL;
    while (in != NULL) {
        link out = newLink(getLinkItem(in), NULL);
        insertLink(result, previous, out);
        previous = out;
        in = getLinkNext(in);
    }

    return result;
}

/* returns a new list that contains all values of input1 and all values 
 * of input2
 */
list concatLists(list input1, list input2) {
    list result = listDeepCopy(input1);
    list temp2 = listDeepCopy(input2);
    concatListsDestructive(result, temp2);
    free(temp2);
    return result;
}

/* Modifies target, by inserting at the end of it all links from source.  */
void concatListsDestructive(list target, list source) {
    link previous = NULL;
    link c;

    /* find the last link of target*/
    for (c = getFirst(target); c != NULL; c = getLinkNext(c)) {
        previous = c;
    }

    /* at this point, previous is the last link of target */
    setLinkNext(previous, getFirst(source));
    target->length = target->length + source->length;  // important      
    
    /* Source should not link to the moved nodes any more (when target and source 
     * lists get destroyed, each of them will attempt to free the nodes in 
     * source).
     */
    setEmpty(source);
}



/*----------------------------------------------------------------
 New functions.
 Finish their implementation below. 
 If you need to use helper functions you can define them and use them in this file.
 You should not change the behavior of any of the functions defined above.
 */


// N time
void _insertAtPosition(link A, Item P, int pos)
{
	if(!A) return;

	link B = A;
	A = A->next;
	pos--; 
	
	if(pos == 0)
	{
		// Example : pos = 1 when it begins.
		// B - P - A 
		B->next = newLink(P, A);
	}
	else
	_insertAtPosition(A, P, pos);
}


// N time
void insertAtPosition(list A, Item P, int pos)
{
	if(!A) return;
		
	// Fixes positions that are too high.
	if (pos > A->length)
	{
		pos = A->length; 
	}

	// Fixes positions that are too low.
	if (pos < 0)
	{
		pos = 0;		
	}	

	if (pos == 0)
	{
		A->length++;
		A->first = newLink(P, A->first); 
	}
	else
	{
 		A->length++;
		_insertAtPosition(A->first, P, pos);
	}
}


// N time
void deleteOccurrences(list A, Item V) 
{
	if(!A || A->length == 0) return;
	
	while(A->first && A->first->item == V)
	{
		A->length--;

		// This can easily be removed, but I'm assuming this is what is desired.
		destroyLink(A->first);

		A->first = A->first->next; 	
	}
	
	A->length -= deleteOccurencesL(A->first, V);

}

// N time
int deleteOccurencesL(link A, Item V)
{
	if(!A) return 0;

	link B = A->next;	
	if(B && B->item == V)
	{
		int count = 0;
		while(B && B->item == V)
		{
			count++;
			A->next = B->next;

			// Frees the memory.
			destroyLink(B);
			B = A->next;
		}
		// Increments the count of deleted items.
		return deleteOccurencesL(A->next, V) + count;
	}

	// Does nothing
	return deleteOccurencesL(B, V);
}

// Pretty sure this is N 
Item grabItem(link A, int pos)
{
	if (pos == 0) return A->item;
	return grabItem(A->next, pos-1);
}

// n*m time. 
list sublist(list A, list pos_list) 
{
	if(!A || A->length == 0) return 0;
	if(!pos_list || pos_list == 0) return 0; 

	link node = pos_list->first;



	//Create a new list and the first node.
	list returned_list = newList();

	link ins = newLink(grabItem(A->first, node->item), 0);
	
	returned_list->first = ins;
	returned_list->length++; 	

	node = node->next;

	// Iterates through each of the nodes.
	while (node)
	{
        // If there is an out of bounds issue, destroys itself, returns null.
		if ( node->item < 0 || node->item >= A->length )
		{
			destroyList(returned_list);	
			return 0; 
		}

		ins->next = newLink(grabItem(A->first, node->item), 0);
		ins = ins->next;
		returned_list->length++;
		node = node->next;
	}	

		

	return returned_list;
}

// Pretty sure this is N time. It goes through multiple passes, sure,
// But it just scans through in linear time. 
void moveAllMaxAtEnd(list A) 
{
	if (!A || A->length == 0)
	{
		return; 
	}

	link end = A->first;
	Item max_value = A->first->item;

	// Does a run through the list, to find max. 
	while (end)
	{
		if(max_value < end->item)
		{
			max_value = end->item; 
		}
		end = end->next;
	}

	// Creates two non-existent links, for creating a sub-list.
	link bMove = 0, eMove = 0;

	// Sets the "end" to the first variable, for traversal.
	end = A->first;
	
	// Parses through the beginning of the list, culls out max_values, and inserts them into the other list.
	while(A->first && A->first->item == max_value)
	{
		// If there is no other list, start making it. 
		if(!bMove) 
		{
			eMove = bMove = A->first;
		
		}
		else
		// If there is, append onto it.
		{
			eMove->next = A->first;
			eMove = eMove->next;
		}
		
		// Traverses the main list.
		end = A->first = A->first->next;
	}

	// If the list ends up being empty, set it to the sub-list and return. 
	if (!end)
	{
		A->first = bMove;
		return;
	}
	
	// Parses through the rest of the list, culls out max_values, inserts them into the other list.
	while(end->next)
	{
		// if the next item is a "max_value"
		if(end->next->item == max_value)
		{
			// If there is no other list, start making it from this node.
			if(!bMove)
			{
				eMove = bMove = end->next;
			}
			// Otherwise, append onto it.
			else
			{
				eMove->next = end->next;
				eMove = eMove->next;
			}
	
			// Cull it out of the main list.
			end->next = end->next->next;
		}
		
		// If there is a next link, set end to it. 	
		if(end->next)
		end = end->next;
	}

	// Make the last node of the sub-list point to nothing. 
	eMove->next = 0;

	// Set the last node in the main list equal to the first node of the sub-list. 
	end->next = bMove;
	return;
}


