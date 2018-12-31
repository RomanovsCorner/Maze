/*******************************************************************************************************************************
 * File: Maze.cpp
 * Author: Romanov Andre
 * Class: COP2001
 * Purpose: Create a maze generator using bitwise operators, arrays, stacks, and several other functions
*******************************************************************************************************************************/ 

#include <iostream>
#include <string>
#include <time.h>

#define BYTE unsigned char

// wall & visited flag values
const BYTE CELL_TOP = 1 << 0;
const BYTE CELL_BOT = 1 << 1;
const BYTE CELL_LEFT = 1 << 2;
const BYTE CELL_RIGHT = 1 << 3;
const BYTE CELL_VISITED = 1 << 4;
const BYTE CELL_MOUSE = 1 << 5;

// maze print characters
const char OUT_TOP_LEFT = '-';
const char OUT_TOP_MID = '-';
const char OUT_TOP_RIGHT = '-';
const char OUT_SIDE_LEFT = '|';
const char OUT_SIDE_RIGHT = '|';
const char OUT_BOT_LEFT = '-';
const char OUT_BOT_MID = '-';
const char OUT_BOT_RIGHT = '-';
const char INSIDE_MIDDLE = '+';
const char CELL_TOP_BOT = '-';
const char CELL_LEFT_RIGHT = '|';
const char CELL_OPEN_HORZ = ' ';
const char CELL_OPEN_VERT = ' ';
const char CELL_VISITED_ON = '.';
const char CELL_VISITED_OFF = ' ';
const char CELL_MOUSE_CHAR = '*';



// maze dimensions
const int MAZE_ROWS =  3; 
const int MAZE_COLS =  3;

// location indexes
const int CELL_ROW_INDEX = 0;
const int CELL_COL_INDEX = 1;

// start cell location
const int START_ROW = 0;
const int START_COL = 0;

// end cell location
const int END_ROW = MAZE_ROWS - 1;
const int END_COL = MAZE_COLS - 1;

// function declerations
void initMaze(BYTE cells[][MAZE_COLS]);
int pushStack(int stack[][2], int location[], int stackPoint);
void popStack(int stack[][2], int location[], int& stackPoint);
void printMazeDebug(BYTE cells[][MAZE_COLS]);
void printMaze(BYTE cells[][MAZE_COLS]);
void toggleByte(BYTE cells[][MAZE_COLS], int location[], BYTE target);
void copyOneLocTwo(int locOne[], int locTwo[]);
bool hasUnvisitedCells(BYTE cells[][MAZE_COLS]);
bool hasAvailableNeighbors(BYTE cells[][MAZE_COLS], int location[]);
void chooseRandomNeighbor(BYTE cells[][MAZE_COLS], int current[], int neighbor[]);
void removeWalls(BYTE cells[][MAZE_COLS], int current[], int neighbor[]);
void generateMaze(BYTE maze[][MAZE_COLS], int stack[][2], int stackPointer);
void solveMaze(BYTE maze[][MAZE_COLS], int stack[][2], int stackPointer);
void chooseNoWallNeighbor(BYTE cells[][MAZE_COLS], int current[], int neighbor[]);
void waitForUserToContinue();

enum MENU { EXIT, GENERATE, SOLVE };

int main() {
    
    // init random generator 
    srand(time(NULL));
    
    // storage for the maze cells
    BYTE maze[MAZE_ROWS][MAZE_COLS] = {0};
    
    // storage for our stack of visited cells
    int stack[MAZE_ROWS * MAZE_COLS][2] = {0};
    int stackPointer = -1; // stack is empty
    
    // checks if a maze has been generated
    bool generated = false;
    
    // user choice
    MENU choice = EXIT;
    std::cout << "Welcome to the random maze generator: " << std::endl;
    std::cout << "Mr. Nibbles (mouse maze solver) will be assisting you. \n" << std::endl;
    
    do {
        
        // all available options
        std::cout << "1. Generate a new random maze" << std::endl;
        std::cout << "2. Solve Maze" << std::endl;
        std::cout << "0. Exit" << std::endl;
        
        //gets user input
        std::cout << "Select an option: ";
        int option = 0;
        std::cin >> option;
        choice = MENU(option);
        std::cout << std::endl;
        std::cin.ignore();

        switch(choice) {
            
            case GENERATE:
            initMaze(maze);
            generateMaze(maze, stack, stackPointer);
            generated = true;
            break;
            
            case SOLVE:
            if (generated) 
            solveMaze(maze, stack, stackPointer);
            else
                std::cout << "Please generate a maze.";
            break;
            
            case EXIT:
            std::cout << "Successfully closed. \n";
            std::cout << "Mr. Nibbles will patiently wait for your return." << std::endl;
            std::cout << "Notice: No mice were hurt in the process of making this program.";
            break;

        }
        std::cout << "\n" << std::endl;
        
    } while(choice != EXIT);
}

