#include <stdio.h>
#include <string.h>

/*
 * These constants set the largest maze that the program can read.
 * MAX_ROWS is the largest number of rows.
 * MAX_COLS is the largest number of columns.
 * MAX_CELLS is the largest number of positions that the BFS queue may hold.
 */
#define MAX_ROWS 100
#define MAX_COLS 200
#define MAX_CELLS 20000

/*
 * A Position represents one square in the maze.
 * row tells us which horizontal row the square is in.
 * column tells us which vertical column the square is in.
 */
typedef struct {
  int row;
  int column;
} Position;

/*
 * The maze is stored in a two-dimensional character array.
 * Each character is one square:
 *   # means wall
 *   . or space means open square
 *   S means start
 *   E means end
 *
 * rows and columns store the real size of the maze that was read.
 * The start and end variables store the locations of S and E.
 */
char maze[MAX_ROWS][MAX_COLS + 1];
int rows;
int columns;
int startRow;
int startColumn;
int endRow;
int endColumn;

/*
 * These variables are used only by BFS.
 *
 * bfsVisited:
 *   1 means BFS has already discovered this square.
 *   0 means BFS has not discovered it yet.
 *
 * bfsPath:
 *   1 means this square is part of the final BFS route.
 *
 * parentRow and parentColumn:
 *   These remember which previous square led to each new square.
 *   After BFS finds E, the program uses these arrays to walk
 *   backward from E to S and rebuild the route.
 */
int bfsVisited[MAX_ROWS][MAX_COLS];
int bfsPath[MAX_ROWS][MAX_COLS];
int parentRow[MAX_ROWS][MAX_COLS];
int parentColumn[MAX_ROWS][MAX_COLS];
int bfsFound;
int bfsExplored;
int bfsPathLength;

/*
 * DFS uses separate arrays so that it does not erase or change
 * the BFS result.
 *
 * dfsVisited records every square checked by DFS.
 * dfsPath records only the final successful DFS route.
 *
 * DFS does not use parentRow or parentColumn in this program.
 * It marks the route while the recursive calls return.
 */
int dfsVisited[MAX_ROWS][MAX_COLS];
int dfsPath[MAX_ROWS][MAX_COLS];
int dfsFound;
int dfsExplored;
int dfsPathLength;

/*
 * Read the maze from a text file.
 *
 * This function:
 * 1. Opens the file.
 * 2. Reads one row at a time.
 * 3. Removes the newline character from the end of each row.
 * 4. Checks that every row has the same length.
 * 5. Checks that every character is allowed.
 * 6. Finds the positions of S and E.
 *
 * The function returns 1 when the maze is valid.
 * It returns 0 when there is an error.
 */
int readMaze(char filename[]) {
  FILE *file;
  char line[MAX_COLS + 2];
  int lineLength;
  int expectedLength = -1;
  int startCount = 0;
  int endCount = 0;
  int column;

  /* Try to open the file for reading. */
  file = fopen(filename, "r");

  if (file == NULL) {
    printf("Could not open %s.\n", filename);
    return 0;
  }

  rows = 0;

  /* fgets reads one maze row at a time. */
  while (fgets(line, sizeof(line), file) != NULL) {
    lineLength = (int) strlen(line);

    /*
     * A text file normally puts \n at the end of each row.
     * We remove it because it is not part of the maze.
     */
    if (lineLength > 0 && line[lineLength - 1] == '\n') {
      line[lineLength - 1] = '\0';
      lineLength--;
    }

    /*
     * Some text files also contain \r before the newline.
     * Removing it allows the same file format to work on
     * different operating systems.
     */
    if (lineLength > 0 && line[lineLength - 1] == '\r') {
      line[lineLength - 1] = '\0';
      lineLength--;
    }

    /* A blank row would break the rectangular maze shape. */
    if (lineLength == 0) {
      printf("The maze cannot have a blank row.\n");
      fclose(file);
      return 0;
    }

    /* Stop if the maze is larger than our fixed array. */
    if (rows >= MAX_ROWS) {
      printf("The maze has too many rows.\n");
      fclose(file);
      return 0;
    }

    /*
     * The first row tells us how many columns the maze should have.
     * Every later row must have this same length.
     */
    if (expectedLength == -1) {
      expectedLength = lineLength;
    }

    if (lineLength != expectedLength) {
      printf("Every maze row must have the same length.\n");
      fclose(file);
      return 0;
    }

    /*
     * Check every square in this row.
     * At the same time, copy the square into the maze array
     * and remember where S and E are located.
     */
    for (column = 0; column < lineLength; column++) {
      if (line[column] != '#'
          && line[column] != '.'
          && line[column] != ' '
          && line[column] != 'S'
          && line[column] != 'E') {
        printf("The maze contains an unknown symbol.\n");
        fclose(file);
        return 0;
      }

      maze[rows][column] = line[column];

      if (line[column] == 'S') {
        startRow = rows;
        startColumn = column;
        startCount++;
      }

      if (line[column] == 'E') {
        endRow = rows;
        endColumn = column;
        endCount++;
      }
    }

    /*
     * Add \0 so this row can also be printed as a normal C string.
     */
    maze[rows][lineLength] = '\0';
    rows++;
  }

  fclose(file);

  if (rows == 0) {
    printf("The maze file is empty.\n");
    return 0;
  }

  /*
   * The program needs exactly one start and exactly one end.
   */
  if (startCount != 1 || endCount != 1) {
    printf("The maze needs exactly one S and one E.\n");
    return 0;
  }

  columns = expectedLength;
  return 1;
}

