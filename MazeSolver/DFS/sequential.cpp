#include <iostream>
#include <stdio.h>
#include <vector>
#include <random>
using namespace std;

vector<pair<int, int> > directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

bool isValid(int &row, int &col, int &size){
    return row>=0 && row<size && col>=0 && col<size;
}

bool DFS(vector<vector<char> > &maze, int &size, int &x, int &y, vector<vector<bool> > &visited, vector<pair<int,int> > &path){
    if(maze[x][y]=='E'){
        return true;
    }

    visited[x][y] = true;

    for(auto dir:directions){
        int newRow = x+dir.first;
        int newCol = y+dir.second;

        if(isValid(newRow,newCol,size) && !visited[newRow][newCol] && maze[newRow][newCol]!=WALL){
            path.push_back({x,y});
            if(DFS(maze,size,newRow,newCol,visited,path)){
                return true;
            }
            path.pop_back();
        }
    }
    return false;
}

void sequential(vector<vector<char> > &maze, int &size, int debug){
    vector<vector<bool> > visited(size, vector<bool> (size,false));
    vector<pair<int,int> > path;
    int startRow = 0, startCol = 1;
    DFS(maze,size,startRow,startCol,visited,path);
    for(int i=1;i<path.size();++i){
        maze[path[i].first][path[i].second] = SOL;
    }
    if(debug){
        printf("Sequential solution...\n");
        print_maze(maze,size);
    }
}