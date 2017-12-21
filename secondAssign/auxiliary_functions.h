#include <iostream>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/select.h>


int max_positve_element(int *array)  // maximus positive element in an integer vector
{
        int  max_fd = 0;
        int size = sizeof(array)/sizeof(int); // number of elements in the array
        for (int i=0; i < size; i++)
        {
                if (array[i] > max_fd)
                {
                        max_fd = array[i];
                }
        }
        return max_fd;
}

int max_positive_in_column_2D_array(int **table, int column) // return the maximum element of a single column of a 2D array of integers
{
        int max_fd = 0;
        int size = sizeof(table)/( column * sizeof(int)); // number of elements in a column (number of rows)
        for (int i=0; i < size; i++)
        {
                if (table[i][column] > max_fd)
                {
                        max_fd = table[i][column];
                }
        }
        return max_fd;
}
