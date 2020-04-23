/*
 * FDM Operator class definition
 */

// Headers
#include <iostream>
#include <occa.hpp>
#include "fdm_operator.hpp"
#include "utilities.hpp"

// Namespaces
using namespace std;

// Global variables
extern double *work_hst_1;
extern double *work_hst_2;
extern occa::memory work_dev_1;
extern occa::memory work_dev_2;


// Constructor
FDM_Operator::FDM_Operator(int dim_, int num_elements_, int N_, occa::device device_) :
    dim(dim_),
    num_elements(num_elements_),
    N(N_),
    operator_size(N_ * N_),
    device(device_)
{
    // Initialize memory
    S_x = device.malloc<double>(operator_size * num_elements);
    S_y = device.malloc<double>(operator_size * num_elements);
    S_z = device.malloc<double>(operator_size * num_elements);

    St_x = device.malloc<double>(operator_size * num_elements);
    St_y = device.malloc<double>(operator_size * num_elements);
    St_z = device.malloc<double>(operator_size * num_elements);

    L_x = device.malloc<double>(N * num_elements);
    L_y = device.malloc<double>(N * num_elements);
    L_z = device.malloc<double>(N * num_elements);

    num_points_elem = (dim == 2) ? N * N : N * N * N;
    num_points_total = num_elements * num_points_elem;

    inv_L = device.malloc<double>(num_points_total);

    // Kernels
    properties += device.properties();
    properties["defines/DIM"] = dim;
    properties["defines/N"] = N;
    properties["defines/N2"] = N * N;
    properties["defines/NUM_ELEMENTS"] = num_elements;
    properties["defines/NUM_POINTS_TOTAL"] = num_points_total;
    properties["defines/NUM_POINTS_ELEM"] = num_points_elem;

    lambda_inverse_kernel = device.buildKernel("fdm_operator.okl", "lambda_inverse", properties);
    fdm_operator_kernel = device.buildKernel("fdm_operator.okl", "fdm_operator", properties);
}

void FDM_Operator::build()
{
    for (int e = 0; e < num_elements; e++)
    {
        // Eigenvectors
        if (dim >= 1)
        {
            occa::memory S_x_e = S_x.slice(e * operator_size, operator_size);
            occa::memory St_x_e = St_x.slice(e * operator_size, operator_size);

            for (int i = 0; i < N; i++)
                for (int j = 0; j < N; j++)
                    work_hst_1[i * N + j] = (double)(rand()) / (double)(RAND_MAX);

            for (int i = 0; i < N; i++)
                for (int j = 0; j < N; j++)
                    work_hst_2[i * N + j] = work_hst_1[j * N + i];

            S_x_e.copyFrom(work_hst_1, operator_size * sizeof(double));
            St_x_e.copyFrom(work_hst_2, operator_size * sizeof(double));
        }

        if (dim >= 2)
        {
            occa::memory S_y_e = S_y.slice(e * operator_size, operator_size);
            occa::memory St_y_e = St_y.slice(e * operator_size, operator_size);

            for (int i = 0; i < N; i++)
                for (int j = 0; j < N; j++)
                    work_hst_1[i * N + j] = (double)(rand()) / (double)(RAND_MAX);

            for (int i = 0; i < N; i++)
                for (int j = 0; j < N; j++)
                    work_hst_2[i * N + j] = work_hst_1[j * N + i];

            S_y_e.copyFrom(work_hst_1, operator_size * sizeof(double));
            St_y_e.copyFrom(work_hst_2, operator_size * sizeof(double));
        }

        if (dim >= 2)
        {
            occa::memory S_z_e = S_z.slice(e * operator_size, operator_size);
            occa::memory St_z_e = St_z.slice(e * operator_size, operator_size);

            for (int i = 0; i < N; i++)
                for (int j = 0; j < N; j++)
                    work_hst_1[i * N + j] = (double)(rand()) / (double)(RAND_MAX);

            for (int i = 0; i < N; i++)
                for (int j = 0; j < N; j++)
                    work_hst_2[i * N + j] = work_hst_1[j * N + i];

            S_z_e.copyFrom(work_hst_1, operator_size * sizeof(double));
            St_z_e.copyFrom(work_hst_2, operator_size * sizeof(double));
        }

        // Eigenvalues
        if (dim >= 1)
        {
            occa::memory L_x_e = L_x.slice(e * N, N);

            for (int i = 0; i < N; i++)
                work_hst_1[i] = (double)(rand()) / (double)(RAND_MAX);

            L_x_e.copyFrom(work_hst_1, N * sizeof(double));
        }

        if (dim >= 2)
        {
            occa::memory L_y_e = L_y.slice(e * N, N);

            for (int i = 0; i < N; i++)
                work_hst_1[i] = (double)(rand()) / (double)(RAND_MAX);

            L_y_e.copyFrom(work_hst_1, N * sizeof(double));
        }

        if (dim >= 3)
        {
            occa::memory L_z_e = L_z.slice(e * N, N);

            for (int i = 0; i < N; i++)
                work_hst_1[i] = (double)(rand()) / (double)(RAND_MAX);

            L_z_e.copyFrom(work_hst_1, N * sizeof(double));
        }
    }

    lambda_inverse_kernel(inv_L, L_x, L_y, L_z);
}

void FDM_Operator::apply(occa::memory Su, occa::memory u)
{
    fdm_operator_kernel(Su, S_x, S_y, S_z, inv_L, u);
}
