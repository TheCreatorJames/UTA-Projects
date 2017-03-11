/* 
 * File:   main.c
 * Author: alex
 *
Compile:
gcc -o perm main.c perm.c
run:
./perm
 */

#include <stdio.h>
#include <stdlib.h>

#include "perm.h"



void call_perm_rec_1(int N, int nr_vals){
    printf("\nCalled : perm_rec_1(%d, %d);\n", N, nr_vals);
    perm_rec_1(N, nr_vals); // you must implement this function.    
}

/*
 * Test generating permutations recursively and iteratively and returning an 
 * array with all the permutations.
 */
int main(int argc, char** argv) {        
    char str[10] = {'a','b','c','d','e'};
	printf("Original: %s\n", str);
    reverse_str(str,5);
    printf("Reversed: %s\n", str);
	
	printf("\n Gray code for N = 3: ");
	Gray_code(3);
		
	printf("\n\n Gray code for N = 4: ");
	Gray_code(4);

    call_perm_rec_1(3,2);  // you need to implement perm_rec_1
    call_perm_rec_1(5,2);
    call_perm_rec_1(4,3);

    call_perm_rec_1(2,4);
    call_perm_rec_1(4,2);  
      
    int N = 4;
    int nr_vals = 2;
     
            
    printf("\nIn Main, called : perm_rec_2(%d, %d);\n", N, nr_vals);       
    matrix_ptr res = perm_rec_2(N, nr_vals);  // you must implement this function.
    printf("\nIn Main, printing the res->data_arr table: \n");        
    print_2D(res);
    
    free_matrix_ptr(res); // you must implement this function. 
   
    printf("\nCalling student_tests: \n");
    student_tests(); // This will run your tests.
	
	
    return (EXIT_SUCCESS);
}



