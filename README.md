# Sparse.c
Main program for this program.

I used a readInt function I had created for a previous project, but then also used this function as a basis for a new function, readDouble, alongside a helper function, exponent. Double does the same thing as readInt, but with some differences: it can detect a `.` character, initating a new kind of number read which, rather than multiplying the currently read number by 10 and adding the single-digit read result, will multiply the read result by 10^n before adding it to the already read number, where n is the number of places past the decimal point that the function is reading. My exponent function is a helper for this because it simply computes a number to the power of n, in this case 10^n.

Once I had gotten readDouble to work properly I had a few more issues. I had to add functionality for negative values to readDouble, and then had to account for various formatting edge-cases in the input files, such as having more entries per matrix than the pre-stated number of nonzero entries. Then, there were some simple bugs in my diff and sum functions to fix, and i was done.

# Matrix.c

Function definitions for a Matrix ADT specialized for sparse matrices.

My implementation of changeEntry *should* be accurate to the theta(A) optimization limit. It only iterates in one loop, iterating through the elements in a linked list, meaning it should only scale up in complexity with the number of elements in this linked list. Since the list only contains non-zero elements, the program scales only with the number of non-zero elements in the list.

My implementation of copy() is made more efficient by forgoing usage of changeEntry to instead just iterate through each list linearly. This means only iterating through each list once rather than calling changeEntry to iterate through each list of B while already iterating through A.

My implementation of sum, diff, and product all use helper functions which operate row-by-row to form the full matrix. I implemented them more-or-less exactly how the assignment doc recommended. These functions ended up rather bloated to account for various edge-cases, such as adding a matrix to itself.

# Matrix.h
Function declarations for a Matrix ADT specialized for sparse matrices.

# MatrixTest.c
Function tests for the Matrix ADT

# List.c
Function definitions for List ADT.

Mostly copied from previous assignments, but edited to use `void*` for node data instead of `ints`, allowing for a list of Matrix `Entries` in Matrix.c

# List.h
Function declarations for a List ADT.

# ListTest.c
Currently empty. List would be hard to thoroughly test without a different ADT to test it with. I know that my list functions are sturdy enough to change some things without them breaking, so I instead just tested them in MatrixTest.c and found that everything pretty much worked as intended first try.

# README.md
This file, index of other files

# Makefile
Copied from an example. Tweaked slightly.
