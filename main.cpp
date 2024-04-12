#include <iostream>
#include <stdlib.h>
#include "./MazeGenerator/mazegenerator.cpp"
#include "./MazeSolver/sequential.cpp"
using namespace std;

int main(int argc, char *argv[]){
    if(argc<2){
        printf("You must provide the size(odd number) of maze\n");
        exit(0);
    }
    int size = atoi(argv[1]);
    if(size%2==0){
        size++;
        printf("To maintain symmetry, maze size is generally always odd. Creating maze of size %d\n",size);
    }
    vector<vector<char> > maze(size, vector<char> (size));
    mazegenerator(maze,size,false);
    sequential(maze,size,true);
}