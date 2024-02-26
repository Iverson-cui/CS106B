// TODO: remove and replace this file header comment
// This is a .cpp file you will edit and turn in.
// Remove starter comments and add your own
// comments on each function and on complex code sections.
#include <iostream>
#include <fstream>
#include "error.h"
#include "filelib.h"
#include "grid.h"
#include "maze.h"
#include "mazegraphics.h"
#include "queue.h"
#include "set.h"
#include "stack.h"
#include "vector.h"
#include "testing/SimpleTest.h"
using namespace std;


/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
Set<GridLocation> generateValidMoves(Grid<bool>& maze, GridLocation cur) {
    Set<GridLocation> neighbors;
    GridLocation North(cur.row - 1,cur.col);
    GridLocation East(cur.row,cur.col - 1);
    GridLocation West(cur.row,cur.col + 1);
    GridLocation South(cur.row + 1,cur.col);
    // if the northern element is in bound and true
    if (maze.inBounds(North) && maze[North]) {
        neighbors.add(North);
    }
    // if the eastern element is in bound and true
    if (maze.inBounds(East) && maze[East]) {
        neighbors.add(East);
    }
    // if the western element is in bound and true
    if (maze.inBounds(West) && maze[West]) {
        neighbors.add(West);
    }
    // if the southern element is in bound and true
    if (maze.inBounds(South) && maze[South]) {
        neighbors.add(South);
    }
    /* TODO: Fill in the remainder of this function. */

    return neighbors;
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
void validatePath(Grid<bool>& maze, Stack<GridLocation> path) {
    // The exit is at the lower right corner of the maze.
    GridLocation mazeExit = {maze.numRows()-1,  maze.numCols()-1};
    //GridLocation mazeentry(0,0);
    if (path.peek() != mazeExit) {
        error("Path does not end at maze exit");
    }
    /* TODO: Fill in the remainder of this function. */
    Set<GridLocation> done;
    int size = path.size();
    // The reason why i <= instead of = is that I need to examine the first is entry or not.
    for(int i = 0; i < size - 1; i++) {
        if (done.contains(path.peek())) {
            error("This path has loops");
        }
        else {
            // Add this to the done set
            done.add(path.peek());
            // To test if the last but two move is a valid move of the last move as well as pop the last move.
            if ( !generateValidMoves(maze,path.pop()).contains(path.peek())) {
                error("This is not a valid path.");
            }
        }
    }
    GridLocation entry(0,0);
    if (path.pop() != entry) {
        error("This path doesn't start with the entry");
    }

    /* If you find a problem with the path, call error() to report it.
     * If the path is a valid solution, then this function should run to completion
     * without throwing any errors.
     */
}

/* TODO: Replace this comment with a descriptive function
 * header comment.
 */
Stack<GridLocation> solveMaze(Grid<bool>& maze) {
    MazeGraphics::drawGrid(maze);
    Stack<GridLocation> path;
    /* TODO: Fill in the remainder of this function. */
    Queue < Stack<GridLocation> > paths;
    Stack <GridLocation> entry = {GridLocation (0,0)};
    // For every queue, there is one entry element at first.
    paths.enqueue(entry);

    Set <GridLocation> validneighbors = {GridLocation(0,0)};
    while(true) {
        Stack <GridLocation> curpath = paths.dequeue();
        // Test if the current path ends with exit
        if (curpath.peek() == GridLocation (maze.numRows() - 1, maze.numCols() - 1)) {
            MazeGraphics::highlightPath(curpath,"red",1000);
            return curpath;
        }
        else {
            //validneighbors += generateValidMoves(maze, curpath.peek());

            for ( GridLocation moves_waited_to_be_checked : generateValidMoves(maze, curpath.peek()) ) {
                // check if this move has been searched.
                if (!validneighbors.contains( moves_waited_to_be_checked)) {
                    validneighbors.add(moves_waited_to_be_checked);
                    curpath.push(moves_waited_to_be_checked);
                    paths.enqueue(curpath);
                    curpath.pop();
                }

            }
        }
    }
}

/*
 * The given readMazeFile function correctly reads a well-formed
 * maze from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readMazeFile(string filename, Grid<bool>& maze) {
    /* The following code reads data from the file into a Vector
     * of strings representing the lines of the file.
     */
    ifstream in;

    if (!openFile(in, filename))
        error("Cannot open file named " + filename);

    Vector<string> lines;
    readEntireFile(in, lines);

    /* Now that the file data has been read into the Vector, populate
     * the maze grid.
     */
    int numRows = lines.size();        // rows is count of lines
    int numCols = lines[0].length();   // cols is length of line
    maze.resize(numRows, numCols);     // resize grid dimensions

    for (int r = 0; r < numRows; r++) {
        if (lines[r].length() != numCols) {
            error("Maze row has inconsistent number of columns");
        }
        for (int c = 0; c < numCols; c++) {
            char ch = lines[r][c];
            if (ch == '@') {        // wall
                maze[r][c] = false;
            } else if (ch == '-') { // corridor
                maze[r][c] = true;
            } else {
                error("Maze location has invalid character: '" + charToString(ch) + "'");
            }
        }
    }
}