/*
 * Clear all old BFS and DFS information before a new search.
 *
 * The program uses 0 to mean "not visited" or "not in the path."
 * The parent arrays use -1 to mean that no parent has been saved yet.
 */
void clearSearchArrays(void) {
  int row;
  int column;

  for (row = 0; row < rows; row++) {
    for (column = 0; column < columns; column++) {
      bfsVisited[row][column] = 0;
      bfsPath[row][column] = 0;
      parentRow[row][column] = -1;
      parentColumn[row][column] = -1;
      dfsVisited[row][column] = 0;
      dfsPath[row][column] = 0;
    }
  }

  bfsFound = 0;
  bfsExplored = 0;
  bfsPathLength = 0;
  dfsFound = 0;
  dfsExplored = 0;
  dfsPathLength = 0;
}

/*
 * Check whether the program is allowed to move to one square.
 *
 * The move is not allowed when:
 * 1. The row is outside the maze.
 * 2. The column is outside the maze.
 * 3. The square is a wall.
 *
 * The function returns 1 for a legal move and 0 for an illegal move.
 */
int canMove(int row, int column) {
  if (row < 0 || row >= rows || column < 0 || column >= columns) {
    return 0;
  }

  if (maze[row][column] == '#') {
    return 0;
  }

  return 1;
}

/*
 * Build the final BFS route after BFS reaches E.
 *
 * BFS saved the previous location of every discovered square
 * in parentRow and parentColumn.
 *
 * We start at E and repeatedly move to its parent.
 * This continues until we return to S.
 * Every square on this backward trip is marked in bfsPath.
 */
void buildBfsPath(void) {
  int row = endRow;
  int column = endColumn;
  int previousRow;
  int previousColumn;

  while (row != startRow || column != startColumn) {
    bfsPath[row][column] = 1;
    bfsPathLength++;

    previousRow = parentRow[row][column];
    previousColumn = parentColumn[row][column];

    row = previousRow;
    column = previousColumn;
  }

  /* Mark S as part of the route too. */
  bfsPath[startRow][startColumn] = 1;
}

/*
 * Solve the maze with Breadth-First Search.
 *
 * BFS uses a queue:
 * - rear is where a new position is added.
 * - front is where the next position is removed.
 *
 * Because the queue is first in, first out, BFS checks closer
 * squares before farther squares. Since every move costs one step,
 * the first route BFS finds to E is a shortest route.
 */
