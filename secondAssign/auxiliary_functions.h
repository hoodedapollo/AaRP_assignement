#include <iostream>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/select.h>
#include <stdio.h>
#include <vector>

int max_positve_element(std::vector<int> v)  // maximus positive element in an integer vector
{
        int  max_fd = 0;
        for (unsigned int i=0; i < v.size(); i++)
        {
                if (v[i] > max_fd)
                {
                        max_fd = v[i];
                }
        }
        return max_fd;
}

int max_positive_in_column_2D_array( std::vector<std::vector<int> > table, int column) // return the maximum element of a single column of a 2D array of integers
{
        int max_fd = 0;
        for (unsigned int i=0; i < table.size(); i++) // for all the rows
        {
                if (table[i][column] > max_fd) // if the element of the given column and i-th row is larger than max_fd
                {
                        max_fd = table[i][column]; // update max_fd
                }
        }
        return max_fd;
}

void int_2Darray_dynamic_alloc(int ** table, int rows, int columns) // Dynamic allocate memory for a 2D array int** table with number of rows = int rows and number of colums = int colums 
{
        table = new int*[rows];
                for (int i = 0; i < rows; i++)
                {
                        table[i] = new int[columns];
                }
}

