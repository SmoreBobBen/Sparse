//-----------------------------------------------------------------------------
// Matrix.c
// programmer: Benjamin Watkins
// cruzid: bdwatkin
// assignment: pa4
//-----------------------------------------------------------------------------

#include "List.h"
#include <stdlib.h>

// Bones -----------------------------------------------------------------------
typedef struct EntryObj {
  int col;
  double data;
} EntryObj;

typedef struct EntryObj *Entry;

typedef struct MatrixObj {
  int size;
  int NNZ;
  List *EList;
} MatrixObj;

typedef struct MatrixObj *Matrix;

// newMatrix()
// Returns a reference to a new nXn Matrix object in the zero state.
Matrix newMatrix(int n) {
  Matrix M = calloc(1, sizeof(struct MatrixObj));
  M->size = n;
  M->NNZ = 0;
  M->EList = calloc(n + 1, sizeof(List));
  for (int i = 0; i < n + 1; i++) {
    M->EList[i] = newList();
  }
  return M;
}

// freeMatrix()
// Frees heap memory associated with *pM, sets *pM to NULL.
void freeMatrix(Matrix *pM) {
  for (int i = 0; i < ((*pM)->size + 1); i++) {
    moveFront((*pM)->EList[i]);
    while (index((*pM)->EList[i]) >= 0) {
      free(front((*pM)->EList[i]));
      deleteFront((*pM)->EList[i]);
      moveFront((*pM)->EList[i]);
    }
    freeList(&((*pM)->EList[i]));
  }
  free((*pM)->EList);
  free(*pM);
  pM = NULL;
}

Entry newEntry(int column, double x) {
  Entry E = calloc(1, sizeof(struct EntryObj));
  E->col = column;
  E->data = x;
  return E;
}

void freeEntry(Entry *pE) {
  free(*pE);
  pE = NULL;
}

// Internals  ------------------------------------------------------------------
void printEntryList(FILE *out, List L) {
  // prints a list of entries in the specified (col, val) format
  Entry current;
  moveFront(L);
  while (index(L) >= 0) {
    current = get(L);
    fprintf(out, "(%i, %.1f) ", current->col, current->data);
    moveNext(L);
  }
}

// Access functions -----------------------------------------------------------
// size()
// Return the size of square Matrix M.
int size(Matrix M) { return M->size; }

// NNZ()
// Return the number of non-zero elements in M.
int NNZ(Matrix M) { return M->NNZ; }

// entryEquals()
// returns true(1) if entries a and b are the same, false (0) otherwise
int entryEquals(Entry a, Entry b) {
  if (a->col == b->col || a->data == b->data) {
    return 1; // true
  }
  return 0; // false
}

// equals()
// Return true (1) if matrices A and B are equal, false (0) otherwise.
int equals(Matrix A, Matrix B) {
  if (size(A) != size(B) || NNZ(A) != NNZ(B)) {
    return 0;
  }
  for (int i = 1; i <= size(A); i++) {
    if (length(A->EList[i]) != length(B->EList[i])) {
      return 0;
    }
    moveFront(A->EList[i]);
    moveFront(B->EList[i]);
    while (index(A->EList[i]) >= 0) {
      if (entryEquals(get(A->EList[i]), get(B->EList[i])) != 1) {
        return 0;
      }
      moveNext(A->EList[i]);
      moveNext(B->EList[i]);
    }
  }
  // passed the gauntlet
  return 1;
}

// Manipulation procedures ----------------------------------------------------
// makeZero()
// Re-sets M to the zero Matrix state.
void makeZero(Matrix M) {
  M->NNZ = 0;
  for (int i = 1; i <= size(M); i++) {
    moveFront(M->EList[i]);
    while (index(M->EList[i]) >= 0) {
      free(front(M->EList[i]));
      deleteFront(M->EList[i]);
      moveFront(M->EList[i]);
    }
  }
}

// entryInsert()
// helper function for changeEntry, adds an entry while maintaining uniqueness
// and column order
void entryInsert(Matrix M, List L, int j, double x) {
  // matrix input is only used to increment/decrement NNZ
  Entry current;
  moveFront(L);
  while (index(L) >= 0) {
    current = get(L);
    if (current->col == j) {
      current->data = x;
      // check if setting entry to zero
      if (x == 0.0) {
        freeEntry(&current);
        delete (L);
        M->NNZ--;
      }
      return;
    } else if (current->col > j) {
      // check if creating a zero entry
      if (x == 0.0) {
        return;
      }
      M->NNZ++;
      Entry insert = newEntry(j, x);
      insertBefore(L, insert);
      return;
    }
    moveNext(L);
  }
  if (x == 0.0) {
    return;
  }
  M->NNZ++;
  Entry insert = newEntry(j, x);
  append(L, insert);
}