void solveBfs(void) {
  Position queue[MAX_CELLS];
  Position current;
  int front = 0;
  int rear = 0;
  int nextRow;
  int nextColumn;

  /*
   * Put S into the queue first.
   * Mark it visited immediately so it is not added again later.
   */
  queue[rear].row = startRow;
  queue[rear].column = startColumn;
  rear++;
  bfsVisited[startRow][startColumn] = 1;

  /*
   * Continue while the queue still has positions to check.
   * front moves forward when we remove a position.
   * rear moves forward when we add a position.
   */
  while (front < rear) {
    current = queue[front];
    front++;
    bfsExplored++;

    /* Stop as soon as BFS removes E from the queue. */
    if (current.row == endRow && current.column == endColumn) {
      bfsFound = 1;
      break;
    }

    /*
     * Try moving up.
     * If the square is open and not visited:
     * 1. Mark it visited.
     * 2. Save the current square as its parent.
     * 3. Add it to the rear of the queue.
     */
    nextRow = current.row - 1;
    nextColumn = current.column;

    if (canMove(nextRow, nextColumn) == 1
        && bfsVisited[nextRow][nextColumn] == 0) {
      bfsVisited[nextRow][nextColumn] = 1;
      parentRow[nextRow][nextColumn] = current.row;
      parentColumn[nextRow][nextColumn] = current.column;
      queue[rear].row = nextRow;
      queue[rear].column = nextColumn;
      rear++;
    }

    /* Try moving right using the same steps. */
    nextRow = current.row;
    nextColumn = current.column + 1;

    if (canMove(nextRow, nextColumn) == 1
        && bfsVisited[nextRow][nextColumn] == 0) {
      bfsVisited[nextRow][nextColumn] = 1;
      parentRow[nextRow][nextColumn] = current.row;
      parentColumn[nextRow][nextColumn] = current.column;
      queue[rear].row = nextRow;
      queue[rear].column = nextColumn;
      rear++;
    }

    /* Try moving down using the same steps. */
    nextRow = current.row + 1;
    nextColumn = current.column;

    if (canMove(nextRow, nextColumn) == 1
        && bfsVisited[nextRow][nextColumn] == 0) {
      bfsVisited[nextRow][nextColumn] = 1;
      parentRow[nextRow][nextColumn] = current.row;
      parentColumn[nextRow][nextColumn] = current.column;
      queue[rear].row = nextRow;
      queue[rear].column = nextColumn;
      rear++;
    }

    /* Try moving left using the same steps. */
    nextRow = current.row;
    nextColumn = current.column - 1;

    if (canMove(nextRow, nextColumn) == 1
        && bfsVisited[nextRow][nextColumn] == 0) {
      bfsVisited[nextRow][nextColumn] = 1;
      parentRow[nextRow][nextColumn] = current.row;
      parentColumn[nextRow][nextColumn] = current.column;
      queue[rear].row = nextRow;
      queue[rear].column = nextColumn;
      rear++;
    }
  }

  /*
   * BFS only builds a path when E was actually found.
   */
  if (bfsFound == 1) {
    buildBfsPath();
  }
}

/*
 * This is the recursive DFS function.
 *
 * DFS tries one route as deeply as possible.
 * The direction order is:
 *   up, right, down, left
 *
 * When one direction reaches a dead end, the function returns
 * and tries another direction. This is called backtracking.
 *
 * This DFS does not use a parent array.
 * When E is found, the successful recursive calls return 1.
 * While those calls return, each square is marked in dfsPath.
 */
int searchDfs(int row, int column) {
  /* Mark the current square as visited. */
  dfsVisited[row][column] = 1;
  dfsExplored++;

  /*
   * Base case:
   * If the current square is E, the search is successful.
   */
  if (row == endRow && column == endColumn) {
    dfsPath[row][column] = 1;
    return 1;
  }

  /*
   * Try moving up.
   * We call searchDfs again, which is the recursive step.
   * If the deeper call finds E, mark the current square as
   * part of the successful path and return 1.
   */
  if (canMove(row - 1, column) == 1
      && dfsVisited[row - 1][column] == 0) {
    if (searchDfs(row - 1, column) == 1) {
      dfsPath[row][column] = 1;
      dfsPathLength++;
      return 1;
    }
  }

  /* Try moving right. */
  if (canMove(row, column + 1) == 1
      && dfsVisited[row][column + 1] == 0) {
    if (searchDfs(row, column + 1) == 1) {
      dfsPath[row][column] = 1;
      dfsPathLength++;
      return 1;
    }
  }

  /* Try moving down. */
  if (canMove(row + 1, column) == 1
      && dfsVisited[row + 1][column] == 0) {
    if (searchDfs(row + 1, column) == 1) {
      dfsPath[row][column] = 1;
      dfsPathLength++;
      return 1;
    }
  }

  /* Try moving left. */
  if (canMove(row, column - 1) == 1
      && dfsVisited[row][column - 1] == 0) {
    if (searchDfs(row, column - 1) == 1) {
      dfsPath[row][column] = 1;
      dfsPathLength++;
      return 1;
    }
  }

  /*
   * Returning 0 means no route from this square reached E.
   * The previous recursive call can now try another direction.
   */
  return 0;
}