/**
 * Initialize maze array elements to turn all
 * walls on and visited off
 */
void initMaze(BYTE cells[][MAZE_COLS]) {
    std::cout << "Initializing Maze..." << std::endl;
	for (int row = 0; row < MAZE_ROWS; row++) {
		for (int col = 0; col < MAZE_COLS; col++) {
			cells[row][col] = CELL_TOP | CELL_BOT | CELL_LEFT | CELL_RIGHT;
		}
	}	
}

/**
 * Adds a cell location onto the stack
 * @param stack - the location stack
 * @param location - row & col of a maze cell
 * @param stackPoint - last location added to stack
 * @return int - new stack pointer
 */
int pushStack(int stack[][2], int location[], int stackPoint) {
	int spNew = stackPoint;
	spNew++;
	stack[spNew][CELL_ROW_INDEX] = location[CELL_ROW_INDEX];
	stack[spNew][CELL_COL_INDEX] = location[CELL_COL_INDEX];
	return spNew;
}

/**
 * Adds a cell location onto the stack
 * @param stack - the location stack
 * @param location - row & col of a maze cell
 * @param stackPoint - last location added to stack
 * @return int - new stack pointer
 */
void popStack(int stack[][2], int location[], int& stackPoint) {
	location[CELL_ROW_INDEX] = stack[stackPoint][CELL_ROW_INDEX];
	location[CELL_COL_INDEX] = stack[stackPoint][CELL_COL_INDEX];
	stackPoint--;
}

/**
 * Printing the maze cell values to the console
 * @param cells - the grid of cells in the maze 
 */
void printMazeDebug(BYTE cells[][MAZE_COLS]) {
	for (int row = 0; row < MAZE_ROWS; row++) {
		for (int col = 0; col < MAZE_COLS; col++) {
			std::cout << std::to_string(int(cells[row][col])) << " ";
		}
		std::cout << std::endl;
	}
}

/**
 * Printing the maze cell values to the console
 * @param cells - the grid of cells in the maze 
 */
