#include <iostream>
#include <stdio.h>
#include <vector>
#include <random>
#include <omp.h>
using namespace std;


vector<pair<int, int>> directionsp = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

bool isValidp(int &row, int &col, int &size){
    return row >= 0 && row < size && col >= 0 && col < size;
}

bool parallelDFS(vector<vector<char>> &maze, int &size, int x, int y, vector<vector<bool>> &visited, vector<pair<int,int>> &path){
    if (maze[x][y] == 'E') {
        path.push_back({x, y});
        return true;
    }

    visited[x][y] = true;
    bool found = false;

    #pragma omp parallel for shared(maze, visited, path, found)
    for (int i = 0; i < directionsp.size(); i++) {
        if (found) continue;

        auto dir = directionsp[i];
        int newRow = x + dir.first;
        int newCol = y + dir.second;

        if (isValidp(newRow, newCol, size) && !visited[newRow][newCol] && maze[newRow][newCol] != WALL) {
            vector<pair<int,int>> localPath(path); // Create a local copy of the path for this thread
            localPath.push_back({x, y});

            if (parallelDFS(maze, size, newRow, newCol, visited, localPath)) {
                #pragma omp critical
                {
                    if (!found) { // Check again within the critical section
                        path = localPath; // Only update the global path if it's the first found solution
                        found = true;
                    }
                }
            }
        }
    }

    return found;
}

void parallel(vector<vector<char>> &maze, int size, int debug){
    vector<vector<bool>> visited(size, vector<bool>(size, false));
    vector<pair<int,int>> path;

    if (parallelDFS(maze, size, 0, 1, visited, path)) { // Assuming the start is at (0, 1)
        for (auto& p : path) {
            if (maze[p.first][p.second] == ' ')
                maze[p.first][p.second] = SOL;
        }
    }

    if (debug) {
        cout << "Parallel solution..." << endl;
        print_maze(maze, size);
    }
}
