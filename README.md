# MazeSolver

## Project Overview
MazeSolver is a project designed to explore maze solving algorithms by comparing sequential and parallel approaches. We utilize multiple particles, where each particle independently attempts to find a path from the start to the end of the maze.

## Getting Started

### Prerequisites
- Ensure you have `gcc` installed on your system (we've tested this on gcc-12.2).

### Compilation
Compile the program using the following command:
```bash
g++ main.cpp -o main -fopenmp
```

### Execute the Program

```bash
./main (size_of maze) (number_of_particles)
```

### Generic Instructions

Recommended size_of_maze<=101 to avoid long running times and potential memory over usage. Recommended number_of_particles can be 2x the size_of_maze  
  
Set the debug flag to true to see the maze and the path found