void printMaze(BYTE maze[][MAZE_COLS]) {
    for (int row = 0; row < MAZE_ROWS; row++) {    
        // print top row of cells 
        for (int col = 0; col < MAZE_COLS; col++) {
            // prints left spacer
            
            // checks for the top row
            if (row == 0) {
          
                //checks for the left wall
                if (col == 0) {
                    std::cout << OUT_TOP_LEFT;
                } else {
                    std::cout << OUT_TOP_MID;
                }
            } else { // not on top row
          
                //checks for left wall
                if (col == 0) {
                    std::cout << OUT_SIDE_LEFT;
                } else {
                    std::cout << INSIDE_MIDDLE;
                }
            } // print top left spacer
            
                // print cell top
                if (maze[row][col] & CELL_TOP) {
                    std::cout << CELL_TOP_BOT;
                } else {
                    std::cout << CELL_OPEN_HORZ;
                }
    
            // print last right spacer
            if (col == MAZE_COLS - 1) {
                // top row
                if (row == 0) {
                    std::cout << OUT_TOP_RIGHT;
                } else  { // not on top row
                    std::cout << OUT_SIDE_RIGHT;
                }
            }
        }
        std::cout << std::endl;
    
        // print side walls of the cells
        for (int col = 0; col < MAZE_COLS; col++) {
    
            // print cell left side wall
            if (maze[row][col] & CELL_LEFT) {
                std::cout << CELL_LEFT_RIGHT;    
            } else {
                std::cout << CELL_OPEN_VERT;
            }
            
            // print cell visited
            if (maze[row][col] & CELL_VISITED && (maze[row][col] & CELL_MOUSE)) {
                std::cout << CELL_MOUSE_CHAR;   
            } else {
            
            // print cell visited
            if (maze[row][col] & CELL_VISITED && !(maze[row][col] & CELL_MOUSE)) {
                std::cout << CELL_VISITED_ON;   
            } else {
                std::cout << CELL_VISITED_OFF;   
            }
            }
    
            // print wall right
            if (col == MAZE_COLS - 1) {
                // print cell left side wall
                if (maze[row][col] & CELL_RIGHT) {
                    std::cout << CELL_LEFT_RIGHT;
                } else {
                    std::cout << CELL_OPEN_VERT;
                }
            }
        } // print side walls
        std::cout << std::endl;
    
        // print row bottom
        if (row == MAZE_ROWS -1) {
            // print the bottom row of the cell walls
            for (int col = 0; col < MAZE_COLS; col++) {
    
                // print spacer 
                if (col == 0) {
                   std::cout <<  OUT_BOT_LEFT;
                } else {
                    std::cout << OUT_BOT_MID;   
                }
    
                // print cell bottom wall
                if (maze[row][col] & CELL_BOT) {
                    std::cout << CELL_TOP_BOT;    
                } else {
                    std::cout << CELL_OPEN_HORZ;    
                }
    
                // print right corner
                if (col == MAZE_COLS - 1) {
                    std::cout << OUT_BOT_RIGHT;    
                }
            } // bottom walls
            // end the maze
            std::cout << std::endl;
        }
    }
}


/**
 * Toggles the value of a targeted BYTE
 * @param cells - the grid cells in the maze
 * @param location - a cell in the maze
 * @param target - BYTE to toggle
 * 
 */
void toggleByte(BYTE cells[][MAZE_COLS], int location[], BYTE target) {
    cells[location[CELL_ROW_INDEX]][location[CELL_COL_INDEX]] ^= target;
}

/**
 * Copies an array to another
 * @param locOne - first array/cell
 * @param locTwo - second array/cell
 */
void copyOneLocTwo(int locOne[], int locTwo[]) {
    locTwo[CELL_ROW_INDEX] = locOne[CELL_ROW_INDEX];
    locTwo[CELL_COL_INDEX] = locOne[CELL_COL_INDEX];   
}

/**
 * Checks for any unvisited cell in the maze
 * @param cells - the grid cells in the maze
 */
bool hasUnvisitedCells(BYTE cells[][MAZE_COLS]) {
    for (int row = 0; row < MAZE_ROWS; row++) {
        for (int col = 0; col < MAZE_COLS; col++) {
            if (!(cells[row][col] & CELL_VISITED)) {
                return true;
            }
        }
    }
    return false;
}


/**
 * Checks for any available neighbor
 * location - a cell from the maze
 */
bool hasAvailableNeighbors(BYTE cells[][MAZE_COLS], int location[]) {
    bool available = false;
    
    // neighbor above
    if (location[CELL_ROW_INDEX] > 0) {
        if (!(cells[location[CELL_ROW_INDEX] - 1][location[CELL_COL_INDEX]] & CELL_VISITED)) available = true;
    }
    
    // neighbor below
    if (location[CELL_ROW_INDEX] < MAZE_ROWS - 1) {
        if (!(cells[location[CELL_ROW_INDEX] + 1][location[CELL_COL_INDEX]] & CELL_VISITED)) available = true;
    }
    
    // neighbor left
    if (location[CELL_COL_INDEX] > 0) {
        if (!(cells[location[CELL_ROW_INDEX]][location[CELL_COL_INDEX] - 1] & CELL_VISITED)) available = true;
    }
    
    // neighbor right
    if (location[CELL_COL_INDEX] < MAZE_COLS - 1) {
        if (!(cells[location[CELL_ROW_INDEX]][location[CELL_COL_INDEX] + 1] & CELL_VISITED)) available = true;
    }
    return available; 
}

