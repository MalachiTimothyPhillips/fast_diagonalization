/*
 * Utilities declaration
 */

// Headers

#ifndef UTILITIES_HPP
#define UTILITIES_HPP

// Function declaration
void matrix_matrix_multiplication(double*, double*, double*, int, int, int);
void two_vector_collocation(double*, double*, double*, int);
void quit();

// Templated functions
template <typename T>
void save_array(const char*, T, int, int = 1);

template <typename T>
void print_array(T, int, int = 1, int = 1);

#include "utilities.tpp"

#endif
