#include <iostream>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/select.h>
#include <stdio.h>
#include <vector>

using namespace std;

#define NCHAR_FOR_INT 6

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

// char** fromVecToCharReadingFds(std::vector<std::vector<std::vector<int> > > fildes_3Dtable)
// {
//         int subs_num = fildes_3Dtable.size();
//         int pubs_num = fildes_3Dtable[0].size();
//         char** array;
//         int array_length = (2 + subs_num * pubs_num);
//         array = new char*[array_length];
//         for (int i = 0; i < array_length; i++)
//         {
//                 array[i] = new char[NCHAR_FOR_INT];
//         }
//         sprintf(array[0], "%d", subs_num);
//         sprintf(array[1], "%d", pubs_num);
//         int q = 2;
//         for (int i = 0; i < subs_num; i++)
//         {
//             for (int j = 0; j < pubs_num; j++)
//             {
//                     sprintf(array[q],"%d", fildes_3Dtable[i][j][0]);
//                     q++;
//             }
//         }
//         q--;
// }
//
// char** fromVecToCharWritingFds(std::vector<std::vector<std::vector<int> > > fildes_3Dtable)
// {
//         int subs_num = fildes_3Dtable.size();
//         int pubs_num = fildes_3Dtable[0].size();
//         char** array;
//         int array_length = (subs_num * pubs_num);
//         array = new char*[array_length];
//         for (int i = 0; i < array_length; i++)
//         {
//                 array[i] = new char[NCHAR_FOR_INT];
//         }
//         int q = 0;
//         for (int i = 0; i < subs_num; i++)
//         {
//             for (int j = 0; j < pubs_num; j++)
//             {
//                     sprintf(array[q],"%d", fildes_3Dtable[i][j][0]);
//                     q++;
//             }
//         }
// }


// vector<vector<vector<int> > > fromCharToVecFds(char* reading_filedes[], char* writing_filedes[])
// {
//        subs_num = atoi(reading_filedes[0]);
//        pubs_num = atoi(reading_filedes[1]);
//
//        vector<int> fd_row(2,0);
//        vector<vector<int> > pubs_row(pubs_num, fd_row);
//        vector<vector<vector<int> > > fildes_3Dtable(subs_num, pubs_row);
//        int q = 2;
//                for (int i = 0; i < subs_num; i++)
//                {
//                        for (int j = 0; j < pubs_num; j++)
//                        {
//                                fildes_3Dtable[i][j][0] = atoi(reading_filedes[q]);
//                                q++;
//                        }
//                }
//                q=0;
//                for (int i = 0; i < subs_num; i++)
//                {
//                        for (int j = 0; j < pubs_num; j++)
//                        {
//                                fildes_3Dtable[i][j][1] = atoi(writing_filedes[q]);
//                                q++;
//                        }
//                }
// }


char** subsFdsFrom3DVecToStrArray(vector<vector<vector<int> > > filedes_3Dtable) // convert a 3Dvector of file descriptors into an array of string where each string is a file descriptor, first two element of the array are the number of subscribers and the number of publishers 
{
        int subs_num = filedes_3Dtable.size(); // number of rows of the table corresponds to the number of subscribers
        int pubs_num = filedes_3Dtable[0].size(); // number of columns of the table corresponds to the number of publishers

        char** array; 
        int array_length = (2 + 2*(subs_num * pubs_num)); // allocate memory dynamically to store all the file descriptors + two element for the number of subscribers and the number of publishers
        array = new char*[array_length];
        for (int i = 0; i < array_length; i++)
        {
                array[i] = new char[NCHAR_FOR_INT];
        }
        sprintf(array[0], "%d", subs_num); // store the number of subscribers in the first element of the array 
        sprintf(array[1], "%d", pubs_num); // store the number of subscribers in the first element of the array 
        int q = 2; // start to store the file descriptors from the third element since the first two are no more available 
        for (int i = 0; i < subs_num; i++)
        {
            for (int j = 0; j < pubs_num; j++)
            {
                    sprintf(array[q],"%d", filedes_3Dtable[i][j][0]); // store the reading file descriptor converting it into a string
                    q++;
            }
        }

        for (int i = 0; i < subs_num; i++)
        {
            for (int j = 0; j < pubs_num; j++)
            {
                    sprintf(array[q],"%d", filedes_3Dtable[i][j][1]); // store the writing file descriptor converting it into a string
                    q++;
            }
        } 
        return array;
}