/**
 * Finds an existing neighbor
 * @param cells - the grid cells in the maze
 * @param current - current location
 * @param neighbor - adjacent to current location
 */
void chooseRandomNeighbor(BYTE cells[][MAZE_COLS], int current[], int neighbor[]) {
    bool found = false;
    while (!found) {
        int randNeighbor = rand() % 4;
        switch (randNeighbor) {
            case 0:
                    if (current[CELL_ROW_INDEX] > 0) {
                        if (!(cells[current[CELL_ROW_INDEX] - 1][current[CELL_COL_INDEX]] & CELL_VISITED)) {
                            copyOneLocTwo(current, neighbor);
                            neighbor[CELL_ROW_INDEX]--;
                            found = true;
                        }       
                    }
                    break;
            case 1:
                    if (current[CELL_ROW_INDEX] < MAZE_ROWS - 1) {
                        if (!(cells[current[CELL_ROW_INDEX] + 1][current[CELL_COL_INDEX]] & CELL_VISITED)) {
                            copyOneLocTwo(current, neighbor);
                            neighbor[CELL_ROW_INDEX]++;
                            found = true;
                        }       
                    }
                    break;
            case 2:
                    if (current[CELL_COL_INDEX] > 0) {
                        if (!(cells[current[CELL_ROW_INDEX]][current[CELL_COL_INDEX] - 1] & CELL_VISITED)) {
                            copyOneLocTwo(current, neighbor);
                            neighbor[CELL_COL_INDEX]--;
                            found = true;
                        }       
                    }
                    break;
            case 3:
                    if (current[CELL_COL_INDEX] < MAZE_COLS - 1) {
                        if (!(cells[current[CELL_ROW_INDEX]][current[CELL_COL_INDEX] + 1] & CELL_VISITED)) {
                            copyOneLocTwo(current, neighbor);
                            neighbor[CELL_COL_INDEX]++;
                            found = true;
                        }       
                    }
                    break;

        }
    }
}

/**
 * Removes wall in between two cells
 * @param cells - the grid cells in the maze
 * @param current - current location
 * @param neighbor - adjacent to current location
 */
void removeWalls(BYTE cells[][MAZE_COLS], int current[], int neighbor[]) {
    // neighbor above 
    if (neighbor[CELL_ROW_INDEX] < current[CELL_ROW_INDEX]) {
        toggleByte(cells, neighbor, CELL_BOT);
        toggleByte(cells, current, CELL_TOP);
    } else if (neighbor[CELL_ROW_INDEX] > current[CELL_ROW_INDEX]) { // neighbor below 
        toggleByte(cells, neighbor, CELL_TOP);
        toggleByte(cells, current, CELL_BOT);
    } else if (neighbor[CELL_COL_INDEX] < current[CELL_COL_INDEX]) { // neighbor left
        toggleByte(cells, neighbor, CELL_RIGHT);
        toggleByte(cells, current, CELL_LEFT);
    } else { // neighbor right 
        toggleByte(cells, neighbor, CELL_LEFT);
        toggleByte(cells, current, CELL_RIGHT);
    }
}


/***
 * Generates a randomly carved maze 
 * @param maze - the grid cells
 * @param stack - the location of stack
 * @param stackPointer - last location added to stack
 */
