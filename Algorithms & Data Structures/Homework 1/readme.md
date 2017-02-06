
# [Homework](http://vlm1.uta.edu/~alex/courses/__s17_common/homework/homework.html) - Homework 1

Points: 100  
Topics: text indexing: Indirect sorting with insertion sort and binary search in text.  
Note that the program requires processing of strings and it can be time consuming. Start early. No extension will be given.  

* * *

## Task 1 (15 points)

The selection sort algorithm given in class is not stable. The problem is when swapping (see what happens to the two 4s in the example below):

<pre><i>4</i>, <u>4</u>, 5, 6, 7, 0, 8, 9
0, <u>4</u>, 5, 6, 7, <i>4</i>, 8, 9
</pre>

You can have a stable version of Selection sort by modifying the algorithm to not swap, but slide the needed elements to the right to make room for the minimum to go in its place (note that _4_ did not jump between 9 and 2 any more, instead the whole sequence 4,4,5,6,7 was slid):

<pre><i>4</i>, <u>4</u>, 5, 6, 7, 0, 8, 9
0, <i>4</i>, <u>4</u>, 5, 6, 7, 8, 9
</pre>

Write a program called <it>stable_selection_sort.c</it> that implements the following:

1.  Read the data and store each pair (number and string) as a struct. <font color="red">**Store the whole data in an array of structs (NOT a linked list). -Updated 2/1/2017**</font>
2.  Print the data. Write a method to do this and call it: after your read the data (to show that you read it correctly), after the stable and after the unstable sort.
3.  The original (unstable) version of selection sort.
4.  The modified, stable version of selection sort.
5.  Call both versions of selection sort in main and show the data before and after running each one of them.

Derive the time complexity of your stable selection sort. Is this time complexity still quadratic (i.e., disregarding constants, the dominant term is quadratic) or is it larger than that? Show how you computed the time complexity by clearly showing what loop generated each term (either in a pdf file or as comments in the code).  

You can assume that:

1.  The first line in the file will contain the number of records to be read from the and that the file will have data for at least as many records as given at the top.
2.  You will never need to read more than 2000 records.
3.  The word associated with the number will not have more than 100 characters.

Requirements:

