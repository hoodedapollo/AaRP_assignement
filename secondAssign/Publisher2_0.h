#include <string>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <iomanip>

using namespace std;


class Publisher {
        private:
                int publishing_period;
                int pipe_filedes_reading;
                int pipe_filedes_writing;
                char published_char;
                int pub_id;

        public:
                Publisher(int publisher_id, int pub_period, int filedes0, int filedes1); 
                void write_in_pipe(char); // generate a random char and write it in the pipe once per period
};

Publisher::Publisher (int publisher_id, int pub_period, int filedes0, int filedes1) // constructor: publisher id, publisher period
{
        pub_id = publisher_id;
        publishing_period = pub_period;

        pipe_filedes_reading = filedes0; // sets attribute accordingly
        pipe_filedes_writing = filedes1; // sets attribute accordingly
        close(filedes0); // closes pipe reading file descriptor
}

void Publisher::write_in_pipe (char Aa) // generate a random char, capitol letter or not depending on Aa, and write it in the pipe once per period
{
        char randomletter;
        while(1)
        {
                sleep(publishing_period);
                if (Aa == 'a')
                {
                        randomletter = 'a' + (random() % 26);
                }
                if (Aa == 'A')
                {
                        randomletter = 'A' + (random() % 26);
                }
                int write_ret = write(pipe_filedes_writing, &randomletter, sizeof(randomletter));
                if (write_ret < 0)
                {
                        perror("pub write");
                }
                cout << "PUB" << pub_id << " --> " << randomletter << endl;
        }
}

