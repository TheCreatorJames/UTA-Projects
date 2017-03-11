#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "perm.h"


// Prints the 2D data.
void print_2D(matrix_ptr mat){
    int i, j;
    if (mat == NULL || mat->data_arr == NULL) {
        return;
    }
    
    printf("\nrows = %d, columns = %d\n", mat->rows, mat->cols);
    int ct = 0;
    for(i = 0; i < mat->rows; i++){
        for(j = 0; j < mat->cols; j++, ct++) {
            printf("%5d", mat->data_arr[i][j] );            
        }
        printf("\n");
    }    
}

// Prints the 2D data. Ignores -1: prints empty space instead of -1.
// This will make it show the constructed part of the Gray code.
void print_Gray_2D(matrix_ptr mat, int nr_rows){
    int i, j;
    if (mat == NULL || mat->data_arr == NULL) {
        return;
    }
    
    printf("\nrows = %d\n", nr_rows);
    int ct = 0;
    for(i = 0; i < nr_rows; i++){
        for(j = 0; j < mat->cols; j++, ct++) {
			if (mat->data_arr[i][j] != -1)
				printf("%1d", mat->data_arr[i][j] );            
			else
				printf("%1s", " " );            
        }
        printf("\n");
    }    
}





// Implement the functions below. 
// You can add any helper function you need.


// Use this method to create and run any tests you want. It is called from main so 
// you do can still test your code, without modifying main.
void student_tests()
{
	// Your code here.

    perm_rec_1(5, 1);
    
    perm_rec_1(0, 3);

    Gray_code(1);
}


/* Recursive function that reverses the string str. 
 * N is the length of the string. 
 */
// Time Complexity: O(N),
// It iterates linearly through the string.
void reverse_str(char * str, int N)
{
    if(!str) return;
    if(N == 1 || N == 0) return;

    // your code here
    char x = str[N-1];
    str[N-1] = str[0];
    str[0] = x;
    reverse_str(str+1, N-2);
}


// This is used to create the matrix_ptr.
// Time complexity: 
// O(N*M), where N = Number of Rows, and M = number of Cols 
matrix_ptr create_matrix_ptr(int rows, int cols, int mode)
{
    int i, x, y;
    // Creates the matrix pointer.
    matrix_ptr m = (matrix_ptr)malloc(sizeof(struct matrix_2D_struct));
    m->data_arr = (int**)malloc(sizeof(int*)*rows);
    m->rows = rows;
    m->cols = cols;
    for(i = 0; i < rows; i++)
    {
        if(mode)
        m->data_arr[i] = calloc(cols, sizeof(int));
        else
        m->data_arr[i] = malloc(sizeof(int)*cols);        
    }


    // This technically isn't necessary, but was requested in the prompt.
    if(!mode)
    for(x = 0; x < rows; x++)
    {
        for(y = 0; y < cols; y++)
        {
            m->data_arr[x][y] = -1;
        }
    }
    return m;
}

// O(N), where N = size, unless the system architecture has a much more efficient method of copying blocks of memory.
// However, we should asusme that it is primitive.
void copyRow(int* dest, int* source, int size)
{
    /*
    for(int i = 0; i < size; i++)
    {
        dest[i] = source[i];
    }*/

    memcpy(dest, source, sizeof(int)*size);
}

// Time complexity: O(2^N * M), I'm almost certain. 
// It looks like log_2(2^N)*N, but once you unwrap that, it becomes N^2, but then you notice that it calls the 
// 'print_Gray_2D' method, which scales at a rate 2^N * M. 
void Gray_code(int N)
{
    int i,j;


	/*
        //The way that I'd actually implement this is by doing something like this:

        for(int i = 0; i < (1 << N); i++)
        {
            printf("%i\n", i ^ (i>>1));
        }

        // Totally more efficient :|
    */
    

    if(N <= 0)
    {
        return;
    }
   

    int rows = 1 << N; 
    int negLoc = N - 2;
    matrix_ptr m = create_matrix_ptr(rows, N, 0);


    m->data_arr[0][N-1] = 0;
    m->data_arr[1][N-1] = 1;

    if(N == 1)
    {
        print_Gray_2D(m, 2);
        free_matrix_ptr(m);
        return;
    }


    for(i = 2; i < rows; i <<= 1)
    {
        // Copies the rows.
        for(j = 0; j < i; j++)
        {
            copyRow(m->data_arr[2*i-j-1], m->data_arr[j], N);
        }

        //Does the appending steps.
        for(j = 0; j < i; j++)
        {
            // Appends zeroes to initial set.
            m->data_arr[j][negLoc] = 0;
            // Appends ones to copied set.
            m->data_arr[j+i][negLoc] = 1;
        }

        print_Gray_2D(m, i << 1);

        // Shifts the location of where to append the negatives.
        negLoc--;
    }


    free_matrix_ptr(m);

}

