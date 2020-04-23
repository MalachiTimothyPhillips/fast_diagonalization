/*
 * Utilities definition
 */

// Headers
#include "utilities.hpp"

// Functions definition
extern "C" void mxm_(double*, const int*, double*, const int*, double*, const int*);

void matrix_matrix_multiplication(double *AB, double *A, double *B, int n, int p, int m)
{
    mxm_(B, &m, A, &p, AB, &n);
}

void two_vector_collocation(double *uv, double *u, double *v, int n)
{
    for (int i = 0; i < n; i++)
        uv[i] = u[i] * v[i];
}

void quit()
{
    exit(EXIT_SUCCESS);
}
