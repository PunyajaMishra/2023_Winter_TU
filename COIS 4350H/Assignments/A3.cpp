#include <numeric>
#include <iostream>
#include <stdio.h>          
#include <stdlib.h>     
#include <cmath>
#include <mpi.h>

int main(int argc, char** argv){
    // Initialize MPI environment
    MPI_Init(&argc, &argv);
    int world_size, world_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // size of matrix
    int SIZE = 100;     
    double MINIMUM = 12.0;

    // calculate chunk size
    int chunk_size = SIZE / 2;
    if(SIZE % 2 != 0){
        std::cerr << "Error: Size of matrix not evenly divisible by 2" << std::endl;
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // create 2 matrices - one to store temp calculations and values for each iteration
    double** matrix = new double*[SIZE];
    double** tempM = new double*[SIZE];


     // initialize matrix 
    // generating SIZE x SIZE matrix of value 0
    for (int i = 0; i < SIZE; i++){
        matrix[i] = new double[SIZE];
        tempM[i] = new double[SIZE];

        for(int j = 0; j< SIZE; j++){
            matrix[i][j] = 0.0;
            tempM[i][j] = 0.0;
        }
    }

     // Starting simulation
    // First frame, (0,49) gets value 100000
    int mid_col = (int)(SIZE/2.0); 
    matrix[0][mid_col] = SIZE*SIZE;

    //print matrix first frame
    std::cout << "First Frame Matrix: \n" << std::endl;
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j< SIZE; j++){
            std::cout << matrix[i][j] << " ";
        }
        std::cout << "\n";
    }

    // dividing the matrix into 4 parts as squares
    int row_start, row_end, col_start, col_end;
    if(world_rank == 0){
        row_start = 0; row_end = chunk_size - 1;
        col_start = 0; col_end = chunk_size - 1;
    }else if(world_rank == 1){
        row_start = 0; row_end = chunk_size - 1;
        col_start = chunk_size; col_end = SIZE - 1;
    }else if(world_rank == 2){
        row_start = chunk_size; row_end = SIZE - 1;
        col_start = 0; col_end = chunk_size - 1;
    }else{
        row_start = chunk_size; row_end = SIZE - 1;
        col_start = chunk_size; col_end = SIZE - 1;
    }

    // distribute the matrix among processes
    double* sendbuf = new double[chunk_size*chunk_size];
    double* recvbuf = new double[chunk_size*chunk_size];

    // initialize the send buffer
    int count = 0;
    for(int i = row_start; i <= row_end; i++){
        for(int j = col_start; j <= col_end; j++){
            sendbuf[count] = matrix[i][j];
            count++;
        }
    }

    // boolean value to store if simulation has completed
    // Simulation is completed when all cell values are <=12
    int completed = 0;
    // Keep count of iterations of the loop done to reach the required state
    int iterations = 0;
    // sum of values of neighbors to calculate average
    double sum_neighbor = 0.0;
    // number of neighbors to calculate average
    double count_neighbor = 0.0;   

    while(completed!=1){
        // copy all values from previous interation to temp matrix
        // this is where the new calculated value will be stored
        for(int i = 0; i < SIZE; i++){
            for(int j = 0; j< SIZE; j++){
                tempM[i][j] = matrix[i][j];
            }
        }

        // program should iterate through each cell in main matrix
        for(int i = 0; i < SIZE; i++){
            for(int j = 0; j< SIZE; j++){
                sum_neighbor = 0.0;
                count_neighbor = 0.0;

                // For each cell, count the number of neightbors and sum of neighbors
                // calculate the 5% of difference of avg and cell value
                for(int x = i-1; x <= i+1; x++){
                    // make sure within matrix
                    if(x<0 || x==SIZE) continue;                        
                    for(int y = j-1; y <= j+1; y++){
                        // make sure within matrix
                        if(y<0 || y== SIZE) continue;
                        if(x == i && y == j) continue; // exclude the cell itself
                        sum_neighbor+=matrix[x][y];
                        count_neighbor+=1.0;
                    }
                }

                // calculate the avg value of neighbour sum
                double avg = 0.0;
                if(count_neighbor !=0.0 ){
                    avg = sum_neighbor/count_neighbor;
                }
                
                // calculate the value to be gien to neighbors
                //std::cout << "----CELL---: " << i << "," <<j << "\n";
                double diff = 0.05 * (matrix[i][j] - avg);
                if(diff<0) {diff = diff * (-1);} //take absolute value
                //std::cout << "diff: " << diff << "\n";
                
                // store the cell's value in a variable 
                double curr_cell = matrix[i][j];
                //std::cout << "curr_cell: " << matrix[i][j] << "\n";
                

                // loop through each neighbor 
                for(int x = i-1; x <= i+1; x++){
                    // make sure within matrix
                    if(x<0 || x==SIZE) continue;                        
                    for(int y = j-1; y <= j+1; y++){
                        // make sure within matrix
                        if(y<0 || y== SIZE) continue;
                        if(x == i && y == j) continue; // exclude the cell itself

                        // then either decrease or increase value accordingly
                        // store the new value in tempM  
                        // make sure value is not <=12    
                        
                        if(curr_cell >= matrix[x][y]){
                            // neighbor cell value is less than the current cell value
                            //std::cout << "matrix[x][y]: " << matrix[x][y] << "\n";
                            if((curr_cell-diff) < 0) {
                                //std::cout << "curr_cell-diff: " << curr_cell-diff << "\n";
                                continue;
                            }
                            else{
                                tempM[x][y] += diff;
                                curr_cell -= diff;
                                //std::cout << "temp[x][y]: " << tempM[x][y] << "\n";
                                //std::cout << "curr_cell: " << curr_cell << "\n";
                            }
                        }
                        else{
                            if((matrix[x][y]-diff)>=0){
                                tempM[x][y] -= diff; 
                                curr_cell += diff;
                                //std::cout << "temp[x][y]: " << tempM[x][y] << "\n";
                                //std::cout << "curr_cell: " << curr_cell << "\n";
                            }
                        }
                    }
                }
                // after transferring of all values is done now transfer the main cell's values
                tempM[i][j] = curr_cell;
                
            // cell loop 
            }
        }


        // copy all new caluclated values to the main matrix
        for(int i = 0; i < SIZE; i++){
            for(int j = 0; j< SIZE; j++){
                matrix[i][j] = tempM[i][j];
            }
        }



        // check if the program is completed
        completed=1;
        for(int i = 0; i < SIZE; i++){
            for(int j = 0; j< SIZE; j++){
                if(matrix[i][j] > MINIMUM){
                    completed = 0;
                    break;
                }
            }
            if(completed == 0) break;
        }

        iterations++;
        /*std::cout << "Iteration" << iterations << "\n";
        for(int i = 0; i < SIZE; i++){
        for(int j = 0; j< SIZE; j++){
            std::cout << matrix[i][j] << "\t\t";
        }
        std::cout << "\n";
        }*/
    }

    // print final matrix
    std::cout << "\nFinal Matrix: \n" << std::endl;
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j< SIZE; j++){
            std::cout << matrix[i][j] << " ";
        }
        std::cout << "\n";
    }

    std::cout << "\n Iterations " << iterations  << std::endl;

    // Free memory allocated to matrices
    for(int i = 0; i < SIZE; i++){
        delete [] matrix[i];
        delete [] tempM[i];
    }

    delete [] matrix;
    delete [] tempM;

     MPI_Finalize(); // finalize MPI
     return 0;

}