//  Time complexity: O(N), where N is the number of rows.  
void free_matrix_ptr(matrix_ptr my_matrix)
{
    int i;
    if(!my_matrix) return;
    
    // frees all the cols.
    for(i = 0; i < my_matrix->rows; i++)
    {
        free(my_matrix->data_arr[i]);
    }

    // frees all the rows.
    free(my_matrix->data_arr);
    
    // frees the memory allocated specifically for the matrix_ptr.
    free(my_matrix);
}



// This is my recursive solution. 
//  Time complexity: N^M, where M is the range.
//  As mentioned in another section, it is realistically more efficient than this, but the growth
//  is in this format. 
void perm_rec_1_r(int N, int range, int* vals)
{
    if(N < 1) return;

    int i;
    if(!vals)
    {
        vals = (int*)calloc(N, sizeof(int));
    }
    else
    for(i = N-1; i >= 0; i--)
    {
        if(vals[i] + 1 == range)
        {
            // This next line is why it terminates. It would try to add 1 to the next one,
            // but it can't because it is out of bounds.
            if(i == 0) 
            {
                free(vals);
                return; 
            }
            vals[i] = 0; 
        } else
        {
            vals[i]++;
            break;
        }
    }


    // This was in the out file.
    printf("  ");

    // print them out, I guess.
    for(i = 0; i < N; i++)
    {
        printf("%i ", vals[i]);
    }
    printf("\n");

    perm_rec_1_r(N, range, vals);
}

//  Time complexity: N^M, where M is the range. It's actually slightly more efficient than that, 
//  but the growth is very similar. 
void perm_rec_1(int N, int range)
{
    if(N < 1) return;

    // Honestly, I wish the assignment prompt had been a bit more specific about how this was supposed to be implemented.
    // There are many ways this can be programmed. I will avoid the modulo and integer mapping methods though.
    // Hopefully I'll stumble upon the method you'd specifically like for us to use. 
    perm_rec_1_r(N, range, 0);
}


// This is my recursive solution. 
//  Time complexity: N^M, where M is the range.
//  As mentioned in another section, it is realistically more efficient than this, but the growth
//  is in this format. 
matrix_ptr perm_rec_2_r(int N, int range, matrix_ptr vals, int pos, int* temp)
{
    int i;

    if(N < 1) 
    {
        free_matrix_ptr(vals);
        free(temp);
        return 0;
    }

    if(!temp)
    {
        temp = calloc(N, sizeof(int));
    }

    if(!vals)
    {
        vals = create_matrix_ptr((int)pow(N,range), N, 1);
    }
    else
    {
        // I could make this code more efficient (both memory and speed wise), by using a different method of termination,
        // but the prompt requested we terminate by acknowledging that we can't continue creating permutations, since it'd go out of bounds.
        copyRow(temp, vals->data_arr[pos-1], N);
        
        for(i = N-1; i >= 0; i--)
        {
            if(temp[i] + 1 == range)
            {
                // This next line is why it terminates. It would try to add 1 to the next one,
                // but it can't because it is out of bounds.
                if(i == 0) 
                {
                    free(temp);
                    return vals; 
                }
                temp[i] = 0; 
            } else
            {
                temp[i]++;
                break;
            }
        }
    }

    copyRow(vals->data_arr[pos], temp, N);
    return perm_rec_2_r(N, range, vals, pos+1, temp);
}



//  Time complexity: N^M, where M is the range.
//  As mentioned in another section, it is realistically more efficient than this, but the growth
//  is in this format. 
matrix_ptr perm_rec_2(int N, int range) 
{   
    if(N < 1) return 0;

    matrix_ptr res = perm_rec_2_r(N, range, 0, 0, 0);
    return res;
}