// changeEntry()
// Changes the ith row, jth column of M to the value x.
// Pre: 1<=i<=size(M), 1<=j<=size(M)
void changeEntry(Matrix M, int i, int j, double x) {
  // ith row, jth column
  // theta(NNZ)
  if (1 <= i && i <= size(M) && 1 <= j && j <= size(M)) {
    entryInsert(M, M->EList[i], j, x);
  }
}

// Matrix Arithmetic operations -----------------------------------------------
// copy()
// Returns a reference to a new Matrix object having the same entries as A.
Matrix copy(Matrix A) {
  Matrix B = newMatrix(size(A));
  Entry current;
  for (int i = 1; i <= size(A); i++) {
    moveFront(A->EList[i]);
    while (index(A->EList[i]) >= 0) {
      current = get(A->EList[i]);
      Entry insert = newEntry(current->col, current->data);
      append(B->EList[i], insert);
      moveNext(A->EList[i]);
    }
  }
  B->NNZ = A->NNZ;
  return B;
}

// transpose()
// Returns a reference to a new Matrix object representing the transpose
// of A.
Matrix transpose(Matrix A) {
  // same as copy but switch i and j
  Matrix B = newMatrix(size(A));
  Entry current;
  for (int i = 1; i <= size(A); i++) {
    moveFront(A->EList[i]);
    while (index(A->EList[i]) >= 0) {
      current = get(A->EList[i]);
      Entry insert = newEntry(i, current->data);
      append(B->EList[current->col], insert);
      moveNext(A->EList[i]);
    }
  }
  B->NNZ = A->NNZ;
  return B;
}

// scalarMult()
// Returns a reference to a new Matrix object representing xA.
Matrix scalarMult(double x, Matrix A) {
  // same as copy but mult each data by x
  Matrix B = newMatrix(size(A));
  Entry current;
  for (int i = 1; i <= size(A); i++) {
    moveFront(A->EList[i]);
    while (index(A->EList[i]) >= 0) {
      current = get(A->EList[i]);
      Entry insert = newEntry(current->col, (x * current->data));
      append(B->EList[i], insert);
      moveNext(A->EList[i]);
    }
  }
  B->NNZ = A->NNZ;
  return B;
}

// rowSum()
// helper function for sum(), sums two rows of a matrix
void rowSum(List A, List B, List C) {
  /*printf("rowsum of:\n");
  printEntryList(stdout, A);
  printf("\n  +  \n");
  printEntryList(stdout, B);
  printf("\n");*/

  moveFront(A);
  moveFront(B);
  Entry a, b; // current entry of A/B
  while (index(A) >= 0 || index(B) >= 0) {
    if (index(A) < 0) {
      b = get(B);
      Entry insert = newEntry(b->col, b->data);
      append(C, insert);
      moveNext(B);
    } else if (index(B) < 0) {
      a = get(A);
      Entry insert = newEntry(a->col, a->data);
      append(C, insert);
      moveNext(A);
    } else {
      // a and b exist
      a = get(A);
      b = get(B);
      if (a->col < b->col) {
        Entry insert = newEntry(a->col, a->data);
        append(C, insert);
        moveNext(A);
      } else if (a->col > b->col) {
        Entry insert = newEntry(b->col, b->data);
        append(C, insert);
        moveNext(B);
      } else if (a->col == b->col) {
        // printf("\nat %i, %i, summing to %f", a->col, b->col, (a->data +
        // b->data));
        if ((a->data + b->data) != 0) {
          Entry insert = newEntry(a->col, (a->data + b->data));
          append(C, insert);
        }
        moveNext(A);
        if (A != B) {
          // to not move twice when doing A+A
          moveNext(B);
        }
      }
    }
  }
}

// sum()
// Returns a reference to a new Matrix object representing A+B.
// pre: size(A)==size(B)
Matrix sum(Matrix A, Matrix B) {
  if (size(A) != size(B)) {
    return NULL;
  }
  // prerequisites met, start actual function
  int n = size(A);
  int SumNZ = 0;
  Matrix S = newMatrix(n);
  for (int i = 1; i <= n; i++) {
    rowSum(A->EList[i], B->EList[i], S->EList[i]);
    SumNZ += length(S->EList[i]);
  }
  S->NNZ = SumNZ;
  return S;
}

