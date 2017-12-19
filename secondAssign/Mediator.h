#include <unistd.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>

using namespace std;

class Mediator 
{
        private:
                int num_circular_buffer;
                int *pubs_readfrom_filedes;
                int *subs_writeto_fildes;
                int *subs_notify_readfrom_fildes;
        public:
                Mediator(int);
                void set_pubs_fildes(int*);
                void set_subs_read_fildes(int*);
                void set_subs_notify_fildes(int*);
                char read_pubs();
                void check_subs_notify();
                void write_subs();
}

