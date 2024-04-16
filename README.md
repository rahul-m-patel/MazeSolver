# MazeSolver
This project aims to solve a maze comparing sequential and parallel code. We use n particles to solve this problem, where each particle tries to find path from start to end independent of each other  
  
To execute this code:  
Compile: We have tested our code on gcc-12.2  
g++ main.cpp -o main -fopenmp  
  
Run the program:  
./main (size_of maze) (number_of_particles)
