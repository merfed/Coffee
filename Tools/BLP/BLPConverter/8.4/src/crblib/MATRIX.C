//-{-------

#include "matrix.h"

#define EPSILON	(0.0001f)

//-}{----------- create/destroy stuff ------------------------------------------

Matrix * Matrix_Create(uint dim)
{
Matrix * m;
uint r;
	
	m = new(Matrix);
	assert(m);

	m->dimension = dim;
	m->rows = malloc(sizeofpointer*dim);

	assert(m->rows);
	for(r=0;r<m->dimension;r++)
	{
		m->rows[r] = Vector_Create(dim);
		assert(m->rows[r]);
	}

return m;
}

void Matrix_Destroy(Matrix * m)
{
uint r;
	assert(m);
	assert(m->rows);

	for(r=0;r<m->dimension;r++)
	{
		Vector_Destroy(m->rows[r]);
	}

	destroy(m->rows);
	destroy(m);

}

Matrix * Matrix_CreateDuplicate(const Matrix *m)
{
Matrix * ret;
uint r;
	ret = Matrix_Create(m->dimension);
	assert(ret);
	for(r=0;r<m->dimension;r++)
	{
		memcpy(ret->rows[r]->element,m->rows[r]->element,sizeof(double)*m->dimension);
	}
return ret;
}

void Matrix_SetIdentity(Matrix * m)
{
uint r,dim;
	dim = m->dimension;

	for(r=0;r<dim;r++)
	{
		Vector_SetZero(m->rows[r]);
		m->rows[r]->element[r] = 1.0;
	}
}

void Matrix_SetZero(Matrix *m)
{
uint r,dim;
	dim = m->dimension;

	for(r=0;r<dim;r++)
	{
		Vector_SetZero(m->rows[r]);
	}
}

void Matrix_WriteToFile(const Matrix *m,FILE *fp)
{
uint r,c;
	assert(m && fp);
	fprintf(fp,"%d\n",m->dimension);
	for(r=0;r<m->dimension;r++)
	{
		for(c=0;c<m->dimension;c++)
		{
			fprintf(fp,"%f ",m->rows[r]->element[c]);
		}
		fprintf(fp,"\n");
	}
}

Matrix * Matrix_CreateFromFile(FILE *fp)
{
Matrix * m;
uint dim;
	assert(fp);
	fscanf(fp,"%d",&dim);
	if ( dim == 0 )
		return NULL;
	
	m = Matrix_Create(dim);

	Matrix_SetFromFile(m,fp);
return m;
}

void Matrix_SetFromFile(Matrix *m,FILE *fp)
{
uint r;
	assert(m && fp);
	
	for(r=0;r<m->dimension;r++)
	{
		Vector_SetFromFile(m->rows[r],fp);
	}
}

void Matrix_MultiplyInto(Matrix *to,const Matrix *by) // right-multiply : to = to * by
{
uint r,c,i,dim;
double * row;

	assert( to && by );
	assert( to != by );
	assert( to->dimension == by->dimension );
	
	dim = to->dimension;

	row = malloc(sizeof(double)*dim);

	for(r=0;r<dim;r++)
	{
		memcpy(row,to->rows[r]->element,sizeof(double)*dim);
		for(c=0;c<dim;c++)
		{
		double val;
			val = 0.0;
			// row r of m1 & column c of m2
			for(i=0;i<dim;i++)
			{
				val += row[i] * by->rows[i]->element[c];	
			}
			to->rows[r]->element[c] = val;
		}
	}

	free(row);
}

void Matrix_Multiply(Matrix *to,const Matrix *m1,const Matrix *m2) // to = m1 * m2
{
uint r,c,i,dim;

	assert( to && m1 && m2 );
	assert( to != m1 && to != m2 );
	assert( to->dimension == m1->dimension && m1->dimension == m2->dimension );
	
	dim = to->dimension;

	for(r=0;r<dim;r++)
	{
		for(c=0;c<dim;c++)
		{
		double val;
			val = 0.0;
			// row r of m1 & column c of m2
			for(i=0;i<dim;i++)
			{
				val += m1->rows[r]->element[i] * m2->rows[i]->element[c];	
			}
			to->rows[r]->element[c] = val;
		}
	}
}