void generateMaze(BYTE maze[][MAZE_COLS], int stack[][2], int stackPointer) {
    std::cout << "Generating Maze...\n" << std::endl;
    // set starting cell
    int startCell[2] = {START_ROW, START_COL}; 
	stackPointer = pushStack(stack, startCell, stackPointer);
	
    // turns off the starting cell's target wall
	toggleByte(maze, startCell, CELL_LEFT);
	
	// set the ending cell
	int endCell[2] = {END_ROW, END_COL};
	stackPointer = pushStack(stack, startCell, stackPointer);
	
	// turns off the ending cell's ending wall
	toggleByte(maze, endCell, CELL_RIGHT);
	
	// 1. Make the initial cell the current cell and mark it as visited
	int currentCell[2];
	copyOneLocTwo(startCell, currentCell);
	
	// mark visited flag
	toggleByte(maze, currentCell, CELL_VISITED);
	
	// 2. While there are unvisited cells 
    while(hasUnvisitedCells(maze)) {
        
        // i. If the current cell has any neighbors which have not been visited
        if (hasAvailableNeighbors(maze, currentCell)) {
        
        // 1. Find one of the unvisited neighbors
        int neighborCell[2] = {0};
        chooseRandomNeighbor(maze, currentCell, neighborCell);
        
        // 2. Push the current cell to the stack
        stackPointer = pushStack(stack, currentCell, stackPointer);
        
        // 3. Remove the wall between the current and the neighbor cell
        removeWalls(maze, currentCell, neighborCell);
        
        // 4. Make the chosen cell the current cell and mark it as visited
        copyOneLocTwo(neighborCell, currentCell);
        toggleByte(maze, currentCell, CELL_VISITED);
        
        // 5. Print the maze
        printMaze(maze);
        printMazeDebug(maze);
        std::cout << std::endl;
        
        } else if (stackPointer > -1) {     // ii. Else if the stack is not empty        
            // 1. pop last cell from stack int current 
            popStack(stack, currentCell, stackPointer);  
        }
    }    
}

/***
 * Solves the generated maze 
 * @param maze - the grid cells
 * @param stack - the location of stack
 * @param stackPointer - last location added to stack
 */
void solveMaze(BYTE maze[][MAZE_COLS], int stack[][2], int stackPointer) {
    std::cout << "Mr. Nibbles will attempt to solve the maze... \n" << std::endl;
    
    // reset all visited cells in our maze
    for (int row = 0; row < MAZE_ROWS; row++) {
        for (int col = 0; col < MAZE_COLS; col++) {
            int currentCell[2] = {row, col};
            toggleByte(maze, currentCell, CELL_VISITED);
        }
    } 
    
    // set starting cell
    int startCell[2] = {START_ROW, START_COL}; 
	stackPointer = pushStack(stack, startCell, stackPointer);
		
	// set ending cell
	int endCell[2] = {END_ROW, END_COL};
	stackPointer = pushStack(stack, endCell, stackPointer);
		
	// 1. Set mouse cell as starting cell and mark it as visited
	int mouseCell[2];
	copyOneLocTwo(startCell, mouseCell);
	
	// mark visited flag
	toggleByte(maze, mouseCell, CELL_VISITED);
	toggleByte(maze, mouseCell, CELL_MOUSE);
	
    // shows mouse very first position
    printMaze(maze);
    printMazeDebug(maze);
    std::cout << std::endl;
    waitForUserToContinue();	
	
		// 2. While there are unvisited cells 
        while(hasUnvisitedCells(maze)) { 
            
            // mouse cell is at same position as end cell
            if (mouseCell[CELL_ROW_INDEX] == endCell[CELL_ROW_INDEX] && mouseCell[CELL_COL_INDEX] == endCell[CELL_COL_INDEX]) {
                break;
            }
            
            // removes mouse old position
            toggleByte(maze, mouseCell, CELL_MOUSE);
            
            // i. If the current cell has any neighbors which have not been visited
            if (hasAvailableNeighbors(maze, mouseCell)) {
                
                // 1. Finds one of the unvisited neighbors if there isn't a wall in the way
                int neighborCell[2] = {0};
                chooseNoWallNeighbor(maze, mouseCell, neighborCell);  
                
                // 2. Push the current cell to the stack
                stackPointer = pushStack(stack, mouseCell, stackPointer);
                            
                // 4. Make the chosen cell the current cell and mark it as visited
                copyOneLocTwo(neighborCell, mouseCell);
                toggleByte(maze, mouseCell, CELL_VISITED);
                // shows mouse new position
                toggleByte(maze, mouseCell, CELL_MOUSE);
                
                // 5. Print the maze
                printMaze(maze);
                printMazeDebug(maze);
                std::cout << std::endl; 
                waitForUserToContinue();	

                
                } else if (stackPointer > -1) {     // ii. Else if the stack is not empty
                    std::cout << "Nice try, but Mr. Nibbles is persistent." << std::endl;
                    std::cout << "Back Tracking..." << std::endl;
                    
                    // 1. pop last cell from stack int current 
                    popStack(stack, mouseCell, stackPointer);
                    
                    // shows new mouse position during back tracking
                    toggleByte(maze, mouseCell, CELL_MOUSE);
                    printMaze(maze);
                    printMazeDebug(maze);
                    std::cout << std::endl;
                    waitForUserToContinue();	
                }
            }
            // all cells have been visited, a path does not exist
            if (!(mouseCell[CELL_ROW_INDEX] == endCell[CELL_ROW_INDEX] && mouseCell[CELL_COL_INDEX] == endCell[CELL_COL_INDEX])) {
                std::cout << "Mr. Nibbles could not solve the maze." << std::endl;
                std::cout << "Mr. Nibbles says well played!" << std::endl;
            } else  // a path is found
                std::cout << "Mr. Nibbles solved the maze." << std::endl;

        }


