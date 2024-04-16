#include <random>
#include <iostream>
#include <stdlib.h>
#include <cstdio>
#include <vector>

using namespace std;

// Definition of possible moves
enum MOVES {
    N,
    U,
    R,
    D,
    L,
};

// // Function to display the maze
// void print_maze(vector<vector<char>> &maze, int &size){
//     for(int i=0;i<size;++i){
//         for(int j=0;j<size;++j){
//             printf("%c ",maze[i][j]);
//         }
//         printf("\n");
//     }
//     printf("\n");
// }

// Structure to manage particle information
struct Particles{
    int n_particles;
    vector<pair<int,int>> position;
    vector<vector<pair<int,int>>> path;
    vector<MOVES> moves;
    Particles(int n_particles) : n_particles(n_particles), position(vector<pair<int,int>>(n_particles)), path(vector<vector<pair<int,int>>>(n_particles)), moves(vector<MOVES>(n_particles)) {}

    void addParticle(int index, int x, int y, MOVES move = MOVES::N){
        this->position[index] = make_pair(x,y);
        this->path[index].push_back(make_pair(x,y));
        this->moves[index] = move;
    }
};

// Calculates new coordinates based on the move direction
pair<int,int> get_updated_coordinates(MOVES new_move, pair<int,int> position) {
    switch (new_move) {
        case MOVES::U:
            return make_pair(position.first - 1, position.second);
        case MOVES::D:
            return make_pair(position.first + 1, position.second);
        case MOVES::R:
            return make_pair(position.first, position.second + 1);
        case MOVES::L:
            return make_pair(position.first, position.second - 1);
        case MOVES::N:
            cout << "Unexpected error: the N move is meant only for particle initialization!" << endl;
            return {-1,-1};
        
    }
    return {-1,-1};
}

// Determines available moves from a given position within the maze
vector<MOVES> get_next_moves(vector<vector<char>> &maze, int &size, pair<int,int> position) {
    vector<MOVES> moves;
    int row = position.first;
    int col = position.second;
    if (-1 < (row-1) && (row-1) < size && maze[row - 1][col] != '#') 
        moves.push_back(MOVES::U);

    if(-1 < (row+1) && (row+1) < size && maze[row + 1][col] != '#')
        moves.push_back(MOVES::D);

    if(-1 < (col-1) && (col-1) < size && maze[row][col - 1] != '#')
        moves.push_back(MOVES::L);

    if(-1 < (col+1) && (col+1) < size && maze[row][col + 1] != '#')
        moves.push_back(MOVES::R);
    return moves;
}

// Identifies the move needed to transition from one coordinate to another
MOVES determine_move_from_position(pair<int,int> &position, pair<int,int> &next_coords) {
    if(next_coords.first == position.first) {
        if(next_coords.second > position.second)
            return MOVES::R;
        else
            return MOVES::L;
    } else {
        if(next_coords.first > position.first)
            return MOVES::D;
        else
            return MOVES::U;
    }
}

// Retraces steps for particles that exited to find the successful path
void backtrack_exited_particle(vector<vector<char>> &maze, vector<vector<char>> &maze_copy, pair<int,int> &initial_position, int &size, Particles &particles, vector<pair<int,int>>& exited_particle_path, int exited_particle_index, bool debug) {
    int n_particles = particles.n_particles;
    int n_particles_copy = n_particles;
    vector<bool> particles_on_track_map;
    vector<bool> exited_particles_map;

    // Initialize tracking maps to false
    for(int index = 0; index < n_particles_copy; index++) {
        particles_on_track_map.push_back(false);
        exited_particles_map.push_back(false);
    }

    particles_on_track_map[exited_particle_index] = true;
    exited_particles_map[exited_particle_index] = true;

    int n_exited_particles = 1;
    maze_copy.clear();

    while(n_exited_particles < n_particles) {
        if(debug) {
            // Reset maze to show step-by-step tracking
            maze_copy = maze;
            maze_copy[initial_position.first][initial_position.second] = 'S';
        }

        // Retracing steps and aligning particle paths with the successful path
        for(int particle_index = 0; particle_index < n_particles_copy; particle_index++) {
            if(!exited_particles_map[particle_index]) {
                if(!particles_on_track_map[particle_index]) {
                    for(int path_index = 0; path_index < static_cast<int>(exited_particle_path.size()); path_index++) {
                        pair<int,int> coord = exited_particle_path[path_index];
                        if(particles.position[particle_index].first == coord.first && particles.position[particle_index].second == coord.second) {
                            particles_on_track_map[particle_index] = true;
                            particles.path[particle_index].clear();
                            int remaining_path_size = static_cast<int>(exited_particle_path.size());
                            particles.path[particle_index].reserve(remaining_path_size);

                            // Assign remaining steps in reverse order
                            for(int index = remaining_path_size - 1; index > path_index; index--) {
                                particles.path[particle_index].push_back(exited_particle_path[index]);
                            }
                            break;
                        }
                    }
                }

                // Execute particle steps backward
                if(!particles.path[particle_index].empty()) {
                    pair<int,int> next_coords = particles.path[particle_index].back();
                    if(next_coords.first == particles.position[particle_index].first && next_coords.second == particles.position[particle_index].second) {
                        particles.path[particle_index].pop_back();
                        next_coords = particles.path[particle_index].back();
                    }
                    MOVES move = determine_move_from_position(particles.position[particle_index], next_coords);
                    pair<int, int> position = get_updated_coordinates(move, particles.position[particle_index]);
                   
                    particles.path[particle_index].pop_back();
                    particles.moves[particle_index] = move;
                    particles.position[particle_index] = position;

                    if(debug) {
                        maze_copy[position.first][position.second] = 'o';
                    }
                } else if (!exited_particles_map[particle_index]){
                    n_exited_particles += 1;
                    exited_particles_map[particle_index] = true;
                }
            } else {
                if(debug) {
                    maze_copy[particles.position[particle_index].first][particles.position[particle_index].second] = 'o';
                }
            }
        }

        if(debug)
            print_maze(maze_copy, size); 
    }
}

