#include <iostream>
#include <stdlib.h>
#include "./MazeGenerator/mazegenerator.cpp"
#include "./MazeSolver/sequential.cpp"
#include "./MazeSolver/parallel.cpp"
using namespace std;

int main(int argc, char *argv[]){
    // Check if at least the size of the maze is provided
    if(argc < 2){
        printf("You must provide the size (odd number) of the maze and optionally the type of execution (s or p).\n");
        exit(0);
    }

    // Parse the maze size from the first argument
    int size = atoi(argv[1]);
    // Ensure the maze size is odd
    if(size % 2 == 0){
        size++;
        printf("To maintain symmetry, maze size is generally always odd. Creating maze of size %d\n", size);
    }

    // Generate the maze
    vector<vector<char>> maze(size, vector<char>(size));
    mazegenerator(maze, size, true);

    // Check if the execution type is provided
    char execution = (argc > 2) ? argv[2][0] : '\0';

    // Execute based on the provided type or default to both
    if (execution == 's') {
        sequential(maze, size, true);
    } else if (execution == 'p') {
        parallel(maze, size, true);
    } else {
        // No valid execution type provided, execute both by default
        vector<vector<char>> mazeForSequential = maze;
        vector<vector<char>> mazeForParallel = maze;
        sequential(mazeForSequential, size, true);
        parallel(mazeForParallel, size, true);
    }
    
    return 0;
}