/* The given readSolutionFile correctly reads a path from a file.
 *
 * This provided function is fully correct. You do not need to change
 * any of the code in this function.
 */
void readSolutionFile(string filename, Stack<GridLocation>& soln) {
    ifstream in;

    if (!openFile(in, filename)) {
        error("Cannot open file named " + filename);
    }

    if (!(in >> soln)) {// if not successfully read
        error("Maze solution did not have the correct format.");
    }
}


/* * * * * * Test Cases * * * * * */

STUDENT_TEST("generate valid moves my test eg1"){
    Grid<bool> maze = {{true, true, false},
                       {false, true, true},
                       {false, false, true}};
    GridLocation test = {0,1};
    Set<GridLocation> expected = {{0,0},{1,1}};
    EXPECT_EQUAL(generateValidMoves(maze, test), expected);
}
STUDENT_TEST("generate valid moves my test eg2"){
    Grid<bool> maze = {{true, true, false},
                       {true, true, true},
                       {false, false, true}};
    GridLocation test = {1,1};
    Set<GridLocation> expected = {{0,1},{1,0},{1,2}};
    EXPECT_EQUAL(generateValidMoves(maze, test), expected);
}
STUDENT_TEST("validatePath on correct solution eg1") {
    Grid<bool> maze = {{true, true, true},
                       {false,false, true},
                       {false, false, true}};
    Stack<GridLocation> soln = { {0 ,0}, {0, 1}, {0, 2}, {1,2}, {2,2} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

STUDENT_TEST("validatePath on correct solution eg2") {
    Grid<bool> maze = {{true, true, true,true,true},
                       {false,false, false,false,true},
                        {true, true, true,true,true},
                        {true,false,false,false,false},
                       {true,true,true,true,true}};
    Stack<GridLocation> soln = { {0 ,0}, {0, 1}, {0, 2}, {0,3}, {0,4},{1,4},{2,4},{2,3},{2,2},{2,1},{2,0},{3,0},{4,0},{4,1},{4,2},{4,3},{4,4} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("generateValidMoves on location in the center of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation center = {1, 1};
    Set<GridLocation> expected = {{0, 1}, {1, 0}, {1, 2}, {2, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, center), expected);
}

PROVIDED_TEST("generateValidMoves on location on the side of 3x3 grid with no walls") {
    Grid<bool> maze = {{true, true, true},
                       {true, true, true},
                       {true, true, true}};
    GridLocation side = {0, 1};
    Set<GridLocation> expected = {{0,0}, {0,2}, {1, 1}};

    EXPECT_EQUAL(generateValidMoves(maze, side), expected);
}

PROVIDED_TEST("generateValidMoves on corner of 2x2 grid with walls") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    GridLocation corner = {0, 0};
    Set<GridLocation> expected = {{1, 0}};

    EXPECT_EQUAL(generateValidMoves(maze, corner), expected);
}

PROVIDED_TEST("validatePath on correct solution") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> soln = { {0 ,0}, {1, 0}, {1, 1} };

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for medium maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/5x7.maze", maze);
    readSolutionFile("res/5x7.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on correct solution loaded from file for large maze") {
    Grid<bool> maze;
    Stack<GridLocation> soln;
    readMazeFile("res/25x33.maze", maze);
    readSolutionFile("res/25x33.soln", soln);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("validatePath on invalid path should raise error") {
    Grid<bool> maze = {{true, false},
                       {true, true}};
    Stack<GridLocation> not_end_at_exit = { {1, 0}, {0, 0} };
    Stack<GridLocation> not_begin_at_entry = { {1, 0}, {1, 1} };
    Stack<GridLocation> go_through_wall = { {0 ,0}, {0, 1}, {1, 1} };
    Stack<GridLocation> teleport = { {0 ,0}, {1, 1} };
    Stack<GridLocation> revisit = { {0 ,0}, {1, 0}, {0, 0}, {1, 0}, {1, 1} };

    EXPECT_ERROR(validatePath(maze, not_end_at_exit));
    EXPECT_ERROR(validatePath(maze, not_begin_at_entry));
    EXPECT_ERROR(validatePath(maze, go_through_wall));
    EXPECT_ERROR(validatePath(maze, teleport));
    EXPECT_ERROR(validatePath(maze, revisit));
}

PROVIDED_TEST("solveMaze on file 5x7") {
    Grid<bool> maze;
    readMazeFile("res/5x7.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

PROVIDED_TEST("solveMaze on file 21x23") {
    Grid<bool> maze;
    readMazeFile("res/21x23.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}

// TODO: add your test cases here
PROVIDED_TEST("solveMaze on file 17x37") {
    Grid<bool> maze;
    readMazeFile("res/17x37.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}
PROVIDED_TEST("solveMaze on file 19x35") {
    Grid<bool> maze;
    readMazeFile("res/19x35.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}
PROVIDED_TEST("solveMaze on file 21x35") {
    Grid<bool> maze;
    readMazeFile("res/21x35.maze", maze);
    Stack<GridLocation> soln = solveMaze(maze);

    EXPECT_NO_ERROR(validatePath(maze, soln));
}
