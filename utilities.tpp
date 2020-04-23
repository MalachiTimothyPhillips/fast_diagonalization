/*
 * Utilities templates
 */

// Headers
#include <iostream>

// Templates definition
template <typename T>
void save_array(const char *name, T array, int n, int m)
{
    FILE *file = fopen(name, "w");

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            fprintf(file, "%.16g ", array[i * m + j]);
        }

        fprintf(file, "\n");
    }

    fclose(file);
}

template <typename T>
void print_array(T array, int n, int m, int p)
{
    for (int k = 0; k < p; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                printf("%8.5f ", array[i * m + j + k * n * m]);
            }

            printf("\n");
        }

        printf("\n");
    }
}
