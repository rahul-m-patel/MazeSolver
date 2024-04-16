#include <iostream>
#include <stdlib.h>
#include "./MazeGenerator/mazegenerator.cpp"
#include "./MazeSolver/sequential.cpp"
#include "./MazeSolver/parallel.cpp"
#include <omp.h>
using namespace std;

int main(int argc, char *argv[]){
    // Check if arguments are correct
    if(argc < 3){
        printf("Usage: %s <size_of_maze> <no_of_particles>\n",argv[0]);
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
    //pass the maze, size and debug flag
    mazegenerator(maze, size, false);

    vector<vector<char>> mazeForSequential = maze;
    vector<vector<char>> mazeForParallel = maze;
    double start;
    double end;
    int particles = atoi(argv[2]);
    start = omp_get_wtime();  
    n_particles_sequential(mazeForSequential, size, particles, false);
    end = omp_get_wtime();
    printf("Execution time for sequential code with %d particles is %f seconds\n",particles,end-start);

    start = omp_get_wtime();  
    n_particles_parallel(mazeForParallel,size,particles,false);
    end = omp_get_wtime();
    printf("Execution time for parallel code with %d particles is %f seconds\n",particles,end-start);

    return 0;
}