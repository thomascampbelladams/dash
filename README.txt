Thomas Campbell-Adams
12/9/2011

To compile:
* dash: simply type make
* buddySystem: The test functions will need to be uncommented, as well as the main function. After that, type make buddySystem
* buddy-test: make buddy-test
* TestList: make TestList. The executable will be named tl

Files Included and Purpose:
command.c     -- Contains all the functions needed to parse commands sent to the shell
command.h     |
common.h      -- Contains common types and includes
error.c       -- Contains functions used to output fork errors
Job.c         -- Contains functions related to storing background process information in an object
Job.h         |
keyBindings.c -- Contains functions used to bind keys in order to have the up and down arrow keys work to browse the history
keyBindings.h |
List.c        -- Contains functions used to create and use a doubly linked list
List.h        |
Makefile      -- Makefile for the project
Node.c        -- Contains functions related to storing links and data of a node in the list
Node.h        |
ourhdr.h      -- Contains common includes for shell projects
shell1.c      -- Contains the main function of the program
shellUtil.c   -- Contains all the functionality related to internal commands used in the shell
shellUtil.h   |
signals.c     -- Contains all the functions related to subscribing and unsubscribing to events, and sending events to other processes
signals.h     |
loop.c        -- Test program for the dash
buddySystem.c -- Functions related to the buddy system for allocating memory.
buddySystem.h |
buddy-test.c  -- Program to test the buddy system implementation
docs          -- doxygen documentation for all files in this assignment.

Design:
All related functions are grouped into seperate files as described above.
Most functions use verbose names that describe what the function's purpose is.

Testing:
All tests and their intended results are located in TESTS.txt

Features:
* Ability to run tasks, run background tasks, and keep track of background processes
* Ability to set a custom prompt
* Ability to handle EOFs
* Ability to change directories
* Ability to manage background and foreground jobs
* The dash will make use of it's own memory management when using it's own internal functions.

Bugs:
* Job statuses don't update until the user enters an empty command

Parts of the assignment attempted:
* The buddy system was pretty much all I had time to implement for this project. I did not get a chance to fully
implement the piping in the shell.

Personal Observations:
After a stressful day with the last project and having to rewrite the program so it worked as it did before,
I started taking SVN a lot more seriously after that. I used it extensively on this project so I couldn't
lose track of my changes.

Also, Tortoise SVN is such a nice tool for this. I'm surprised Mac/Linux don't have anything as nice as this tool
for SVN.