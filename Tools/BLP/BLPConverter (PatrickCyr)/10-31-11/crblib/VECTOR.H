#ifndef VECTOR_H
#define VECTOR_H

#include <crblib/inc.h>
#include <stdio.h>

typedef struct
{
	uint length;
	double * element;
} Vector;

Vector * Vector_Create(uint length);
void Vector_Destroy(Vector * v);

Vector * Vector_Duplicate(const Vector *v);

void Vector_SetZero(Vector *v);

void Vector_SetFromFile(Vector *v,FILE *fp);

Vector * Vector_CreateFromFile(FILE *fp);    // reads length & elements
void Vector_SetFromFile(Vector *v,FILE *fp); // does not read length

void Vector_Add(Vector *to,const Vector *fm1,const Vector *fm2);
void Vector_Subtract(Vector *to,const Vector *fm1,const Vector *fm2);
void Vector_AddScaled(Vector *to,const Vector *fm1,double scale2,const Vector *fm2);
void Vector_Scale(Vector *v,double scale);
void Vector_Normalize(Vector * v);

double Vector_LengthSquared(const Vector * v);
double Vector_Length(const Vector * v);
double Vector_DistanceSquared(const Vector * v1,const Vector * v2);
double Vector_SumElements(const Vector * v);

double Vector_DotProduct(const Vector * v1,const Vector * v2);


#endif // VECTOR_H