1.  Create a struct to hold the records (number and string). Do NOT store then as two arrays (one of ints and one of strings) - 60% penalty otherwise. (For a review on creating structs in C you can see [TutorialsPoint](https://www.tutorialspoint.com/cprogramming/c_structures.htm).)
2.  You can only read the data once. (You can not explicitly read from a file and open that file and read from it twice.) You must read teh input from teh user and get it from a file by using input redirection as shown in the sample run.

Hints:

1.  You can only read the data once, but you must make a copy so that you can run both the unstable and the stable versions of selection sort.
2.  Note that however you represent the strings (with char* or char[]) you MUST use strcpy when you copy it. In particular you cannot copy directly the structs (e.g. var1 = var2). You have to use strcpy on the strings from within those structs.

Data files: [numbers0.txt](http://vlm1.uta.edu/~alex/courses/__s17_common/homework/hw1_data/numbers0.txt), [numbers1.txt](http://vlm1.uta.edu/~alex/courses/__s17_common/homework/hw1_data/numbers1.txt).  

<pre>---------------  COMPILE THE CODE:
gcc -o ss stable_selection_sort.c

---------------  RUN THE CODE with data from file numbers1.txt:
./ss < numbers1.txt
Enter the number of records:
Enter the data pairs(number string):
Loaded data:
4, A
4, B
5, X
6, Dave
7, random
0, sun
8, rain
9, earth

Data sorted with UNSTABLE selection sort:
0, sun
4, B
4, A
5, X
6, Dave
7, random
8, rain
9, earth

A copy of the original data, sorted with STABLE selection sort:
0, sun
4, A
4, B
5, X
6, Dave
7, random
8, rain
9, earth

---------------  RUN THE CODE with data from file numbers0.txt:
$ ./ss < numbers0.txt
Enter the number of records:
Enter the data pairs(number string):
Loaded data:
4, A
4, B
2, sun

Data sorted with UNSTABLE selection sort:
2, sun
4, B
4, A

A copy of the original data, sorted with STABLE selection sort:
2, sun
4, A
4, B

</pre>

## Task 2 (85 points)

Data files: [data0.txt](http://vlm1.uta.edu/~alex/courses/__s17_common/homework/hw1_data/data0.txt), [data1.txt](http://vlm1.uta.edu/~alex/courses/__s17_common/homework/hw1_data/data1.txt), [data2.txt](http://vlm1.uta.edu/~alex/courses/__s17_common/homework/hw1_data/data2.txt).  
Write a program named text_indexing.c that does the following:

1.  DO NOT read the filename as part of the program input or as command line arguments (do not write any core to read from a file). The program must read the data as user input and be able to use data from a file by redirecting the input as shown in the sample run.
2.  Reads text and stores it as one string called _text_. You can read from a file or from the user. (In my implementation, I read only one paragraph (up to new line) from the user. With this same code, I am able to read data from a file by using input redirection (executable < filename) when I run the program. See sample runs below). You cannot open (and read) the file more than once.
3.  You can assume that the text will not have more than 10000 characters.
4.  We want to be able to perform fast searches in this text. We will index it by keeping the indexes where each word starts in alphabetical order.
5.  Find, store (in an array) and print the index for each word and the word itself. They should show in the order in which they appear in the text. (See sample run.)
6.  Sort using INSERTION SORT the indexes and print again the indexes and the words. They should show in sorted order now.(See sample run.)
7.  The sorting must be case insensitive for the first letter. That is, if the first letter of any of the two words you are comparing is upper case, you must use the lower case value in your comparison. (Notice in the sample run with data1.txt that 'Chemical' is placed after 'application'.). You may want to write your own string compare method.
8.  When searching for words (with binary search) count the number of words you look at until you get the answer (found or not found). Display that number in the run.
9.  When searching (not when sorting) you should ignore the punctuation at the end of words. See the run with data0.txt:

    <pre>	./textidx.exe < data0.txt
        Enter the text:

        ...
        ...
        processes - found (3 iterations)
        processes. - not found (4 iterations)
        ...
        </pre>

10.  REQUIREMENT: you must NOT make a copy of the data. That is, you must keep it still as one long string and process the words in it. DO NOT create an array of individual words. (-35 points if you make an array. If you do make the array, it should NOT be in sorted order. The goal here is to use indirect sorting and if you create a sorted array you would not be doing an indirect sort. You will sort the indexes into this array. See the indirect sorting with selection sort in the slides.)
11.  5 points will be given for good practices: variable names, comments, descriptions. These will only be earned if the program is correct 75% or more. Non-working programs or programs that implement too few of the requirements will not get them.
12.  C function that I used:
    1.  scanf - to read the whole text:

        <pre>		<tt>scanf("%[^\n]s",text); // the [^\n] part makes it stop reading when it reaches the first new line, \n.</tt>
                </pre>

    2.  <tt>sscanf</tt> - to read individual words from the long string (holding the whole text). Example:

        <pre>char text[10000];
        char s1[101]; // char array that can hold 101 chars. It can be used to store a string of 100 chars (the 101-th char will be used as the terminator for strings: '\0'
        printf("Enter text: ");
        scanf("%[^\n]s", text); // assume the user enters:   cats and dogs.
        sscanf(text, "%s", s1);		// s1 will be "cats"
        sscanf(&text[0], "%s", s1);		// another way to read the first word: s1 will be "cats"
        sscanf(&text[5], "%s", s1);		// s1 will be "and"
        sscanf(&text[9], "%s", s1);		// s1 will be "dogs."
        </pre>

        You can run these files: [string_utils.c](http://vlm1.uta.edu/~alex/courses/__s17_common/homework/hw1_data/string_utils.c), [str_data1.txt](http://vlm1.uta.edu/~alex/courses/__s17_common/homework/hw1_data/str_data1.txt), [str_data2.txt](http://vlm1.uta.edu/~alex/courses/__s17_common/homework/hw1_data/str_data2.txt). Put all 3 files in the same folder and:

        <pre>------------  COMPILE:
        gcc -o su string_utils.c

        ------------  RUN BY REDIRECTING DATA from file str_data1.txt. EXECUTE:
        ./su < str_data1.txt

        ------  IT SHOUDL PRODUCE THE OUTPUT:
        Enter text:
        text is: cats and dogs.
        s1 is: cats
        s1 is: cats
        s1 is: and
        s1 is: dogs.
        s1 is: do

        ------------  RUN BY REDIRECTING DATA from file str_data2.txt. EXECUTE:
        ./su < str_data2.txt

        ------  IT SHOUDL PRODUCE THE OUTPUT:
        Enter text:
        text is: Second test file.
        s1 is: Second
        s1 is: Second
        s1 is: d
        s1 is: st
        s1 is: st
        </pre>

    3.  Note that <tt>text</tt> is a char array (works as a pointer), <tt>text[i]</tt> is a char, and <tt>&text[i]</tt> is a pointer.
13.  Words are 'cleaned-up' by: making them all lower case and looking at the last symbol. If it was not a letter, that symbol was cut from the word (and so . and , at the end of some words can be removed). However this clean-up is also applied to the words that I am searching for.

<font color="RED">INCLUDE THE COMPILATION AND EXECUTION INSTRUCTIONS. Failing to do so will be a 10 points penalty.</font> For example, for my implementation, those instructions would be:

<pre>compile:
gcc -o textidx text_indexing.c

run with data from file data0.txt:
./textidx.exe < data0.txt

run with data from file data1.txt:
./textidx.exe < data1.txt

run with data from file data2.txt:
./textidx.exe < data2.txt

run with user input:
./textidx.exe
</pre>

### Sample runs

<pre>compile:
gcc -o textidx text_indexing.c

**------------- Sample run 1 with input redirected from file data0.txt (not typed by the user):**
./textidx.exe < data0.txt
Enter the text:

Words and indexes (the printed words are 'cleaned'):
 i   |index[i]|  word
-----|--------|------------------
   0 |      0 |  chemical
   1 |      9 |  engineering
   2 |     21 |  encompasses
   3 |     33 |  the
   4 |     37 |  development
   5 |     50 |  application
   6 |     63 |  and
   7 |     67 |  operation
   8 |     77 |  of
   9 |     80 |  processes

The sorted data (the printed words are 'cleaned'):
 i   |index[i]|  word
-----|--------|------------------
   0 |     63 |  and
   1 |     50 |  application
   2 |      0 |  chemical
   3 |     37 |  development
   4 |     21 |  encompasses
   5 |      9 |  engineering
   6 |     77 |  of
   7 |     67 |  operation
   8 |     80 |  processes
   9 |     33 |  the

Binary search. ---- Enter words to search for. Stop with -1.
(original: that)
cleaned: that - not found (4 iterations)
(original: at)
cleaned: at - not found (3 iterations)
(original: the)
cleaned: the - found (4 iterations)
(original: application)
cleaned: application - found (2 iterations)
(original: processes)
cleaned: processes - found (3 iterations)
(original: processes.)
cleaned: processes - found (3 iterations)
(original: and)
cleaned: and - found (3 iterations)

 <font color="RED">The original text is still the same: Chemical engineering encompasses the development, application, and operation of processes.</font>

**------------- Sample run 2 (with user typing the text shown in blue):**

./textidx.exe
Enter the text:
<font color="blue">THIS, is another, EXAMple.</font>

Words and indexes (the printed words are 'cleaned'):
 i   |index[i]|  word
-----|--------|------------------
   0 |      0 |  this
   1 |      6 |  is
   2 |      9 |  another
   3 |     18 |  example

The sorted data (the printed words are 'cleaned'):
 i   |index[i]|  word
-----|--------|------------------
   0 |      9 |  another
   1 |     18 |  example
   2 |      6 |  is
   3 |      0 |  this

Binary search. ---- Enter words to search for. Stop with -1.
THis
(original: THis)
cleaned: this - found (3 iterations)
THIS
(original: THIS)
cleaned: this - found (3 iterations)
this,
(original: this,)
cleaned: this - found (3 iterations)
is,
(original: is,)
cleaned: is - found (2 iterations)
is,,
(original: is,,)
cleaned: is, - not found (3 iterations)
ANTOTHER
(original: ANTOTHER)
cleaned: antother - not found (2 iterations)
ANOTHER
(original: ANOTHER)
cleaned: another - found (2 iterations)
EXAMPLE
(original: EXAMPLE)
cleaned: example - found (1 iterations)
-1

 <font color="RED">The original text is still the same: THIS, is another, EXAMple.</font>

**------------- Sample run 3 (with data entered from the user shown in blue):**
Enter the text:
<font color="blue">TT tabc</font>

Words and indexes (the printed words are 'cleaned'):
 i   |index[i]|  word
-----|--------|------------------
   0 |      0 |  tt
   1 |      3 |  tabc

The sorted data (the printed words are 'cleaned'):
 i   |index[i]|  word
-----|--------|------------------
   0 |      3 |  tabc
   1 |      0 |  tt

Binary search. ---- Enter words to search for. Stop with -1.
-1

 The original text is still the same: TT tabc

</pre>

### How to submit

The assignment should be submitted via [Blackboard](http://elearn.uta.edu/).  
<font color="red">**Put stable_section_sort.c, text_indexing.c, and the readme file (pdf of txt formats) in a FOLDER called hw1, zip the hw1 FOLDER (it will produce a file called hw1.zip). Submit the hw1.zip file.  
Note that the TA will use the posted files to test your code and so you should not modify those files in any. If you need to to any processing to them, your program must do it.**</font>

As stated on the course syllabus, programs must be in C, and must run on omega.uta.edu.  
Include the compilation instructions and any other notes in the readme file.

**IMPORTANT:** Pay close attention to all specifications on this page, including file names and submission format. Even in cases where your answers are correct, points will be taken off liberally for non-compliance with the instructions given on this page (such as wrong file names, wrong compression format for the submitted code, and so on). The reason is that non-compliance with the instructions makes the grading process significantly (and unnecessarily) more time consuming. Contact the instructor or TA if you have any questions.
