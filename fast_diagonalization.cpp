/*
 * Fast diagonalization preconditioner
 */

// Headers
#include <iostream>
#include <occa.hpp>

#include "utilities.hpp"
#include "fdm_operator.hpp"
#include "timer.hpp"

// Namespaces
using namespace std;

// Global variable
int dim;
int N;
int num_elements;
int num_points_elem;
int num_points_total;

double *work_hst_1;
double *work_hst_2;
occa::memory work_dev_1;
occa::memory work_dev_2;

Timer timer;

// Main program
int main(int argc, char *argv[])
{
    // Initialize OCCA
    occa::device device;
    device.setup("mode: 'CUDA', device_id: 0");

    timer.set_device(device);

    // Runtime parameters
    dim = 3;
    N = atoi(argv[1]);
    num_elements = atoi(argv[2]);
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

    int num_tests = 10;

    for (int i = 0; i < num_tests; i++)
    {
        timer.reset("fdm");
        timer.start("fdm");
        fdm_operator.apply(Su, u, false);
        timer.stop("fdm");

        printf("FDM time: %8.4e\n", timer.total("fdm"));
    }

    // End of program
    return EXIT_SUCCESS;
}
