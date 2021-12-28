#include <stdio.h>

#define MAX_DIM 16
#define MELEM(m, n, i, j) m[i*n+j]

void add_row_multiple(double m[], int n, int row_to, int row_from, double mul)
{
	int i;
	for (i = 0; i < n; ++i)
		MELEM(m, n, row_to, i) += MELEM(m,n, row_from, i) * mul;
}


void add_col_multiple(double m[], int n, int col_to, int col_from, double mul)
{
	int i;
	for (i = 0; i < n; ++i)
		MELEM(m,n, i, col_to) += MELEM(m,n, i, col_from) * mul;
}

/* TODO: Also apply operations to identity matrix to get the diagonalizing
 * matrix. */
int diagonalize(double m[], int n)
{
	int i, j;
	double factor;
	for (i = 0; i < n; ++i) {
		if (MELEM(m,n,i,i) == 0.0) {
			for (j = i+1; j<n; ++j) {
				if (MELEM(m,n,i,j) != 0.0)
					break;
			}
			if (j == n)
				return -1;
			/* add row j to row i and column to column i */
			add_row_multiple(m,n, i, j, 1.0);
			add_col_multiple(m,n, i, j, 1.0);
		}

		for (j=i+1; j < n; ++j) {
			if (MELEM(m,n, i, j) != 0.0) {
				factor = -(MELEM(m,n, i,j) / MELEM(m,n,i,i));
				add_row_multiple(m,n,j,i,factor);
				add_col_multiple(m,n,j,i,factor);
			}
		}

	}
	return 0;
}

int main(int argc, char *argv[])
{
	int dim;
	double m[MAX_DIM*MAX_DIM];
	int i;
	printf("Enter matrix dimension: ");
	scanf("%d", &dim);
	if (dim > MAX_DIM) {
		fprintf(stderr, "bad dimension (max %d)\n", MAX_DIM);
		return 1;
	}
	printf("Enter symmetric matrix entries in row major order:\n");
	for (i=0; i < dim*dim; ++i)
		scanf("%lf", &m[i]);
	diagonalize(m,dim);
	for(i=0; i<dim*dim; ++i) {
		printf("%f%c", m[i], (i+1)%dim==0?'\n':' ');
	}
	return 0;
}
