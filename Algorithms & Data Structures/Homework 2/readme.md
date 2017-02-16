# [Homework](homework.html) - Homework 2

Submit to [Blackboard](http://elearn.uta.edu/) before the deadline. You will be able to revise your answers until the deadline with no penalty.

* * *

Topics: Lists (single-linked), stacks.  
100 points  

* * *

### Task 1 (85 pts.) - Single-linked list, extend interface.

Files list.h and list.cpp were extended with the functions shown below and were renamed: [list_hw.h](http://vlm1.uta.edu/~alex/courses/__s17_common/homework/hw2_code/list_hw.h) and [list_hw.c](http://vlm1.uta.edu/~alex/courses/__s17_common/homework/hw2_code/list_hw.c). Implement these 4 functions in list_hw.c. You should not need to modify the list_hw.h file.  
A client program is also provided: [instructor_client.c](http://vlm1.uta.edu/~alex/courses/__s17_common/homework/hw2_code/instructor_client.c).  
<font color="red">The document with test cases is here: [abc1234.pdf](http://vlm1.uta.edu/~alex/courses/__s17_common/homework/hw2_code/abc1234.pdf)</font>

1.  (20 points) **list sublist(list A, list pos_list)**. This function returns a list that contains the values that appear in list A at positions given in pos_list
    *   Requirement 1: the values should appear in the result in the order given by pos_list. For example if  
        A: 15 -> 100 -> 7 -> 5 -> 100 -> 7 -> 30 and  
        pos_list : 3 -> 0 -> 6 -> 4  
        The resulting list will be A[3] -> A[0] -> A[6] -> A[4] , which gives values: 5 -> 15 -> 30 -> 100.
    *   <font color="red">Requirement 2: the result should be a deep copy, so that any future changes in list A cannot possibly affect the result, and any future changes in the result cannot possibly affect list A. (List A should remain as it was after building the sublist.)</font>
    *   Requirement 3: DO not copy the nodes in an array and then build a list from an array. When found the node in A, make a new node with the same value and insert it in the result list.
    *   The list pos_list CAN have repetitions. E.g.:  
        list A: 10 -> 5  
        pos_list: 0->0->1->-0->1->1->0.  
        <font color="red">produces list: 10->10->5->10->5->5->10</font>  
        implicitly the size of A is unrelated to the size of pos_list.  

2.  (15 points) **void deleteOccurrences(list A, int V)**. This function should delete (and free) all links in list A that contain value V. For example if  
    A: 15 -> 100 -> 7 -> 5 -> 100 -> 7 -> 30 and we delete value 7, list A will become:  
    A: 15 -> 100 -> 5 -> 100 -> 30
3.  (10 points)**void insertAtPosition(list A, Item val, int P)**. This function should create a new node N with data val and insert N in list A so that, at the end of the function call, link N is at position P of list A.  
    If an out-of-bound position (such as negative or too large) is given, insert at beginning and end of list respectively.  
    Use the convention that the first element of the list is at position 0\.  
    Example 1:  
    A: 15 -> 100 -> 5 -> 100 -> 30 and we insert 12 at position 0 A will become:  
    A: 12 -> 15 -> 100 -> 5 -> 100 -> 30  
    Example 2:  
    A: 15 -> 100 -> 5 -> 100 -> 30 and we insert 99 at position -3 A will become:  
    A: 99 -> 15 -> 100 -> 5 -> 100 -> 30
4.  (25 points) **void moveAllMaxAtEnd(list A)**. This function should move ALL **the nodes** containing the maximum value in the list, to the end of the list.
    *   <font color="red">Requirement: You must move the original nodes (by updating the links), NOT delete a node and make a new copy at the end.</font>
    *   For example if  
        A: 15 -> <font color="red">100</font> -> 5 -> <font color="red">100</font> -> 30 and call this function, list A will become:  
        A: 15 -> 5 -> 30 <font color="red">-> 100 -> 100</font>

6.  You must keep the signature for these functions as shown here. We will use test code that will call your functions expecting this format. - NO CREDIT WILL BE GIVEN IF THE CODE DOES NOT COMPILE DUE TO CHANGE IN FUNCTION SIGNATURES OR SYNTAX ERRORS.  

7.  <font color="red">You cannot use arrays to make working with lists easier. E.g., do not copy the data in an array and then access the numbers from the array. The function "arrayToList"provided in instructors_client is using an array and that is fine. It is not bypassing the list manipulation needed for the homework, it is making it easier to create a hardcoded list. - Updated 2/9 @ 6:21pm</font>  

8.  (<font color="red"><s>15 points</s> 7 points</font>) You must thoroughly test your functions. Rename the [abc1234.pdf](hw2_code/abc1234.pdf) file and put your answers in it indicating if you function passes or fails that test.
    *   Sample test case file: [data_1.txt](hw2_code/data_1.txt). Your function "failed a test case" if it did not have code to deal with that case and as a result of that, it either crashed or it produced an unexpected effect, e.g.: it used a pointer that was not initialized (maybe your program will not crash, it will access data that was not allocated by the program), or it accessed the wrong data  

9.  (<font color="RED">**8 points** </font>)You should make it easy for the grader to also run these tests. You can do that by either:  
    - providing the data files (one file for each test), or  
    - by MODIFYING the client code and defining and calling a function or a set of functions in which you hardcode all your tests (similar to the tests from instr_client).
10.  Compile and run with the commands:

    <pre> gcc -o instr instructor_client.c list_hw.c
     <s>./instr < data_1.txt</s> - Updated 2/9 @4:03pm This program does not take user input. The data is hardcoded so you do not need input redirection.
     ./instr

     <font color="purple">Updated 2/9 @4:03pm The instr_client as given, does not take user input.
    But if you change the hardcoded data:
    	A = arrayToList(arr, 7);
    	//A = buildListOfInts() ;  // use this line and redirect input to run test cases from data files. See posted example data file.
    to user input data:
    	//A = arrayToList(arr, 7);
    	A = buildListOfInts() ;  // use this line and redirect input to run test cases from data files. See posted example data file.
    then you can run the program with :
    	./instr < data_1.txt</font> 
    </pre>

Below are shown two executions:  
- The first one is with the given files as they are (as soon as you run them you should get the same output).  
- The second is with the implemented solution. You should get the same output after you implement your solution.<font color="purple">**  
--------------------------------------  
BEFORE YOU IMPLEMENT YOUR FUNCTIONS, when you run ./instr <s>< data_1.txt</s> you should get:**</font>

<pre>LIST A:
 List length = 7
 List items:
item 0: 15
item 1: 100
item 2: 7
item 3: 5
item 4: 100
item 5: 7
item 6: 30

--------------------------------------
Testing sublist (for lists of ints).

 List length = 7
 List items:
item 0: 15
item 1: 100
item 2: 7
item 3: 5
item 4: 100
item 5: 7
item 6: 30

List with positions pos_list:
 List length = 4
 List items:
item 0: 3
item 1: 0
item 2: 6
item 3: 4

LIST new list:
The list pointer is NULL)

The list pointer is NULL)

--------------------------------------
Test deleteOccurrences (for lists of ints).

LIST A:
 List length = 7
 List items:
15    100    7    5    100    7    30    
After calling deleteOccurrences(A,7) (currently not implemented).
After you implement this function, list A (printed below) should not have any values of 7.

 List length = 7
 List items:
15    100    7    5    100    7    30    

----------  end test function -------------------

--------------------------------------
Test insertAtPosition (for lists of ints).

LIST A:
 List length = 7
 List items:
15    100    7    5    100    7    30    
After calling insertAtPosition(A, 12, 0) (currently not implemented).
After you implement this function, list A (printed below) should have 12 at the beginning.

 List length = 7
 List items:
15    100    7    5    100    7    30    

----------  end test function -------------------

 <font color="red">Updated 2/9 @ 6:10pm
--------------------------------------
Test moveAllMaxAtEnd (for lists of ints).

LIST A:
 List length = 7
List items:    15           100          7            5            100          7            30
List pointers: 0x600010400 0x600010420 0x600010440 0x600010460 0x600010480 0x6000104a0 0x6000104c0
After calling moveAllMaxAtEnd (currently not implemented).
After you implement this function, list A (printed below) should have the largest value(s) at the end.

 List length = 7
List items:    15           100          7            5            100          7            30
List pointers: 0x600010400 0x600010420 0x600010440 0x600010460 0x600010480 0x6000104a0 0x6000104c0

----------  end test function -------------------</font> 
 <s>--------------------------------------
Test moveAllMaxAtEnd (for lists of ints).

LIST A:
 List length = 7
 List items:
15    100    7    5    100    7    30    
After calling moveAllMaxAtEnd (currently not implemented).
After you implement this function, list A (printed below) should have the largest value(s) at the end.

 List length = 7
 List items:
15    100    7    5    100    7    30    

----------  end test function -------------------</s> 
</pre>

<font color="purple">**  
--------------------------------------  
AFTER YOU IMPLEMENT YOUR FUNCTIONS, when you run ./instr you should get:**</font>

<pre>LIST A:
 List length = 7
 List items:
item 0: 15
item 1: 100
item 2: 7
item 3: 5
item 4: 100
item 5: 7
item 6: 30

--------------------------------------
Testing sublist (for lists of ints).

 List length = 7
 List items:
item 0: 15
item 1: 100
item 2: 7
item 3: 5
item 4: 100
item 5: 7
item 6: 30

List with positions pos_list:
 List length = 4
 List items:
item 0: 3
item 1: 0
item 2: 6
item 3: 4

LIST new list:
 List length = 4
 List items:
item 0: 5
item 1: 15
item 2: 30
item 3: 100

--------------------------------------
Test deleteOccurrences (for lists of ints).

LIST A:
 List length = 7
 List items:
15    100    7    5    100    7    30    
After calling deleteOccurrences(A,7) (currently not implemented).
After you implement this function, list A (printed below) should not have any values of 7.

 List length = 5
 List items:
15    100    5    100    30    

----------  end test function -------------------

--------------------------------------
Test insertAtPosition (for lists of ints).

LIST A:
 List length = 5
 List items:
15    100    5    100    30    
After calling insertAtPosition(A, 12, 0) (currently not implemented).
After you implement this function, list A (printed below) should have 12 at the beginning.

 List length = 6
 List items:
12    15    100    5    100    30    

----------  end test function -------------------

 <font color="red">Updated 2/9 @ 6:10pm
--------------------------------------
Test moveAllMaxAtEnd (for lists of ints).

LIST A:
 List length = 6
List items:    12           15           100          5            100          30
List pointers: 0x6000104a0 0x600010400 0x600010420 0x600010460 0x600010480 0x6000104c0
After calling moveAllMaxAtEnd (currently not implemented).
After you implement this function, list A (printed below) should have the largest value(s) at the end.

 List length = 6
List items:    12           15           5            30           100          100
List pointers: 0x6000104a0 0x600010400 0x600010460 0x6000104c0 0x600010420 0x600010480

----------  end test function -------------------</font> 
 <s>--------------------------------------
Test moveAllMaxAtEnd (for lists of ints).

LIST A:
 List length = 6
 List items:
12    15    100    5    100    30    
After calling moveAllMaxAtEnd (currently not implemented).
After you implement this function, list A (printed below) should have the largest value(s) at the end.

 List length = 6
 List items:
12    15    5    30    100    100

----------  end test function -------------------</s> 
</pre>

(It is not sufficient for your function to only behave correctly in the above example, it should work with any example, and lists of any size.)

* * *

### Task 2 (15 pts.) - Time complexity

For each function discussed in Task 1, specify the time complexity (in Theta notation) that can be attained for that function, using lists **as defined** at list_hw.h and list_hw.c.  
<font color="red">You need to give the complexity of the functions that you wrote, as you wrote them. For example if your implementation is very inefficient you should give the run time of your implementation (not that of some other efficient implementation).</font>  
<font color="purple">Also if your function calls a helper function that you wrote or if it calls some other already provided list function, you should take in consideration the time complexity of that function as well in your answer. - Updated 2/9 @ 6:45pm</font>

Remember that the time complexity describes the worst-case.  
<font color="RED">Also note that these points will not be distributed equally per function, but given as a total. Therefore if any of the above functions requires a more complicated time analysis it will get more points than the others.</font>  
Put your answer in the yourNetid.pdf file. You can also directly write this answer in the source code as a comment at the top of the functions.

* * *

### How to submit

<font color="red">15 points penalty for wrong filenames, name of folder, type of archive (must be ZIP).</font>  
The assignment should be submitted via [Blackboard](http://elearn.uta.edu/). Submit a ZIPPED directory called <tt>hw2.zip</tt>. No other forms of compression accepted, contact the instructor or TA if you do not know how to produce .zip files. The zipped directory should contain the following documents:

*   **yourNetid.pdf**. A PDF file containing your solutions for each task. **Include your name and student ID at the top of this document. It may be printed and graded on paper.**  
    <font color="RED">******** yourNetid.xxx should contain ALL THE COMMANDS you used to COMPILE AND RUN your code. If missing: 10 points penalty per assignment and the assignment will not be graded until you give that information to the TA.</font>
*   **list_hw.c** Your modified list.c.
*   **instructor_client.c** (This may have been modified to run your test cases).

Note that you should not modify file: **list_hw.h**.Programs must have the .h or .c extensions and must run on omega.uta.edu.  

**PARTIAL CREDIT :** Partial credit will NOT be given for programs that have compilation or runtime errors. You are responsible for debugging your own code.  

**IMPORTANT:** Pay close attention to all specifications on this page, including file names and submission format. Even in cases where your answers are correct, points will be taken off liberally for non-compliance with the instructions given on this page (such as wrong file names, wrong compression format for the submitted code, and so on). The reason is that non-compliance with the instructions makes the grading process significantly (and unnecessarily) more time consuming. Contact the instructor or TA if you have any questions.
