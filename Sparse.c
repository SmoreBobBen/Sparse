//-----------------------------------------------------------------------------
// Sparse.c
// programmer: Benjamin Watkins
// cruzid: bdwatkin
// assignment: pa4
//-----------------------------------------------------------------------------

#include "Matrix.h"
#include <stdio.h>

int readInt(FILE *in) { // reads an integer from the file stream, accounting for
                        // multiple digits and converting from number characters
                        // to an actual int value
  int num = 0;          // return value
  int chara = fgetc(in);
  while ('0' <= chara && '9' >= chara) {
    num = (10 * num) + (chara - '0');
    chara = fgetc(in);
  }
  return num; // returns 0 if not reading a digit
}

double exponent(double x, int n) {
  // get double value x^n
  double exp = 1.0;
  for (int i = 0; i < n; i++) {
    exp = exp * x;
  }
  return exp;
}

double readDouble(FILE *in) {
  double num = 0; // return value
  int chara = fgetc(in);
  int sign = 1; // set to -1 for negative numbers
  if (chara == '-') {
    sign = -1;
    chara = fgetc(in);
  }
  while ('0' <= chara && '9' >= chara) {
    num = (10 * num) + ((double)(chara - '0'));
    chara = fgetc(in);
  }
  if (chara == '.') {
    // reading past decimal place
    chara = fgetc(in);
    int depth = 1;
    while ('0' <= chara && '9' >= chara) {
      chara = chara - '0';
      num = num + ((double)chara) * exponent(0.1, depth);
      depth++;
      chara = fgetc(in);
    }
  }
  return sign * num; // returns 0 if not reading a digit
}

int main(int argc, char **argv) {
  // Check correct command line arguments -------------------------------------
  if (argc != 3) {
    // invalid number of arguments
    fprintf(stderr, "Invalid number of arguments\n");
    return 1;
  }
  // Open files from command line arguments
  FILE *infile = fopen(argv[1], "r");
  if (infile == NULL) {
    fprintf(stdout, "Unable to open infile");
    return 1;
  }
  FILE *outfile = fopen(argv[2], "w");
  if (outfile == NULL) {
    fprintf(stdout, "Unable to open outfile");
    return 1;
  }

  // Creating Matrix from file ------------------------------------------
  int n = readInt(infile);    // size of matrix
  int NNZA = readInt(infile); // NNZ (number of non zero entries) of A
  int NNZB = readInt(infile); // NNZ of B
  int i, j;                   // row/column being read
  double x;                   // value being read
  Matrix A = newMatrix(n);
  Matrix B = newMatrix(n);

  // reading file 1 row at a time, each is 2 ints and 1 double
  // making matrix A
  readInt(infile); // skipping a line
  i = readInt(infile);
  while (i != 0) {
    j = readInt(infile);
    x = readDouble(infile);
    changeEntry(A, i, j, x);
    i = readInt(infile);
  }

  // making matrix B
  // readInt(infile);// skipping a line
  i = readInt(infile);
  while (i != 0) {
    j = readInt(infile);
    x = readDouble(infile);
    // printf("changing entry %i, %i to %f\n", i, j, x);
    changeEntry(B, i, j, x);
    i = readInt(infile);
  }

  // Output -------------------------------------------------------------
  fprintf(outfile, "A has %i non-zero entries:\n", NNZA);
  printMatrix(outfile, A);
  fprintf(outfile, "\nB has %i non-zero entries:\n", NNZB);
  printMatrix(outfile, B);

  // scalarMult
  Matrix M = scalarMult(1.5, A);
  fprintf(outfile, "\n(1.5)*A =\n");
  printMatrix(outfile, M);

  // sum A+B
  freeMatrix(&M);
  M = sum(A, B);
  fprintf(outfile, "\nA+B =\n");
  printMatrix(outfile, M);

  // sum A+A
  freeMatrix(&M);
  M = sum(A, A);
  fprintf(outfile, "\nA+A =\n");
  printMatrix(outfile, M);

  // diff B-A
  freeMatrix(&M);
  M = diff(B, A);
  fprintf(outfile, "\nB-A =\n");
  printMatrix(outfile, M);

  // diff A-A
  freeMatrix(&M);
  M = diff(A, A);
  fprintf(outfile, "\nA-A =\n");
  printMatrix(outfile, M);

  // Transpose(A)
  freeMatrix(&M);
  M = transpose(A);
  fprintf(outfile, "\nTranspose(A) =\n");
  printMatrix(outfile, M);

  // product A*B
  freeMatrix(&M);
  M = product(A, B);
  fprintf(outfile, "\nA*B =\n");
  printMatrix(outfile, M);

  // product B*B
  freeMatrix(&M);
  M = product(B, B);
  fprintf(outfile, "\nB*B =\n");
  printMatrix(outfile, M);

  // close and exit
  fclose(infile);
  fclose(outfile);
  freeMatrix(&A);
  freeMatrix(&B);
  freeMatrix(&M);
  return 0;
}
