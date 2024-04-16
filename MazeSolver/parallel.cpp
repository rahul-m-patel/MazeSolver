#include <iostream>
#include <stdio.h>
#include <vector>
#include <random>
#include <omp.h>
using namespace std;

vector<char> news = {'N','S','W','E'};
vector<pair<int, int> > directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

bool isValid(int &row, int &col, int &size){
    return row>=0 && row<size && col>=0 && col<size;
}

struct p_Particles{
    int n_particles;
    vector<pair<int,int>> position;
    vector<vector<pair<int,int>>> path;
    vector<char> moving_direction;
    p_Particles(int n_particles) : n_particles(n_particles), position(vector<pair<int,int>>(n_particles)), path(vector<vector<pair<int,int>>>(n_particles)), moving_direction(vector<char> (n_particles,'F')) {}

    // add particle to a partcular cell
    void addParticle(int index, int x, int y){
        this->position[index] = {x,y};
        this->path[index].push_back({x,y});
    }

    void update_coordinates(int index, char dir, bool backtrack = false){
        // update coordinates
        for(int i=0;i<4;++i){
            if(dir==news[i]){
                this->position[index] = {this->position[index].first+directions[i].first, this->position[index].second+directions[i].second};
                break;
            }
        }
        int path_size = this->path[index].size();
        // if particle came back to where it was one step back
        if(backtrack || path_size>1 && this->position[index].first == this->path[index][path_size-2].first && this->position[index].second == this->path[index][path_size-2].second){
            this->path[index].pop_back();
        }
        else{ //else push current position
            this->path[index].push_back(this->position[index]);
        }
        // set moving direction
        this->moving_direction[index] = dir;
    }
};

// get next set of possible moves from current cell
vector<char> p_get_possible_moves(vector<vector<char>> &maze, int &size, pair<int,int> pos){
    vector<char> moves;
    int x = pos.first;
    int y = pos.second;
    for(int i=0;i<4;++i){
        int new_x = x + directions[i].first;
        int new_y = y + directions[i].second;
        if(isValid(new_x,new_y,size) && maze[new_x][new_y]!=WALL){
            moves.push_back(news[i]);
        }

    }
    return moves;
}


// get direction
char p_get_next_move(pair<int,int> &curr, int &new_x, int &new_y){
    int x = curr.first;
    int y = curr.second;
    return x==new_x?(new_y>y?'E':'W'):(new_x>x?'S':'N');
}

// backtrack remaining particles
void p_backtrack_remaining_particles(vector<vector<char>> &maze, int &size, int &startRow, int &startCol, p_Particles &particles, vector<pair<int,int> > &exited_particle_path, int &exited_particle){
    int n_particles = particles.n_particles;
    vector<bool> particles_on_track(n_particles,false);
    vector<bool> exited_particles(n_particles,false);
    particles_on_track[exited_particle] = true;
    exited_particles[exited_particle] = true;
    int n_exited_particles = 1;
    while(n_exited_particles<n_particles){
        #pragma omp parallel for
        for(int index = 0; index <n_particles; index++){
            if(!exited_particles[index]){
                if(!particles_on_track[index]){
                    for(int path_index = 0; path_index<exited_particle_path.size();path_index++){
                        int x = exited_particle_path[path_index].first;
                        int y = exited_particle_path[path_index].second;
                        if(particles.position[index].first == x && particles.position[index].second == y){
                            particles_on_track[index] = true;
                            particles.path[index].clear();
                            // we store the path backwards because till we get on the correct path, we backtrack (i.e look at the last moved coordinate)
                            for(int i=exited_particle_path.size()-1;i>path_index;i--){
                                particles.path[index].push_back(exited_particle_path[i]);
                            }
                            break;
                        }
                    }
                }
                if(!particles.path[index].empty()){
                    int next_x = particles.path[index].back().first;
                    int next_y = particles.path[index].back().second;
                    if(next_x == particles.position[index].first && next_y == particles.position[index].second){
                        particles.path[index].pop_back();
                        next_x = particles.path[index].back().first;
                        next_y = particles.path[index].back().second;
                    }
                    particles.update_coordinates(index,p_get_next_move(particles.position[index],next_x,next_y), true);
                }
                else if(!exited_particles[index]){
                    #pragma omp critical
                    n_exited_particles+=1;
                    exited_particles[index] = true;
                }
            }
            else{
                //for debugging
            }
        }
    }
}

void p_solve_maze(vector<vector<char>> &maze, int &size, int &startRow, int &startCol, p_Particles &particles, mt19937 &rng){
    bool maze_solved = false;
    int exited_particle = -1;

    while(!maze_solved){
        #pragma omp parallel for
        for(int i=0;i<particles.n_particles;++i){
            if(!maze_solved){
                vector<char> moves = p_get_possible_moves(maze,size,particles.position[i]);
                bool same_direction_move = false;
                if(moves.size()==2){
                    for(int j=0;j<2;++j){
                        if(particles.moving_direction[i] == moves[j]){
                            same_direction_move = true;
                            particles.update_coordinates(i,moves[j]);
                            break;
                        }
                    }
                }
                if(!same_direction_move){
                    uniform_int_distribution<int> uniform_dist(0,moves.size()-1);
                    char new_move = moves[uniform_dist(rng)];
                    particles.update_coordinates(i,new_move);
                }
                if(maze[particles.position[i].first][particles.position[i].second]==EXIT){
                    #pragma omp critical
                    {
                        exited_particle = i;
                        maze_solved = true;
                    }
                }
            }
        }
    }

    vector<pair<int,int> > exited_particle_path = particles.path[exited_particle];

    #pragma parallel for if(exited_particle_path.size()/omp_get_max_threads()>100)
    for(auto &it:exited_particle_path){
        maze[it.first][it.second] = SOL;
    }
    maze[startRow][startCol] = START;
    printf("Exit reached by particle %d.\n",exited_particle);
}

void n_particles_parallel(vector<vector<char>> &maze, int &size, int &n_particles, int debug){
    p_Particles particles(n_particles);
    int startRow = 0, startCol = 1;
    random_device rd;  // Obtain a random seed from the operating system
    mt19937 rng(rd());

    #pragma omp parallel for
    for(int i=0;i<n_particles;++i){
        particles.addParticle(i,startRow,startCol);
    }

    printf("Solving the maze parallely with %d particles...\n",n_particles);

    p_solve_maze(maze,size,startRow,startCol,particles,rng);
    if(debug){
        printf("Solution with %d particles\n",n_particles);
        print_maze(maze,size);
    }
}