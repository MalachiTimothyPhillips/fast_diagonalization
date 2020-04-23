/*
 * FDM Operator class declaration
 */

// Headers
#include <occa.hpp>

// Class definition
#ifndef FDM_HPP
#define FDM_HPP

class FDM_Operator
{
    private:
        // Mesh data
        int dim;
        int num_elements;
        int N;
        int operator_size;
        int num_points_total;
        int num_points_elem;

        // OCCA
        occa::device device;
        occa::properties properties;

        // Eigenvectors
        occa::memory S_x;
        occa::memory S_y;
        occa::memory S_z;

        occa::memory St_x;
        occa::memory St_y;
        occa::memory St_z;

        // Eigenvalues
        occa::memory L_x;
        occa::memory L_y;
        occa::memory L_z;
        occa::memory inv_L;

        // Kernels
        occa::kernel lambda_inverse_kernel;
        occa::kernel two_vector_collocation_kernel;
        occa::kernel tensor_x_kernel;
        occa::kernel tensor_y_kernel;
        occa::kernel tensor_z_kernel;
        occa::kernel tensor_z_plus_collocation_kernel;
        occa::kernel fdm_operator_kernel;

    public:
        // Constructor
        FDM_Operator(int, int, int, occa::device);

        // Functions
        void build();
        void apply(occa::memory, occa::memory, bool = false);
};

#endif
