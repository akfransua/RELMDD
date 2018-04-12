# RELMDD
A C/C++ API\library  for manipulating relations
Library need to run this API 
       CUDD ---https://github.com/ivmai/cudd
       Libxml2    http://xmlsoft.org/



Using RelMDD
Initializing and Shutting Down RelMDD
To use RelMDD in your program, you must first call the function 
RelMDD_Init(); 
This will initialize the package and the CUDD library.
In order to reclaim memory use the function
RelMDD_Quit.

Refer to the test program and the readme file. 
You can contact me at akfransua@yahoo.com




README file for the test program of the RelMDD package


testRelMDD
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

testRelMDD is a test program for the RelMDD package.
Practically, it is a naive user interface to load, store and execute relation 
operations with ADDs from CUDD package
It is included to provide a sanity check for the installation of the
package 

It reads in two matrices and an xml file.
validate the content of the xml file 
then find the union, intersection, composition,converse, complement, and also identity, top and bottom
and returns their result.

MAKE testRelMDD: 
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

Run "make testRelMDD".
This should produce the testRelMDD executable file.

To Execution:
>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
./testRelMDD mat1.txt mat2.txt samplexmlfile.xml

