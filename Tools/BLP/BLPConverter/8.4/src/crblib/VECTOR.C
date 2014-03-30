//-{----------

#include "vector.h"
#include <math.h> // for sqrt()

//-}{----------- create/destroy stuff ------------------------------------------

Vector * Vector_Create(uint length)
{
Vector * v;
	assert( length > 0 );
	v = new(Vector);
	assert(v);
	v->length = length;
	v->element = malloc(sizeof(v->element[0])*length);
	assert(v->element);
return v;
}

void Vector_Destroy(Vector * v)
{
	assert(v);
	assert(v->element);
	destroy(v->element);
	destroy(v);
}

Vector * Vector_Duplicate(const Vector *v)
{
Vector * ret;
	assert(v);
	ret = Vector_Create(v->length);
	memcpy(ret->element,v->element,sizeof(v->element[0])*v->length);
return ret;
}

void Vector_SetZero(Vector *v)
{
	memset(v->element,0,sizeof(v->element[0])*v->length);
}

Vector * Vector_CreateFromFile(FILE *fp)
{
Vector * v;
uint dim;
	assert(fp);
	fscanf(fp,"%d",&dim);
	if ( dim == 0 )
		return NULL;
	
	v = Vector_Create(dim);

	Vector_SetFromFile(v,fp);
return v;
}

void Vector_SetFromFile(Vector *v,FILE *fp)
{
uint c;
	assert(v && fp);
	for(c=0;c<v->length;c++)
	{
	double val;
		fscanf(fp,"%f",&val);
		v->element[c] = val;
	}
}

//-}{----------- routines ------------------------------------------------

void Vector_Add(Vector *to,const Vector *fm1,const Vector *fm2)
{
uint c;
	assert( fm1 && fm2 );
	assert( fm1->length == fm2->length && fm1->length == to->length );

	for(c=0;c<to->length;c++)
	{
		to->element[c] = fm1->element[c] + fm2->element[2];
	}
}

void Vector_Subtract(Vector *to,const Vector *fm1,const Vector *fm2)
{
uint c;
	assert( fm1 && fm2 );
	assert( fm1->length == fm2->length && fm1->length == to->length );

	for(c=0;c<to->length;c++)
	{
		to->element[c] = fm1->element[c] - fm2->element[2];
	}
}

void Vector_AddScaled(Vector *to,const Vector *fm1,double scale2,const Vector *fm2)
{
uint c;
	assert( fm1 && fm2 );
	assert( fm1->length == fm2->length && fm1->length == to->length );

	for(c=0;c<to->length;c++)
	{
		to->element[c] = fm1->element[c] + scale2 * fm2->element[c];
	}
}

void Vector_Scale(Vector *v,double scale)
{
uint c;
	assert(v);
	for(c=0;c<v->length;c++)
	{
		v->element[c] *= scale;
	}
}

double Vector_SumElements(const Vector * v)
{
uint c;
double d;
	assert(v);
	d = 0.0;
	for(c=0;c<v->length;c++)
	{
		d += v->element[c];
	}
return d;
}

double Vector_LengthSquared(const Vector * v)
{
uint c;
double d,x;
	assert(v);
	d = 0.0;
	for(c=0;c<v->length;c++)
	{
		x = v->element[c];
		d += x*x;
	}
return d;
}

double Vector_Length(const Vector * v)
{
return (double)sqrt(Vector_LengthSquared(v));
}

double Vector_DistanceSquared(const Vector * v1,const Vector * v2)
{
uint c;
double d,x;
	assert( v1 && v2);
	assert( v1->length == v2->length );
	d = 0.0;
	for(c=0;c<v1->length;c++)
	{
		x = v1->element[c] - v2->element[c];
		d += x*x;
	}
return d;
}

double Vector_DotProduct(const Vector * v1,const Vector * v2)
{
uint c;
double d;
	assert( v1 && v2);
	assert( v1->length == v2->length );
	d = 0.0;
	for(c=0;c<v1->length;c++)
	{
		d += v1->element[c] * v2->element[c];
	}
return d;
}

void Vector_Normalize(Vector * v)
{
double len;
	len = Vector_Length(v);
	Vector_Scale(v,1.0/len);
}

//-}----------