// Randomly moves particles to find an exit, displaying debug info if enabled
vector<vector<char>> navigate_maze_randomly(vector<vector<char>> &maze, int &size, pair<int,int> initial_position, Particles &particles, bool debug) {
    bool exit_reached = false;
    int exited_particle_index = -1;
    vector<vector<char>> maze_copy;
    std::random_device rd;

    while(!exit_reached) {
        if(debug){
            maze_copy = maze;
        }
        for(int index = 0; index < particles.n_particles; index++) {
            vector<MOVES> moves = get_next_moves(maze, size, particles.position[index]);
            bool same_move_allowed = false;
            for(MOVES move : moves) {
                if(moves.size() == 2 && particles.moves[index] == move) {
                    same_move_allowed = true;
                    pair<int, int> position = get_updated_coordinates(move, particles.position[index]);
                    int path_size = static_cast<int>(particles.path[index].size());
                    if((path_size > 1 && position.first == particles.path[index][path_size - 2].first && position.second == particles.path[index][path_size - 2].second)){
                        particles.path[index].pop_back();
                        particles.moves[index] = move;
                        particles.position[index] = position;
                    }
                    else
                        particles.addParticle(index,position.first, position.second, move);
                    break;
                }
            }
            if(!same_move_allowed) {
                std::uniform_int_distribution<int> uniform_dist(0, static_cast<int>(moves.size()) - 1);
                long processed_seed = rd();
                std::mt19937 rng(processed_seed);
                MOVES new_move = moves[uniform_dist(rng)];
                pair<int, int> position = get_updated_coordinates(new_move, particles.position[index]);
                particles.addParticle(index,position.first, position.second, new_move);
            }

            pair<int,int> curr_pos = particles.position[index];
            if(debug) {
                maze_copy[curr_pos.first][curr_pos.second] = 'o';
                maze_copy[initial_position.first][initial_position.second] = 'S';
            }

            if(maze[curr_pos.first][curr_pos.second] == 'E') {
                exited_particle_index = index;
                exit_reached = true;
                break;
            }
        }

        if(debug && !maze_copy.empty())
            print_maze(maze_copy, size);

    }

    vector<pair<int,int>> exited_particle_path = particles.path[exited_particle_index];

    for(pair<int,int> coord : exited_particle_path) {
        maze[

coord.first][coord.second] = 'o';
    }
    maze[initial_position.first][initial_position.second] = 'S';

    if(debug){
        print_maze(maze, size);
    }

    printf("Exit reached by particle %d\n",exited_particle_index);
    //cout << "Starting to backtrack the remaining particles" << endl;

    //backtrack_exited_particle(maze, maze_copy, initial_position, size, particles, exited_particle_path, exited_particle_index, debug);


    //cout << "All particles have reached the exit!" << endl;

    return maze;
}


// Sequentially solves the maze with multiple particles, initializing from a common start point
vector<vector<char>> n_particles_sequential(vector<vector<char>> &maze, int &size, int &n_particles, bool debug) {
    pair<int,int> initial_position = make_pair(0,1); // The start position is always (0,1)

    Particles particles(n_particles);
    for(int i = 0; i < n_particles; i++) {
        particles.addParticle(i,initial_position.first,initial_position.second);
    }

    printf("Solving the maze sequentially with %d particles...\n",n_particles);

    return navigate_maze_randomly(maze, size, initial_position, particles, debug);
}


// int main() {
//     int size = 5;
//     vector<vector<char>> maze = {
//         {'#', ' ', ' ', '#', ' '},
//         {'#', '#', ' ', '#', ' '},
//         {' ', ' ', ' ', ' ', ' '},
//         {' ', '#', '#', '#', ' '},
//         {' ', ' ', ' ', 'E', '#'}
//     };

//     int debug = 0;
//     int particles = 10;
//     n_particles_sequential(maze, size, particles, debug);

//     return 0;
// }