/*
 * Start DFS at S.
 * searchDfs returns 1 when it finds E.
 */
void solveDfs(void) {
  if (searchDfs(startRow, startColumn) == 1) {
    dfsFound = 1;
  }
}

/*
 * Print the original maze exactly as it was read from the file.
 */
void printOriginalMaze(void) {
  int row;

  printf("Original maze:\n");

  for (row = 0; row < rows; row++) {
    printf("%s\n", maze[row]);
  }
}

/*
 * Print the BFS search result.
 *
 * #, S, and E keep their original symbols.
 * * means the square is part of the final BFS route.
 * + means BFS visited the square, but it is not in the final route.
 * Other squares keep their original character.
 */
void printBfsResult(void) {
  int row;
  int column;

  printf("\nBFS result:\n");

  for (row = 0; row < rows; row++) {
    for (column = 0; column < columns; column++) {
      if (maze[row][column] == '#'
          || maze[row][column] == 'S'
          || maze[row][column] == 'E') {
        printf("%c", maze[row][column]);
      } else if (bfsPath[row][column] == 1) {
        printf("*");
      } else if (bfsVisited[row][column] == 1) {
        printf("+");
      } else {
        printf("%c", maze[row][column]);
      }
    }
    printf("\n");
  }

  if (bfsFound == 1) {
    printf("Path found: yes\n");
    printf("Path length: %d moves\n", bfsPathLength);
  } else {
    printf("Path found: no\n");
  }

  printf("Explored cells: %d\n", bfsExplored);
}

/*
 * Print the DFS search result.
 *
 * The symbols have the same meaning as the BFS output:
 * * is the final route.
 * + is visited but not part of the final route.
 */
void printDfsResult(void) {
  int row;
  int column;

  printf("\nDFS result:\n");

  for (row = 0; row < rows; row++) {
    for (column = 0; column < columns; column++) {
      if (maze[row][column] == '#'
          || maze[row][column] == 'S'
          || maze[row][column] == 'E') {
        printf("%c", maze[row][column]);
      } else if (dfsPath[row][column] == 1) {
        printf("*");
      } else if (dfsVisited[row][column] == 1) {
        printf("+");
      } else {
        printf("%c", maze[row][column]);
      }
    }
    printf("\n");
  }

  if (dfsFound == 1) {
    printf("Path found: yes\n");
    printf("Path length: %d moves\n", dfsPathLength);
  } else {
    printf("Path found: no\n");
  }

  printf("Explored cells: %d\n", dfsExplored);
}

/*
 * Print the main numbers from BFS and DFS.
 *
 * Path length means the number of moves in the final route.
 * Explored cells means the number of squares checked during the search.
 */
void printComparison(void) {
  printf("\nComparison:\n");

  if (bfsFound == 1) {
    printf("BFS path length: %d\n", bfsPathLength);
  } else {
    printf("BFS path length: no path\n");
  }

  if (dfsFound == 1) {
    printf("DFS path length: %d\n", dfsPathLength);
  } else {
    printf("DFS path length: no path\n");
  }

  printf("BFS explored cells: %d\n", bfsExplored);
  printf("DFS explored cells: %d\n", dfsExplored);

  /*
   * Only compare route lengths when both algorithms found a route.
   */
  if (bfsFound == 1 && dfsFound == 1) {
    if (bfsPathLength < dfsPathLength) {
      printf("BFS found the shorter path.\n");
    } else if (bfsPathLength == dfsPathLength) {
      printf("BFS and DFS found paths with the same length.\n");
    }
  }
}

/*
 * main is the starting point of the program.
 *
 * argc tells us how many command-line parts were entered.
 * argv stores those parts.
 *
 * The program expects one maze filename after ./maze_solver.
 */
int main(int argc, char *argv[]) {
  if (argc != 2) {
    printf("Use: ./maze_solver maze-file\n");
    printf("Example: ./maze_solver mazes/maze_compare.txt\n");
    return 1;
  }

  /*
   * argv[1] is the maze filename.
   * Stop if the file cannot be read or is not a valid maze.
   */
  if (readMaze(argv[1]) == 0) {
    return 1;
  }

  /* Clear old values, then run both algorithms. */
  clearSearchArrays();
  solveBfs();
  solveDfs();

  /* Print the input, both results, and the final comparison. */
  printOriginalMaze();
  printBfsResult();
  printDfsResult();
  printComparison();

  return 0;
}
