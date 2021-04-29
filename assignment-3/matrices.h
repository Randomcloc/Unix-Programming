/*
 * -----
 * Code by: Abhijeet Suryawanshi
 * Student Number: 19370773
 * Email: abhijeet.suryawanshi@ucdconnect.ie
 * -----
 */
#ifndef MATRICES_H
#define MATRICES_H

#include <stdio.h>
#include <stdlib.h>

double **multiply(double **slice, double **B, int n, int p);
double **getMatrices(int row, int column);

/* 
 * This is the struct that holds the data that needs to be sent and returned. 
 * The socket ID is also stored in this struct to pass it into the handleRequest() function.
 * The resultant slice matrix will also be stored in this struct (in res).
 */
typedef struct matrices
{

   size_t n;
   size_t p;
   int socket;
   double **slice;
   double **B;
   double **res;

} matrices;

/* 
 * This is a function to multiply two matrices.
 * It takes in the slice matrix, B matrix, the number of rows and the number of processes.
 * It returns a two-dimensional matrix.
 */
double **multiply(double **slice, double **B, int n, int p)
{

   double **resultant = (double **)malloc((n / p) * n * sizeof(double));

   for (int i = 0; i < (n / p); i++)
   {

      resultant[i] = calloc(n, sizeof(double));

      for (int j = 0; j < n; j++)
      {

         for (int k = 0; k < n; k++)
         {

            resultant[i][j] += slice[i][k] * B[k][j];
         }
      }
   }

   return resultant;
}

/* 
 * This function allocates memory for a matrix and also fills it with random values.
 * It takes in the number of rows and columns and returns a two-dimensional matrix.
 */
double **getMatrices(int row, int column)
{

   double **A = malloc(row * sizeof(double *));

   for (int i = 0; i < row; i++)
   {

      A[i] = malloc(column * sizeof(double));

      for (int j = 0; j < column; j++)
      {

         A[i][j] = rand() % 10;
      }
   }

   return A;
}

#endif
