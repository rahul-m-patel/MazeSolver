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