char** pubsFdsFrom2DVecToStrArray(vector<vector<int> >  filedes_2Dtable) // convert a 3Dvector of file descriptors into an array of string where each string is a file descriptor, first two element of the array are the number of subscribers and the number of publishers 
{
        int pubs_num = filedes_2Dtable.size(); // number of rows of the table corresponds to the number of subscribers

        char** array; 
        int array_length = (1 + 2*pubs_num); // allocate memory dynamically to store all the file descriptors + two element for the number of subscribers and the number of publishers
        array = new char*[array_length];
        for (int i = 0; i < array_length; i++)
        {
                array[i] = new char[NCHAR_FOR_INT];
        }
        sprintf(array[0], "%d", pubs_num); // store the number of subscribers in the first element of the array 
        int q = 1; // start to store the file descriptors from the third element since the first two are no more available 
            for (int i = 0; i < pubs_num; i++)
            {
                    sprintf(array[q],"%d", filedes_2Dtable[i][0]); // store the reading file descriptor converting it into a string
                    q++;
            }
        

        
            for (int i = 0; i < pubs_num; i++)
            {
                    sprintf(array[q],"%d", filedes_2Dtable[i][1]); // store the writing file descriptor converting it into a string
                    q++;
            }
         
        return array;
}

vector<vector<vector<int> > > fromStrArrayTo3DVecFds(char* filedes[]) // build a 3Dvector of fildescriptors from an array which stores th number of subscribers and publishers and all the file descriptors
{
       int subs_num = atoi(filedes[0]); 
       int pubs_num = atoi(filedes[1]);

       vector<int> fd_row(2,0);
       vector<vector<int> > pubs_row(pubs_num, fd_row);
       vector<vector<vector<int> > > filedes_3Dtable(subs_num, pubs_row);
       int q = 2;
               for (int i = 0; i < subs_num; i++)
               {
                       for (int j = 0; j < pubs_num; j++)
                       {
                               filedes_3Dtable[i][j][0] = atoi(filedes[q]); 
                               q++;
                       }
               }
               
               for (int i = 0; i < subs_num; i++)
               {
                       for (int j = 0; j < pubs_num; j++)
                       {
                               filedes_3Dtable[i][j][1] = atoi(filedes[q]); 
                               q++;
                       }
               }
               return filedes_3Dtable;
}
               
        
vector<vector<int> > fromStrArrayTo2DVecFds(char* filedes[]) // build a 2Dvector of fildescriptors from an array which stores the number of publishers and all the file descriptors of their pipes
{
        int pubs_num = atoi(filedes[0]); 

        vector<int> fd_row(2,0);
        vector<vector<int> > filedes_2Dtable(pubs_num, fd_row);
        int q = 1;
        for (int i = 0; i < pubs_num; i++)
        {
                filedes_2Dtable[i][0] = atoi(filedes[q]); 
                q++;
        }


        for (int i = 0; i < pubs_num; i++)
        {
                filedes_2Dtable[i][1] = atoi(filedes[q]); 
                q++;
        }
        return filedes_2Dtable;
}


char** from2DVecToStrArrayMatchingTable(vector<vector<int> >  matching_table) // convert a 3Dvector of file descriptors into an array of string where each string is a file descriptor, first two element of the array are the number of subscribers and the number of publishers 
{
        char** array; 

        int subs_num = matching_table.size(); // number of rows of the table corresponds to the number of subscribers
        int pubs_num = matching_table[0].size(); // number of colums corresponds to the number of publishers
        int array_length = 2 + subs_num * pubs_num;
        array = new char*[array_length]; // allocate memory dynamically to store all the file descriptors + two element for the number of subscribers and the number of publishers
        for (int i = 0; i < array_length; i++)
        {
                array[i] = new char[NCHAR_FOR_INT];
        }

        sprintf(array[0], "%d", subs_num);
        sprintf(array[1], "%d", pubs_num);

        int q = 2;
            for (int i = 0; i < subs_num; i++)
            {
                    for (int j = 0; j < pubs_num; j++)
                    {
                    sprintf(array[q],"%d", matching_table[i][j]); // store the reading file descriptor converting it into a string
                    q++;
                    }
            }
         
        return array;
}

vector<vector<int> > fromStrArrayTo2DVecMatchingTable(char* matching_array[]) // build a 2Dvector of fildescriptors from an array which stores the number of publishers and all the file descriptors of their pipes
{
        int subs_num = atoi(matching_array[0]); 
        int pubs_num = atoi(matching_array[1]);

        vector<int> row(pubs_num,0);
        vector<vector<int> > matching_table(subs_num, row);
        int q = 2;
        for (int i = 0; i < subs_num; i++)
        {
                for (int j = 0; j < pubs_num; j++)
                {
                        matching_table[i][j] = atoi(matching_array[q]); 
                        q++;
                }
        }

        return matching_table;
}
