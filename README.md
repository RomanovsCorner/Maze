# Maze
Maze is a console based program that generates and solves mazes using a recursive backtracking algorithm.

#**Description**
This project contains two main procedures which are to generate and solve mazes. When generating a maze, the program defines a grid of cells from a given dimension. It also defines a "starting" and "ending" cell. The grid of cells is later used to represent the maze. Meanwhile, the edges of each cell represents a wall or an obstacle in the maze. After defining the grid, the program attempts to carve (remove the edges/walls) the entire maze based on a several conditions. The program starts at the "starting" cell and marks it as visited. The program is then thrown in a loop that repeats as long as the grid contains unvisited cells. From the "starting" cell the program attempts to find and choose at random a "neighbor" cell (a cell adjacent to the starting cell). Assuming a neighbor is found, the "starting" cell is pushed onto the a stack. The edge/wall that existed between the "starting" cell and the "neighbor" cell is removed. And the neighbor cell becomes our new "starting" cell and is also marked as visited. However, if a neighbor is not found (there is a dead-end) the program uses the recursive backtracking technique. It pops the stack to find the previous cell as an attempt to escape the dead-end. The algorithm is literally backtracking. This process is repeated until it finds a cell that does have a neighbor. The program flows into the loop again and these steps are repeated until the entire grid of cells is carved and thus representing the maze.
The recrusive backtracking algorithm is used to both generate and solve the maze. Both procedures are very similar. When attempting to solve the maze, the program terminates on two conditions as opposed to one. The program terminates when the "ending cell is found (There is a path and the maze has been solved.) or when there are no longer any univsited cells. (There isn't a path and the maze cannot be solved).

**Pseudocode:**
Init Maze
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
Solve Maze
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

**Note**: Maze was developed as a final project for my Data Structures & Algorithms course at Florida Gulf Coast University. This project was developed in C++ — an Object Oriented Programming language. However, an OOP approach was not taken as a way to learn more about bit-wise operators and user defined data structures. That is why they are used extensively and unecessarily throughtout this project. Unfortunately, the program is console based, which makes the maze generations and solutions a bit hard to follow. However, with the description and the pseudocode written above this can easily be reproduced as a GUI program to improve the aeastethic side of this project.

