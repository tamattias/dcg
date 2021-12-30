#include <stdio.h>

#define MAX_DIM 16
#define MELEM(m, n, i, j) m[(i)*(n)+(j)]

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

int diagonalize(double m[], double p[], int n)
{
	int i, j;
	double factor;

	/* Initialize the change of basis matrix to the identity matrix. */
	for (i = 0; i < n; ++i)
		for (j = 0; j < n; ++j)
			MELEM(p,n,i,j) = i == j;

	for (i = 0; i < n; ++i) {
		if (MELEM(m,n,i,i) == 0.0) {
			/* Element is zero, find row in which the leading element is
			 * non-zero. */
			for (j = i+1; j<n; ++j) {
				if (MELEM(m,n,i,j) != 0.0)
					break;
			}
			if (j != n) {
				/* Add row j to row i and column to column i */
				add_row_multiple(m, n, i, j, 1.0);
				add_col_multiple(m, n, i, j, 1.0);

				/* Apply equivalent row operation to identity matrix. */
				add_col_multiple(p, n, i, j, 1.0);
			} else
				return -1; /* Non-diagonalizable! */
		}

		for (j = i+1; j < n; ++j) {
			if (MELEM(m,n, i, j) != 0.0) {
				factor = -(MELEM(m, n, i, j) / MELEM(m,n,i,i));

				add_row_multiple(m, n, j, i, factor);
				add_col_multiple(m, n, j, i, factor);

				/* Only apply row operation to the change of basis matrix. */
				add_row_multiple(p, n, j, i, factor);
			}
		}

	}
	return 0;
}

void print_matrix(double m[], int dim)
{
	int i;
	for(i = 0; i < dim*dim; ++i)
		printf("%f%c", m[i], (i+1) % dim == 0 ? '\n' : ' ');
}

int main(int argc, char *argv[])
{
	int dim;
	double m[MAX_DIM*MAX_DIM]; /* Matrix to diagonalize. */
	double p[MAX_DIM*MAX_DIM]; /* Change of basis matrix. */
	int i;

	printf("Enter square matrix dimension (max %d): ", MAX_DIM);
	scanf("%d", &dim);

	if (dim < 0 || dim > MAX_DIM) {
		fprintf(stderr, "Bad dimension.\n");
		return 1;
	}

	printf("Enter symmetric matrix entries in row major order:\n");
	for (i=0; i < dim*dim; ++i)
		scanf("%lf", &m[i]);

	if (diagonalize(m, p, dim)) {
		fprintf(stderr, "Matrix not congruently diagonalizable.\n");
		return 1;
	}

	printf("Diagonal matrix:\n");
	print_matrix(m, dim);

	printf("Change of basis matrix:\n");
	print_matrix(p, dim);

	return 0;
}
