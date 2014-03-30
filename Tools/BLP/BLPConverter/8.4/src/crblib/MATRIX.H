#ifndef MATRIX_H
#define MATRIX_H

#include <crblib/inc.h>
#include <stdio.h>

#include "vector.h"

typedef struct
{
	uint dimension;
	Vector ** rows;
} Matrix;

Matrix * Matrix_Create(uint dim);
void Matrix_Destroy(Matrix * m);

Matrix * Matrix_CreateDuplicate(const Matrix *m);

Matrix * Matrix_CreateFromFile(FILE *fp);    // reads length & elements
void Matrix_SetFromFile(Matrix *m,FILE *fp); // does not read length

void Matrix_WriteToFile(const Matrix *m,FILE *fp);

void Matrix_ComputeTranspose(const Matrix *m,Matrix * t);
bool Matrix_ComputeInverse(const Matrix *m,Matrix * inv);

void Matrix_MultiplyInto(Matrix *to,const Matrix *by); // right-multiply in place : to = to*by
void Matrix_Multiply(Matrix *to,const Matrix *m1,const Matrix *m2); // to = m1 * m2
void Matrix_Apply(const Matrix *m,const Vector *b,Vector *a); // a = M * b

void Matrix_SolveLinearEquation(const Matrix *m,const Vector *b,Vector *a);
	// solve m a = b for a

void Matrix_SetZero(Matrix *m);

#endif // MATRIX_H