//-}{----------- serious stuff ------------------------------------------

void Matrix_ComputeTranspose(const Matrix *m,Matrix * t)
{
uint r,c,dim;
	assert( m && t );
	assert( m->dimension == t->dimension );

	dim = t->dimension;

	for(r=0;r<dim;r++)
	{
		for(c=0;c<dim;c++)
		{
			t->rows[c]->element[r] = m->rows[r]->element[c];
		}
	}
}

bool Matrix_ComputeInverse(const Matrix *src,Matrix * inv)
{
uint r,c,i,dim;
Matrix * m;

	assert( src && inv );
	assert( src->dimension == inv->dimension );

	// matrix inverse by Gauss-Jordan elimination

	m = Matrix_CreateDuplicate(src);
	Matrix_SetIdentity(inv);

	dim = m->dimension;

	for(c=0;c<dim;c++)
	{
	uint maxR;
	double val,maxVal;
	Vector * swap;

		// try to make the cth column look the identity
		// first, pivot the large value into row c:

		maxVal = m->rows[c]->element[c];
		maxR = c;
		for(r=c+1;r<dim;r++)
		{
			val = m->rows[r]->element[c];
			if ( val > maxVal )
			{
				maxVal = val;
				maxR = i;
			}
		}
		r = maxR;

		if ( c != r )
		{
			swap       = m->rows[c];
			m->rows[c] = m->rows[r];
			m->rows[r] = swap;
			swap         = inv->rows[c];
			inv->rows[c] = inv->rows[r];
			inv->rows[r] = swap;
		}

		val = m->rows[c]->element[c];

		if ( ABS(val) < EPSILON )
			goto fail; // !!

		// subtract this row from all other rows to make their
		//	coefficients zero.

		for(r=0;r<dim;r++)
		{
		double cur;

			if ( r == c )
				continue;
				
			cur = m->rows[r]->element[c];
			if ( ABS(cur) < EPSILON )
			{
				m->rows[r]->element[c] = 0.0;
			}
			else
			{
				cur = - cur/val; // the scale to make the rows[r]->column[c] zero
					
				Vector_AddScaled(inv->rows[r],inv->rows[r], cur, inv->rows[c]);

				// all the elements before c should be 0 or 1 already
				m->rows[r]->element[c] = 0.0;
				for(i=c+1;i<dim;i++)
				{
					m->rows[r]->element[i] += cur * m->rows[c]->element[i];
				}
			}
		}

		// finally, scale my row so my coefficient is 1

		val = 1.0/val;
		Vector_Scale(m->rows[c], val);
		Vector_Scale(inv->rows[c], val);
	}

	Matrix_Destroy(m);

return true;

fail:

	Matrix_Destroy(m);

return false;
}

void Matrix_Apply(const Matrix *m,const Vector *b,Vector *a)
{
uint r;

	assert( a && b && m );
	assert( a->length == b->length && a->length == m->dimension );
	assert( a != b );

	for(r=0;r<m->dimension;r++)
	{
		a->element[r] = Vector_DotProduct(b,m->rows[r]);
	}
}

void Matrix_SolveLinearEquation(const Matrix *m,const Vector *b,Vector *a)
{
Matrix * inv;

	assert( a && b && m );
	assert( a->length == b->length && a->length == m->dimension );
	assert( a != b );

	// solve m a = b for a

	inv = Matrix_Create(m->dimension);

	Matrix_ComputeInverse(m,inv);

	Matrix_Apply(inv,b,a); // a = m^(-1) b

	Matrix_Destroy(inv);
}

//-}-------
