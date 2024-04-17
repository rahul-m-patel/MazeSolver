# MazeSolver
This project aims to solve a maze comparing sequential and parallel code. We use n particles to solve this problem, where each particle tries to find path from start to end independent of each other  
  
To execute this code:  
Compile: We have tested our code on gcc-12.2  
g++ main.cpp -o main -fopenmp  
  
Run the program:  
./main (size_of_maze) (number_of_particles)  
  
Recommended size_of_maze<=101 to avoid long running times and potential memory over usage. Recommended number_of_particles can be 2x the size_of_maze  
  
Set the debug flag to true to see the maze and the path found