// helper function for Diff
void rowDiff(List A, List B, List C) {
  // exact same as rowSum but b is negative
  moveFront(A);
  moveFront(B);
  Entry a, b; // current entry of A/B
  while (index(A) >= 0 || index(B) >= 0) {
    if (index(A) < 0) {
      b = get(B);
      Entry insert = newEntry(b->col, -(b->data));
      append(C, insert);
      moveNext(B);
    } else if (index(B) < 0) {
      a = get(A);
      Entry insert = newEntry(a->col, a->data);
      append(C, insert);
      moveNext(A);
    } else {
      // a and b exist
      a = get(A);
      b = get(B);
      if (a->col < b->col) {
        Entry insert = newEntry(a->col, a->data);
        append(C, insert);
        moveNext(A);
      } else if (a->col > b->col) {
        Entry insert = newEntry(b->col, -(b->data));
        append(C, insert);
        moveNext(B);
      } else if (a->col == b->col) {
        if (a->data != b->data) {
          Entry insert = newEntry(a->col, (a->data - b->data));
          append(C, insert);
        }
        moveNext(A);
        moveNext(B);
      }
    }
  }
}

// diff()
// Returns a reference to a new Matrix object representing A-B.
// pre: size(A)==size(B)
Matrix diff(Matrix A, Matrix B) {
  if (size(A) != size(B)) {
    return NULL;
  }
  // prerequisites met, start actual function
  int n = size(A);
  int DiffNZ = 0;
  Matrix D = newMatrix(n);
  for (int i = 1; i <= n; i++) {
    rowDiff(A->EList[i], B->EList[i], D->EList[i]);
    DiffNZ += length(D->EList[i]);
  }
  D->NNZ = DiffNZ;
  return D;
}

// private function vectorDot
// helper function for product
// computes the vector dot product of two matrix rows represented by Lists P
// and Q. Use this function together with function transpose() to help
// implement product().
double vectorDot(List A, List B) {
  // based on rowsum
  Entry a, b; // current entry of A/B
  double sum = 0;

  moveFront(A);
  moveFront(B);
  while (index(A) >= 0 || index(B) >= 0) {
    if (index(A) < 0) {
      // multiplying by zero, does nothing to sum
      moveNext(B);
    } else if (index(B) < 0) {
      // multiplying by zero, does nothing to sum
      moveNext(A);
    } else {
      // a and b exist
      a = get(A);
      b = get(B);
      if (a->col < b->col) {
        // multiplying by zero, does nothing to sum
        moveNext(A);
      } else if (a->col > b->col) {
        // multiplying by zero, does nothing to sum
        moveNext(B);
      } else if (a->col == b->col) {
        // actual multiplication! yippee!
        sum = sum + (a->data * b->data);
        moveNext(A);
        moveNext(B);
      }
    }
  }
  return sum;
}

// product()
// Returns a reference to a new Matrix object representing AB
// pre: size(A)==size(B)
Matrix product(Matrix A, Matrix B) {
  // each entry is the dot product of its row in A and column in B
  // do this by calling vectorDot on the row i in A and row j in BT
  if (size(A) != size(B)) {
    return NULL;
  }
  int n = size(A);
  int PNZ = 0;
  double num = 0;
  Matrix BT = transpose(B);
  Matrix C = newMatrix(n);

  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      num = vectorDot(A->EList[i], BT->EList[j]);
      if (num != 0.0) {
        Entry insert = newEntry(j, num);
        append(C->EList[i], insert);
      }
    }
    PNZ += length(C->EList[i]);
  }
  freeMatrix(&BT);

  C->NNZ = PNZ;
  return C;
}

// printMatrix()
// Prints a string representation of Matrix M to filestream out. Zero rows
// are not printed. Each non-zero row is represented as one line consisting
// of the row number, followed by a colon, a space, then a space separated
// list of pairs "(col, val)" giving the column numbers and non-zero values
// in that row. The double val will be rounded to 1 decimal point.

void printMatrix(FILE *out, Matrix M) {
  for (int i = 1; i <= size(M); i++) {
    if (length(M->EList[i]) > 0) {
      fprintf(out, "%i: ", i);
      printEntryList(out, M->EList[i]);
      fprintf(out, "\n");
    }
  }
}
