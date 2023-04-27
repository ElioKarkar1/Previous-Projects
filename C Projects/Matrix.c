/*********************************************************************************
 * Karkar Elio
 * Matrix.c
 * C file for Matrix
 *********************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "List.h"
#include "Matrix.h"
// structs
typedef struct EntryObj
{
    int column;
    double val;
} EntryObj;

typedef EntryObj *Entry;

typedef struct MatrixObj
{
    int size;
    int NNZ;
    List *Entries;
} MatrixObj;

// constructors/destructors

Matrix newMatrix(int n)
{
    Matrix M;
    M = malloc(sizeof(MatrixObj));
    M->size = n;
    M->NNZ = 0;
    M->Entries = malloc(sizeof(List) * n);
    for (int i = 0; i < n; i++)
    {
        M->Entries[i] = newList();
    }
    return (M);
}

Entry newEntry(int x, double d)
{
    Entry E = malloc(sizeof(EntryObj));
    E->column = x;
    E->val = d;
    return (E);
}

void freeEntry(Entry *pE)
{
    if (pE != NULL && *pE != NULL)
    {
        free(*pE);
        *pE = NULL;
    }
}

void freeMatrix(Matrix *pM)
{
    if (pM != NULL && *pM != NULL)
    {
        makeZero(*pM);
        for (int i = 0; i < (*pM)->size; i++)
        {
            freeList(&(*pM)->Entries[i]);
            (*pM)->Entries[i] = NULL;
        }
        free((*pM)->Entries);
    }
    free(*pM);
    *pM = NULL;
}

// access funcs
int size(Matrix M)
{
    if (M == NULL)
    {
        printf("Matrix error: calling size on NULL matrix reference");
        exit(EXIT_FAILURE);
    }
    return (M->size);
}

int NNZ(Matrix M)
{
    if (M == NULL)
    {
        printf("Matrix error: calling NNZ on NULL matrix reference");
        exit(EXIT_FAILURE);
    }
    return (M->NNZ);
}

int equals(Matrix A, Matrix B)
{
    if (A == NULL || B == NULL)
    {
        printf("Matrix Error: calling equals on NULL Matrix reference");
        exit(EXIT_FAILURE);
    }

    if (A->size != B->size)
    {
        return 0;
    }

    for (int i = 0; i < A->size; i++)
    {
        List A_list = A->Entries[i];
        List B_list = B->Entries[i];

        if (length(A_list) != length(B_list))
        {
            return 0;
        }

        moveFront(A_list);
        moveFront(B_list);

        while (index(A_list) >= 0)
        {
            Entry A_entry = (Entry)get(A_list);
            Entry B_entry = (Entry)get(B_list);

            if (A_entry->column != B_entry->column || A_entry->val != B_entry->val)
            {
                return 0;
            }

            moveNext(A_list);
            moveNext(B_list);
        }
    }

    return 1;
}

void makeZero(Matrix M)
{
    if (M == NULL)
    {
        printf("Matrix error: calling makeZero on NULL matrix reference");
        exit(EXIT_FAILURE);
    }

    if (M->NNZ == 0)
    {
        return;
    }

    for (int i = 0; i < M->size; i++)
    {
        List L = M->Entries[i];
        moveFront(L);
        while (index(L) > -1)
        {
            Entry E = (Entry)get(L);
            freeEntry(&E);
            moveNext(L);
        }
        clear(L);
    }

    M->NNZ = 0;
}

void changeEntry(Matrix M, int i, int j, double x)
{
    List L;

    if (M == NULL)
    {
        printf("Matrix Error: calling changeEntry on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }

    if (i < 1 || i > M->size || j < 1 || j > M->size)
    {
        printf("Matrix Error: changeEntry called with invalid indices");
        exit(EXIT_FAILURE);
    }

    L = M->Entries[i - 1];

    if (x == 0)
    {
        if (length(L) == 0)
        {
            return; // Base case 1: x=0, Mij=0, do nothing
        }

        moveFront(L);

        while (index(L) >= 0)
        {
            Entry E = (Entry)get(L);

            if (E->column == j)
            {
                if (E->val == 0)
                {
                    return; // Base case 1: x=0, Mij=0, do nothing
                }
                else
                {
                    delete (L);
                    freeEntry(&E);
                    M->NNZ--;
                    return; // Base case 2: x=0, Mij!=0, delete entry
                }
            }

            moveNext(L);
        }

        return; // Base case 1: x=0, Mij=0, do nothing
    }
    else
    {
        if (length(L) == 0)
        {
            prepend(L, newEntry(j, x));
            M->NNZ++;
            return; // Base case 3: x!=0, Mij=0, create new entry
        }

        moveFront(L);

        while (index(L) >= 0)
        {
            Entry E = (Entry)get(L);

            if (E->column == j)
            {
                E->val = x;
                return; // Base case 4: x!=0, Mij!=0, overwrite entry
            }

            if (E->column > j)
            {
                insertBefore(L, newEntry(j, x));
                M->NNZ++;
                return; // Base case 3: x!=0, Mij=0, create new entry
            }

            moveNext(L);
        }

        append(L, newEntry(j, x));
        M->NNZ++;
        return; // Base case 3: x!=0, Mij=0, create new entry
    }
}

Matrix copy(Matrix A)
{
    if (A == NULL)
    {
        printf("Matrix Error: calling copy on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
    Matrix B = newMatrix(A->size);
    for (int i = 1; i <= A->size; i++)
    {
        List L = A->Entries[i - 1];
        moveFront(L);
        while (index(L) != -1)
        {
            Entry E = (Entry)get(L);
            append(B->Entries[i - 1], newEntry(E->column, E->val));
            moveNext(L);
        }
    }
    B->NNZ = A->NNZ;
    return B;
}

Matrix transpose(Matrix A) {
    if (A == NULL) {
        printf("Matrix Error: transpose() called on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
	Matrix M = newMatrix(A->size);
    List L, P;
   for(int i=0; i<A->size; i++)
   {
      L = A->Entries[i];
      moveFront(L);
      while(index(L) > -1)
      {
        Entry E = (Entry)get(L);
         P = M->Entries[E->column - 1];
         append(P, newEntry(i + 1, E->val));
         moveNext(L);
      }
   }
   M->NNZ = A->NNZ;
   return(M);
}



Matrix scalarMult(double x, Matrix A)
{
    Matrix M = copy(A);
    List L;

    if (A == NULL)
    {
        printf("Matrix Error: calling scalarMult on NULL Matrix reference");
        exit(EXIT_FAILURE);
    }
    if (x != 0)
    {
        for (int i = 0; i < M->size; i++)
        {
            L = M->Entries[i];
            moveFront(L);
            while (index(L) > -1)
            {
                ((Entry)get(L))->val *= x;
                moveNext(L);
            }
        }
    }
    else
    {
        makeZero(M);
    }
    return M;
}
void ListAdd(List L, List P, List S)
{
    moveFront(L);
    moveFront(P);
    while (index(L) >= 0 && index(P) >= 0)
    {
        Entry eL = (Entry)get(L);
        Entry eP = (Entry)get(P);
        if (eL->column == eP->column)
        {
            double sum = eL->val + eP->val;
            if (sum != 0.0)
            {
                append(S, newEntry(eL->column, sum));
            }
            moveNext(L);
            moveNext(P);
        }
        else if (eL->column < eP->column)
        {
            append(S, newEntry(eL->column, eL->val));
            moveNext(L);
        }
        else
        { // eL->column > eP->column
            append(S, newEntry(eP->column, eP->val));
            moveNext(P);
        }
    }
    // Append any remaining entries in L and/or P to S
    while (index(L) >= 0)
    {
        Entry eL = (Entry)get(L);
        append(S, newEntry(eL->column, eL->val));
        moveNext(L);
    }
    while (index(P) >= 0)
    {
        Entry eP = (Entry)get(P);
        append(S, newEntry(eP->column, eP->val));
        moveNext(P);
    }
}

Matrix sum(Matrix A, Matrix B)
{
    if (A == NULL || B == NULL)
    {
        printf("Matrix Error: sum() called with NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
    if (A->size != B->size)
    {
        printf("Matrix Error: sum() called with Matrices of different sizes\n");
        exit(EXIT_FAILURE);
    }
    Matrix C = newMatrix(A->size);
    for (int i = 1; i <= A->size; i++)
    {
        ListAdd(A->Entries[i - 1], B->Entries[i - 1], C->Entries[i - 1]);
    }
    return C;
}

Matrix diff(Matrix A, Matrix B)
{
    Matrix M1, M2, M3;
    if (A == NULL || B == NULL)
    {
        printf("Matrix Error: calling diff() on NULL Matrix reference\n");
        exit(EXIT_FAILURE);
    }
    if (A->size != B->size)
    {
        printf("Matrix Error: calling diff() on matrices of different sizes\n");
        exit(EXIT_FAILURE);
    }
    M1 = copy(B);
    M2 = scalarMult(-1, M1);
    M3 = sum(A, M2);
    freeMatrix(&M1);
    freeMatrix(&M2);
    return (M3);
}

double dot(List L, List P)
{
    double result = 0;
    moveFront(L);
    moveFront(P);

    while (index(L) != -1 && index(P) != -1)
    {
        Entry e1 = get(L);
        Entry e2 = get(P);

        if (e1->column < e2->column)
        {
            moveNext(L);
        }
        else if (e1->column > e2->column)
        {
            moveNext(P);
        }
        else
        {
            result += e1->val * e2->val;
            moveNext(L);
            moveNext(P);
        }
    }

    return result;
}

Matrix product(Matrix A, Matrix B)
{
    double value;
    Matrix M;
    List L, P, O;

    if (A == NULL || B == NULL)
    {
        printf("Matrix Error: calling product on NULL Matrix reference");
        exit(EXIT_FAILURE);
    }

    if (A->size != B->size)
    {
        printf("Matrix Error: calling product on matrices of differing sizes");
        exit(EXIT_FAILURE);
    }

    B = transpose(B);
    M = newMatrix(A->size);

    for (int i = 0; i < M->size; i++)
    {
        L = A->Entries[i];
        int La_length = length(L);

        if (La_length != 0)
        {
            for (int j = 0; j < M->size; j++)
            {
                P = B->Entries[j];
                value = dot(L, P);

                if (value != 0)
                {
                    O = M->Entries[i];
                    append(O, newEntry(j + 1, value));
                    M->NNZ++;
                }
            }
        }
    }

    freeMatrix(&B);
    return M;
}

void printMatrix(FILE *out, Matrix M)
{
    Entry E;
    List L;
    if (M == NULL)
    {
        printf("Matrix Error: calling printMatrix on NULL Matrix reference");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < M->size; i++)
    {
        if (length(M->Entries[i]) > 0)
        {
            fprintf(out, "%d:", i + 1);
            L = M->Entries[i];
            moveFront(L);
            while (index(L) >= 0)
            {
                E = (Entry)get(L);
                if (E->val != 0.0)
                {
                    fprintf(out, " (%d, %0.1f)", E->column + 1, E->val);
                }
                moveNext(L);
            }
            fprintf(out, "\n");
        }
    }
}
