/*
At the start of your simulation every cell starts with a value of 
zero.   At the first frame in the middle of the leftmost edge 
(position 0, 49 if 0,0 is bottom left) spikes to a value of 100 000.  
*/

#include <numeric>
#include <iostream>
#include <stdio.h>          
#include <stdlib.h>     
#include <cmath>
#include <omp.h>    
using namespace std;
int main () {
    
    // size of matrix
    int SIZE = 20;     

    //create 2 matrices - one to store temp calculations and values for each iteration
    double** matrix_2d = new double*[SIZE];
    double** temp_matrix = new double*[SIZE];


    //initialize matrix 
    //generating 5x5 matrix of value 0
    for (int i = 0; i < SIZE; i++){
        matrix_2d[i] = new double[SIZE];
        temp_matrix[i] = new double[SIZE];

        for(int j = 0; j< SIZE; j++){
            matrix_2d[i][j] = 0.0;
            temp_matrix[i][j] = 0.0;
        }
    }
    
    // Starting simulation
    // First frame, (0,49) gets value 100000
    int mid_col = (int)(SIZE/2.0); 
    matrix_2d[0][mid_col] = 400;

    //print matrix first frame
    std::cout << "First Frame Matrix: \n" << std::endl;
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j< SIZE; j++){
            std::cout << matrix_2d[i][j] << " ";
        }
        std::cout << "\n";
    }

    /* ------- Simulation start -------- */

    // boolean value to store if simulation has completed
    // Simulation is completed when all cell values are <=12
    int completed = 0;
    // Keep count of iterations of the loop done to reach the required state
    int iterations = 0;
    // sum of vlaues of neighbors to calculate average
    double sum_neighbour = 0.0;
    // numer of neighbours to calculate average
    double count_neightbour = 0.0;

    // Continue till the boolean value is false
    
    while(completed == 0){
        // copy values into temp matrix for calculations
        
        for(int i = 0; i < SIZE; i++){
            for(int j = 0; j< SIZE; j++){
                temp_matrix[i][j] = matrix_2d[i][j];
            }
        }
        // traverse through matrix and get each cell value to perform operation
        #pragma omp parallel shared(temp_matrix) 
        for(int i = 0; i < SIZE; i++){
            for(int j = 0; j< SIZE; j++){
                sum_neighbour = 0.0;
                count_neightbour = 0.0;
                
                // get neighboring cells
                 for(int x = i-1; x <= i+1; x++){
                    // make sure within matrix
                    if(x<0 || x==SIZE) continue;

                    for(int y = j-1; y <= j+1; y++){
                        // make sure within matrix
                        if(y<0 || y== SIZE) continue;
                        if(x == i && y == j) continue; // exclude the cell itself
                        
                        sum_neighbour+=matrix_2d[x][y];
                        count_neightbour+=1.0;
                    }
                }


                // calculate the avg value of neighbour sum
                double avg = 0.0;
                if(count_neightbour !=0.0 ){
                    avg = sum_neighbour/count_neightbour;
                }
               
                // calculate the value to be gien to neighbors
                double diff = 0.05 * (matrix_2d[i][j] - avg);
                

                // giving cells new values
                // operations performed on temp matrix
                for(int x = i-1; x <= i+1; x++){
                    // make sure within matrix
                    if(x<0 || x>=SIZE) continue;

                    for(int y = j-1; y <= j+1; y++){
                        // make sure within matrix
                        if(y<0 || y>= SIZE) continue;
                        if(x == i && y == j) continue; // exclude the cell itself

                        // if neighbour cell has less value than current cell
                        if(matrix_2d[i][j] >= temp_matrix[x][y]){
                            if((temp_matrix[i][j]-=diff)>=12){
                                temp_matrix[x][y] += diff;
                                temp_matrix[i][j] -= diff;
                            }
                        }
                        // else take the value from neighbor cell
                        else{
                            if((temp_matrix[x][y]-=diff)>=12){
                                temp_matrix[x][y] -= diff;
                                temp_matrix[i][j] += diff;
                            }
                        }
                    }
                }
            }
        }

        // copy new values to the main matrix
        for(int i = 0; i < SIZE; i++){
            for(int j = 0; j< SIZE; j++){
                matrix_2d[i][j] = temp_matrix[i][j];
            }
        }

        // check if the values of each cell is <=12, then simulation completed
        completed = 1;
        for(int i = 0; i < SIZE; i++){
            for(int j = 0; j< SIZE; j++){
                if(matrix_2d[i][j] >2){
                    completed = 0;
                    break;
                }
            }
            if(completed == 0) break;
        }

        for(int i = 0; i < SIZE; i++){
            for(int j = 0; j< SIZE; j++){
                if(isnan(matrix_2d[i][j])){
                    completed = 1;
                    break;
                }
            }
            if(completed == 1) break;
        }
        

        iterations++;
    }

    // print final matrix
    std::cout << "After Simulation, Matrix: \n" << std::endl;
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j< SIZE; j++){
            std::cout << matrix_2d[i][j] << "\t\t";
        }
        std::cout << "\n";
    }

    std::cout << "Number of Iterations done: " << iterations << "\n" << std::endl;
    return 0; 
}
