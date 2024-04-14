#include <iostream>
#include <stdlib.h>
#include "./MazeGenerator/mazegenerator.cpp"
#include "./MazeSolver/sequential.cpp"
#include "./MazeSolver/parallel.cpp"
using namespace std;

int main(int argc, char *argv[]){
    if(argc<3){
        printf("You must provide the size(odd number) of maze and the type of execution s or p\n");
        exit(0);
    }
    int size = atoi(argv[1]);
    char execution = argv[2][0];
    if(size%2==0){
        size++;
        printf("To maintain symmetry, maze size is generally always odd. Creating maze of size %d\n",size);
    }
    vector<vector<char> > maze(size, vector<char> (size));
    mazegenerator(maze,size,true);
    if (execution == 's')
        sequential(maze,size,true);
    else if (execution == 'p')
        parallel(maze,size,true);
    else
        printf("choose either s (for sequential) or p (for parallel)\n");
}