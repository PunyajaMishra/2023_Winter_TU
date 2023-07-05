/*
Write a C++ program that generates random 10x10 systems of linear equations 
and solve with with Gauss Jordan elimination.  
(Note this means that your actual matrix will have 11 columns and 10 rows)   


a)	Throughput – every 1 second print off how many times it was able to run the solutions, 
run this for 10 seconds.  

b)	Capacity computing (job time) – calculate how long it takes for 
100, 200, 500, 1000, 2500, 5000, 7500 and 10 000 runs of the calculation.  
(If that takes absurdly long on your machine pick smaller numbers, 
I don’t actually care what it is, it seems like 10 000 should take about 5 minutes).  
Plot the result (feel free to use excel for the plot).  

c)	Parallelize with OpenMP and see how much (if any) improvement you get to one of the 
longer calculations in b).  
*/

#include <iostream>
#include <stdio.h>          /* printf, scanf, puts, NULL */
#include <stdlib.h>         /* srand, rand */
#include <bits/stdc++.h>
#include <omp.h>            /*to parallelize with omp*/
#include <ctime>
#include <chrono>

using namespace std;
 
#define M 11
 
// Function to print the matrix
void PrintMatrix(double a[][M], int n)
{
    for (int i = 0; i < n; i++) {
        for (int j = 0; j <= n; j++)
          cout << a[i][j] << " ";
        cout << endl;
    }
}
 
// function to reduce matrix to reduced
// row echelon form.
int PerformOperation(double a[][M], int n)
{
    int i, j, k = 0, c, flag = 0, m = 0;
    double pro = 0;
     
    // Performing elementary operations
    for (i = 0; i < n; i++)
    {
        if (a[i][i] == 0)
        {
            c = 1;
            while ((i + c) < n && a[i + c][i] == 0)
                c++;           
            if ((i + c) == n) {
                flag = 1;
                break;
            }
            for (j = i, k = 0; k <= n; k++)
                swap(a[j][k], a[j+c][k]);
        }

        #pragma omp parallel for private(i,j,k)
        for (j = 0; j < n; j++) {
             
            // Excluding all i == j
            if (i != j) {
                 
                // Converting Matrix to reduced row
                // echelon form(diagonal matrix)
                double pro = a[j][i] / a[i][i];
                #pragma omp critical
                for (k = 0; k <= n; k++)                
                    
                    a[j][k] = a[j][k] - (a[i][k]) * pro;               
            }
        }
    }
    return flag;
}
 
// Function to print the desired result
// if unique solutions exists, otherwise
// prints no solution or infinite solutions
// depending upon the input given.
void PrintResult(double a[][M], int n, int flag)
{
    cout << "Result is : ";
 
    if (flag == 2)    
      cout << "Infinite Solutions Exists" << endl;   
    else if (flag == 3)    
      cout << "No Solution Exists" << endl;
     
     
    // Printing the solution by dividing constants by
    // their respective diagonal elements
    else {
        for (int i = 0; i < n; i++)        
            cout << a[i][n] / a[i][i] << " ";       
    }
}
 
// To check whether infinite solutions
// exists or no solution exists
int CheckConsistency(double a[][M], int n, int flag)
{
    int i, j;
    double sum;
     
    // flag == 2 for infinite solution
    // flag == 3 for No solution
    flag = 3;
    for (i = 0; i < n; i++)
    {
        sum = 0;
        for (j = 0; j < n; j++)       
            sum = sum + a[i][j];
        if (sum == a[i][j])
            flag = 2;       
    }
    return flag;
}
 
// Driver code
int main()
{
    // Order of Matrix(n)
    int n = 10, flag = 0;

    //A: THROUGHPUT    
    auto start1 = std::chrono::high_resolution_clock::now();
    auto end1 = std::chrono::high_resolution_clock::now();
    auto duration1 = chrono::duration_cast<chrono::seconds>(end1 - start1);

    //matrix
    double a[M][M];
    int count = 0; 

    auto previous = std::chrono::high_resolution_clock::now();
    while(duration1.count() <= 10){

        if(chrono::duration_cast<chrono::seconds>(std::chrono::high_resolution_clock::now() - previous).count() == 1)
        {
            cout << "thorughput - Number of times performed: " << count <<endl;
            count = 0;
            previous = std::chrono::high_resolution_clock::now();
        }

        for(int i=0; i< n; i++){
                for(int j=0; j<n; j++){
                    a[i][j] = rand() % 100;
                }
            }
            // Performing Matrix transformation
            flag = PerformOperation(a, n);
            
            if (flag == 1)    
                flag = CheckConsistency(a, n, flag);   
        
            count++;
            end1 = std::chrono::high_resolution_clock::now();
            duration1 = chrono::duration_cast<chrono::seconds>(end1 - start1);
    }
    
    
  
   
    //OPTION B: Capacity computing
    double b[M][M];
    int testRuns[5] = {10000, 1000000, 2500000, 3000000, 10000000};

    std::chrono::duration<double> elapsedTime[5];

    for(int r=0; r<5; r++){
        auto start2 = std::chrono::high_resolution_clock::now();

        for(int i=0; i< testRuns[r]; i++){
            for(int i=0; i< n; i++){
                for(int j=0; j<n+1; j++){
                    b[i][j] = rand() % 100;
                }
            }
            // Performing Matrix transformation
            flag = PerformOperation(b, n);
            
            if (flag == 1)    
                flag = CheckConsistency(b, n, flag);  

        }
        auto end2 = std::chrono::high_resolution_clock::now();
        elapsedTime[r] = end2 - start2;
        std::cout << "Time elapsed for " << testRuns[r] << " runs: " << elapsedTime[r].count() << " seconds" << std::endl;
    }

    //option C: parallelize with Open MP
    double c[M][M];
    auto start = std::chrono::high_resolution_clock::now();
    for(int i=0; i< 10000000; i++){
        for(int i=0; i< n; i++){
            for(int j=0; j<n+1; j++){
                c[i][j] = rand() % 100;
            }
        }
    // Performing Matrix transformation
    flag = PerformOperation(c, n);            
    if (flag == 1)    
        flag = CheckConsistency(c, n, flag);  
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed_seconds = end - start;

    std::cout << "Time elapsed for 10000000 runs: " << elapsed_seconds.count() << " seconds" << std::endl;
        
    return 0;
}