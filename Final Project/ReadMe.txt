Maze Solver - Final Project
===========================

PROJECT OVERVIEW
----------------
This is a simple maze-solving program written in C.

The program reads a maze from a text file and solves it in two ways:

    - BFS uses a queue and finds a shortest path.
    - DFS uses recursion and finds a valid path, but it may not be shortest.

The program is automatic. It is not a game controlled with arrow keys.


FILES
-----
    maze_solver.c
        The C source code.

    ReadMe.txt
        Instructions for viewing, compiling, and running the program.

    mazes/maze_small.txt
        A small maze used to check the basic program.

    mazes/maze_compare.txt
        A maze designed to compare BFS and DFS on the same input.

    mazes/maze_dead_ends.txt
        A solvable maze with branches and dead ends.

    mazes/maze_unsolvable.txt
        A maze where no path exists from S to E.


MAZE SYMBOLS
------------
    #    wall

    .    open cell

    S    starting cell

    E    ending cell

In the program output:

    *    part of the final path

    +    explored cell that is not part of the final path


HOW THE TEST MAZES WERE DESIGNED
--------------------------------
Each maze has a different testing purpose.

    maze_small.txt
        This checks that the program can read a maze and find a route.
        It has one main route, so BFS and DFS may produce the same result.

    maze_compare.txt
        This maze has two valid routes with different lengths.
        Both algorithms solve the same maze.
        BFS should find the shorter route.
        DFS may find the longer route because it checks upward first.

    maze_dead_ends.txt
        This maze has branches that do not lead to the exit.
        It checks whether DFS can return from a dead end and try
        another direction. The maze still has a valid route to E.

    maze_unsolvable.txt
        Walls separate S from E.
        Both algorithms should finish and report that no path exists.


HOW TO OPEN THE SOURCE CODE
---------------------------
From Terminal, go to the Final Project folder:

    cd "Final Project"

To view the code inside Terminal:

    less maze_solver.c

Use the arrow keys to move through the file.
Press q to leave the viewer.

On macOS, you can also open the file with the default text editor:

    open maze_solver.c


HOW TO COMPILE
--------------
Open Terminal and go to the Final Project folder.

Compile the program with:

    clang -std=c11 -Wall -Wextra -pedantic maze_solver.c -o maze_solver

If Terminal returns to the command prompt without an error message,
the program compiled successfully.


HOW TO RUN
----------
Run the program by giving it one maze file:

    ./maze_solver mazes/maze_compare.txt

The program automatically runs both BFS and DFS.

Other examples:

    ./maze_solver mazes/maze_small.txt

    ./maze_solver mazes/maze_dead_ends.txt

    ./maze_solver mazes/maze_unsolvable.txt


WHAT THE PROGRAM PRINTS
-----------------------
For each maze, the program prints:

    - The original maze
    - The BFS result
    - The DFS result
    - Whether a path was found
    - The path length
    - The number of explored cells
    - A comparison of BFS and DFS


BFS
---
BFS uses a queue.

It checks cells level by level, starting with cells closest to S.
Because every move costs one step, BFS finds a shortest path.

BFS uses parentRow and parentColumn arrays to remember where each
new cell came from. After BFS reaches E, the program follows these
parent positions backward to rebuild the route from E to S.


DFS
---
DFS uses recursion.

It follows one route as deeply as possible. If it reaches a dead end,
it returns to the previous cell and tries another direction.

DFS does not use a parent array in this program. When DFS reaches E,
the successful recursive calls return and mark the final route.


IMPORTANT
---------
Opening maze_solver.c only shows the source code.

To run the maze solver, you must compile the source code and then run
the maze_solver executable.

Not every maze is guaranteed to have a solution. If no route connects
S and E, the program reports that no path was found.
