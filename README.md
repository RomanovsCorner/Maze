# Maze
Maze is a console based program that generates and solves mazes using a recursive backtracking algorithm.

## Description
  >Maze contains three main procedures which initializes, generates, and solves mazes. The first procedure, defines a grid cell and a Stack. The grid cell is later used to represent the maze. Meanwhile, the edges of each cell represents a wall or an obstacle in the maze.

  >The second procedure, defines a "starting", "current", and "ending" cell which are three cells chosen from the grid cell. The program attempts to carve (remove the edges/walls) the entire maze based on a several conditions. Firstly, the "current" cell is assigned to "starting" cell and is marked as visited. The program is then thrown in a loop that repeats as long as the grid contains unvisited cells. From the "current" cell the program attempts to find and choose a random "neighbor" cell (a cell adjacent to the current cell). Assuming a neighbor is found, the "current" cell is pushed onto the a Stack. The edge/wall that existed between the "current" and "neighbor" cell is removed. The "neighbor" cell becomes our new "current" cell and is also marked as visited. Then this routine is repeated. However, if a neighbor is not found (there is a dead-end) the program uses the recursive backtracking technique. The Stack is popped to find the previous cell that does have a neighbor as an attempt to escape the dead-end. This is a literal backtracking. This process is repeated until a cell that has a neighbor is found. The program flows into the loop again and these steps are repeated until the entire grid of cells is carved and thus representing a maze.
  
  >The third procedure is very similar to the second one. When attempting to solve the maze, the program terminates on two conditions as opposed to one. The program terminates when the "ending cell is found (There is a path and the maze has been solved.) or when there are no longer any univsited cells. (There isn't a path and the maze cannot be solved).

## Pseudocode
```diff
- Init Maze
1.Define a grid (ie. 3 x 3)
2.Define a Stack

  Generate Maze 
1.Define "starting", "ending", "current" cell
2.Assign "current" cell to "starting" cell
3.while there are univisited cells
  i.if "current" cell has neighbor
      push "current" cell onto the Stack
      assign "current" cell to "neighbor" cell
      set "current" cell to visited
  ii.else if stack is not empty
      pop stack to get "previous" cell
      set "current" cell to "previous"

- Solve Maze
1.Define "starting", "ending", "current" cell
2.Assign "current" cell to "starting" cell
3.while there are univisited cells
  i.if "current" cell is equal to "ending" cell
      exit while loop
  ii.if "current" cell has neighbor
      push "current" cell onto the Stack
      assign "current" cell to "neighbor" cell
      set "current" cell to visited
  iii.else if stack is not empty
      pop stack to get "previous" cell
      set "current" cell to "previous"
 4.if "current" cell is equal to "ending" cell
      path found
```
**Note**: Maze was developed as a final project for my Programming Methodology course taken at Florida Gulf Coast University. This project is developed in C++ — an Object Oriented Programming language. However, during development, an OOP approach was not taken as a strategy to learn more about bit-wise operators and user defined data structures. That is why they are used extensively and unecessarily throughtout this project. Unfortunately, this program is console based, which makes the maze generations and solutions a bit hard to follow. However, with the description and the pseudocode written above this can easily be reproduced as a GUI program to improve the aeastethic side of this project.