/**
 * Finds an existing neighbor that isn't blocked by a wall
 * @param cells - the grid cells in the maze
 * @param current - current location
 * @param neighbor - adjacent to current location
 */
void chooseNoWallNeighbor(BYTE cells[][MAZE_COLS], int current[], int neighbor[]) {
    bool found = false;
    while (!found) {
        int randNeighbor = rand() % 4;
        switch (randNeighbor) {
            
            // top
            case 0:
                    if (current[CELL_ROW_INDEX] > 0) {
                        if (!(cells[current[CELL_ROW_INDEX] - 1][current[CELL_COL_INDEX]] & CELL_VISITED)) {
                            if (!(cells[current[CELL_ROW_INDEX]][current[CELL_COL_INDEX]] & CELL_TOP)) {
                                std::cout << "Mr. Nibbles moved up" << std::endl;
                                copyOneLocTwo(current, neighbor);
                                neighbor[CELL_ROW_INDEX]--;
                                found = true;
                            }       
                        }
                    }
                    break;
                    
            // bottom
            case 1:
                    
                    if (current[CELL_ROW_INDEX] < MAZE_ROWS - 1) {
                        if (!(cells[current[CELL_ROW_INDEX] + 1][current[CELL_COL_INDEX]] & CELL_VISITED)) {
                            if (!(cells[current[CELL_ROW_INDEX]][current[CELL_COL_INDEX]] & CELL_BOT)) {
                                std::cout << "Mr. Nibbles moved down" << std::endl;
                                copyOneLocTwo(current, neighbor);
                                neighbor[CELL_ROW_INDEX]++;
                                found = true;
                            }
                        }       
                    }
                    break;
                    
            // left
            case 2:
                    if (current[CELL_COL_INDEX] > 0) {
                        if (!(cells[current[CELL_ROW_INDEX]][current[CELL_COL_INDEX] - 1] & CELL_VISITED)) {
                            if (!(cells[current[CELL_ROW_INDEX]][current[CELL_COL_INDEX]] & CELL_LEFT)) {
                                std::cout << "Mr. Nibbles moved left" << std::endl;
                                copyOneLocTwo(current, neighbor);
                                neighbor[CELL_COL_INDEX]--;
                                found = true;
                            }
                        }       
                    }
                    break;
                    
            // right
            case 3:
                    if (current[CELL_COL_INDEX] < MAZE_COLS - 1) {
                        if (!(cells[current[CELL_ROW_INDEX]][current[CELL_COL_INDEX] + 1] & CELL_VISITED)) {
                            if (!(cells[current[CELL_ROW_INDEX]][current[CELL_COL_INDEX]] & CELL_RIGHT)) {
                                std::cout << "Mr. Nibbles moved right" << std::endl;
                                copyOneLocTwo(current, neighbor);
                                neighbor[CELL_COL_INDEX]++;
                                found = true;
                            }
                        }       
                    }
                    break;

        }
    }
}

/**
 * Waits for user input to continue
 */
void waitForUserToContinue() {
    std::string pause;
    std::cout << "Press any key to continue" << std::endl;
    std::getline(std::cin, pause);   
}
