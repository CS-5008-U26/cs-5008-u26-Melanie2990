Mengling Zhang - Maze Solver Final Project
==========================================

WHAT THE PROGRAM DOES
---------------------
This is an automatic maze solver written in C. It reads a maze from a text
file and runs two search algorithms:

    BFS - uses a queue and finds a shortest path
    DFS - uses recursion and finds a path, but not always the shortest path

This is not a game controlled by the player. The program automatically finds
routes from S to E.

FILES
-----
    maze_solver.c                C source code
    ReadMe.txt                   Instructions
    mazes/maze_small.txt         Small basic maze
    mazes/maze_compare.txt       Maze used to compare BFS and DFS
    mazes/maze_dead_ends.txt     Maze with dead ends
    mazes/maze_unsolvable.txt    Maze with no path

OPENING THE C FILE
------------------
Move into the project folder:

    cd "Final Project"

To view the code in Terminal:

    less maze_solver.c

Use the arrow keys to move and press q to exit.

On macOS, this also opens the file in the default text editor:

    open maze_solver.c

Opening maze_solver.c only shows the code. It does not run the program.

COMPILING
---------
From inside the Final Project folder, run:

    clang -std=c11 -Wall -Wextra -pedantic maze_solver.c -o maze_solver

If no error appears, the program compiled successfully.

RUNNING
-------
Give the program one maze file:

    ./maze_solver mazes/maze_compare.txt

The program always runs both BFS and DFS. It prints:

    1. The original maze
    2. The BFS result
    3. The DFS result
    4. A simple comparison

Try all four maze files:

    ./maze_solver mazes/maze_small.txt
    ./maze_solver mazes/maze_compare.txt
    ./maze_solver mazes/maze_dead_ends.txt
    ./maze_solver mazes/maze_unsolvable.txt

MAZE SYMBOLS
------------
    #  wall
    .  open cell
    S  start
    E  end

The program can move up, right, down, and left. It cannot move diagonally or
through a wall.

OUTPUT SYMBOLS
--------------
    *  final path
    +  explored cell that is not on the final path

HOW BFS WORKS
-------------
BFS uses a queue and checks closer cells before farther cells. Since every
move costs one step, BFS finds a shortest path.

BFS uses parentRow and parentColumn arrays. They remember where each new cell
came from. After BFS reaches E, the program follows these records backward to
S and marks the route.

HOW DFS WORKS
-------------
DFS uses recursion. It follows one path deeply. If it reaches a dead end, it
returns and tries another direction.

DFS does not use parent arrays in this program. When DFS finds E, the route is
marked while the recursive calls return.

ABOUT THE TEST MAZES
--------------------
maze_small.txt checks the basic program.

maze_compare.txt runs BFS and DFS on the same maze. It compares their path
length and explored cells. It has a short route and a longer route.

maze_dead_ends.txt checks whether DFS can return from dead ends and try other
paths.

maze_unsolvable.txt has no open path from S to E. Both searches should report:

    Path found: no

A maze is not always guaranteed to have a solution. If a route exists, BFS
and DFS can find one. If no route exists, the program reports no path.

GITHUB FOLDER
-------------
Keep the folder named Final Project at the top level of the course repository,
next to folders such as Day 1 and Day 2.
