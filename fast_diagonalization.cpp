/*
 * Fast diagonalization preconditioner
 */

// Headers
#include <iostream>
#include <occa.hpp>

#include "utilities.hpp"
#include "fdm_operator.hpp"
#include <chrono>

// Namespaces
using namespace std;

// Global variable
int dim;
int N;
int p;
int num_elements;
int num_tests;
int num_points_elem;
int num_points_total;

double *work_hst_1;
double *work_hst_2;
occa::memory work_dev_1;
occa::memory work_dev_2;


// Main program
int main(int argc, char *argv[])
{
    // Initialize OCCA
    occa::device device;
    device.setup("mode: 'CUDA', device_id: 1");

    // Runtime parameters
    dim = 3;
    p = atoi(argv[1]);
    N = p+3;
    num_elements = atoi(argv[2]);
    num_tests = atoi(argv[3]);
    num_points_elem = (dim == 2) ? N * N : N * N * N;
    num_points_total = num_elements * num_points_elem;

    work_hst_1 = new double[num_points_total];
    work_hst_2 = new double[num_points_total];
    work_dev_1 = device.malloc<double>(num_points_total);
    work_dev_2 = device.malloc<double>(num_points_total);

    // FDM Operator
    FDM_Operator fdm_operator(dim, num_elements, N, device);

    fdm_operator.build();

    // Initialize field
    occa::memory u = device.malloc<double>(num_points_total);

    for (int i = 0; i < num_points_total; i++)
        work_hst_1[i] = (double)(rand()) / (double)(RAND_MAX);

    u.copyFrom(work_hst_1, num_points_total * sizeof(double));

    // Apply the FDM operator
    occa::memory Su = device.malloc<double>(num_points_total);

    // spin the device
    for (int i = 0; i < num_tests/10; i++)
    {
        fdm_operator.apply(Su, u, false);
        device.finish();
    }

    device.finish();
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_tests; i++)
    {
        fdm_operator.apply(Su, u);
        device.finish();
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    auto time = std::chrono::duration_cast<std::chrono::microseconds>(t2-t1).count()/static_cast<double>(num_tests);
    return EXIT_SUCCESS;
}
