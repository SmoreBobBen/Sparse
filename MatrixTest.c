//-----------------------------------------------------------------------------
// MatrixTest.c
// programmer: Benjamin Watkins
//-----------------------------------------------------------------------------

#include "Matrix.h"
#include <stdio.h>

int main(void) {
  Matrix M = newMatrix(4);
  // printf("NNZ is %i\n", NNZ(M));

  // copying test script construction
  Matrix A = newMatrix(10);
  Matrix B = newMatrix(10);
  changeEntry(A, 1, 1, -4);
  changeEntry(A, 1, 2, -2);
  changeEntry(A, 1, 3, 0);
  changeEntry(A, 2, 5, 4);
  changeEntry(A, 2, 1, -2);
  changeEntry(A, 3, 1, 2);
  changeEntry(A, 2, 2, -2);
  changeEntry(A, 3, 3, 0);
  changeEntry(B, 1, 1, -4);
  changeEntry(B, 1, 2, 0);
  changeEntry(B, 2, 1, 0);
  changeEntry(B, 2, 2, -2);
  changeEntry(B, 2, 4, 2);
  changeEntry(B, 3, 1, 2);
  changeEntry(B, 3, 2, 2);
  changeEntry(B, 7, 8, 5);

  // copying test script tests
  printf("-------\n");
  printMatrix(stdout, A);
  printf("-------\n");
  printMatrix(stdout, B);
  printf("-------\n");
  Matrix T = diff(A, B);
  printMatrix(stdout, T);
  printf("-------\n");
  printf("\nNNZ of T is %i\n", NNZ(T));
  if (NNZ(T) != 6)
    printf("failed test 2\n");

  printf("-------\n");
  // printMatrix(stdout, M);
  //  printf("NNZ is %i\n", NNZ(M));

  // Matrix CM = copy(M);
  /*Matrix CM = transpose(M);
  printf("\nprinting transpose\n");
  printMatrix(stdout, CM);
  printf("NNZ of CM is %i\n", NNZ(CM));
  freeMatrix(&CM);

  Matrix SM = scalarMult(5, M);
  printf("\nprinting x5 scalarMult\n");
  printMatrix(stdout, SM);
  printf("NNZ of SM is %i\n", NNZ(SM));
  freeMatrix(&SM);
  */

  // testing math functions
  // printf("\n---testing math functions---\n");

  // creating matrix N
  Matrix N = newMatrix(4);

  /*
  printf("\nmatrix M:\n");
  printMatrix(stdout, M);
  printf("\nmatrix N:\n");
  printMatrix(stdout, N);

  // testing sum
  Matrix S = sum(M, N);
  printf("\nmatrix M + N:\n");
  printMatrix(stdout, S);
  printf("\nNNZ is: %i\n", NNZ(S));
  freeMatrix(&S);

  // testing diff
  Matrix D = diff(M, N);
  printf("\nmatrix M - N:\n");
  printMatrix(stdout, D);
  freeMatrix(&D);

  // testing makeZero
  //makeZero(D);

  // testing product
  Matrix P = product(M, N);
  printf("\nmatrix M * N:\n");
  printMatrix(stdout, P);
  freeMatrix(&P);
  */

  freeMatrix(&M);
  freeMatrix(&A);
  freeMatrix(&B);
  freeMatrix(&T);
  freeMatrix(&N);
}
