/*
 * Fast diagonalization preconditioner
 */

// Headers
#include <iostream>
#include <occa.hpp>

#include "utilities.hpp"
#include "fdm_operator.hpp"
#include <mpi.h>
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
    if(argc != 4){
        std::cout << "Usage: ./fast_diagonalization p E ntrial\n";
        exit(0);
    }
    int rank = 0, size = 1;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
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

    std::cout << "Running with : "
     << "p = " << p << "\n"
     << "N = " << N << "\n"
     << "num_elements = " << num_elements << "\n"
     << "num_tests = " << num_tests << "\n";

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
        fdm_operator.apply(Su, u);
        device.finish();
    }

    device.finish();
    MPI_Barrier(MPI_COMM_WORLD);
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_tests; i++)
    {
        fdm_operator.apply(Su, u);
        device.finish();
    }
    MPI_Barrier(MPI_COMM_WORLD);
    auto t2 = std::chrono::high_resolution_clock::now();
    const double elapsed = std::chrono::duration_cast<std::chrono::duration<double>>(t2-t1).count()/static_cast<double>(num_tests);
    const long long bytesMoved = (3*N*N*N+3*N*N)*sizeof(double);
    const double bw = (bytesMoved*num_elements/elapsed)/1.e9;
    const double flopCount = (12*N*N*N*N+N*N*N)*num_elements;
    const double gflops = (flopCount/elapsed)/1.e9;
    const double dofs = num_elements*(p+1)*(p+1)*(p+1); // don't count the extension as a true dof
    std::cout << p
      << ", " << num_elements
      << ", " << elapsed
      << ", " << gflops
      << ", " << bw
      << ", " << dofs
      << "\n";
    MPI_Finalize();
    return EXIT_SUCCESS;
}
