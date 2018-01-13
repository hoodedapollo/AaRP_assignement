#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include "../CircularBuffer.h"

using namespace std;

#define BUFFER_SIZE 3 
#define SUBS_NUM 2
int main ()
{
        Queue buffer;
        buffer.set_attributes(BUFFER_SIZE, SUBS_NUM);

        buffer.display();

        buffer.enQueue('A');
        buffer.enQueue('b');
        buffer.enQueue('C');

        buffer.display();

        buffer.enQueue('d');

        buffer.display();

        char sub1_first = buffer.deQueue(1);
        cout << "sub1_first " << sub1_first << endl;

        buffer.display();

        char sub0_first = buffer.deQueue(0);
        cout << "sub0_first " << sub0_first << endl;
            
        buffer.display();

        char sub1_second = buffer.deQueue(1);
        cout << "sub1_second " << sub1_second << endl;

        buffer.display();

        char sub0_second = buffer.deQueue(0);
        cout << "sub0_second " << sub0_second << endl;

        buffer.display();

        char sub0_third = buffer.deQueue(0);
        cout << "sub0_third " << sub0_third << endl;

        buffer.display();

        char sub0_fourth = buffer.deQueue(0);
        cout << "sub0_fourth " << sub0_fourth << endl;

        buffer.display();

        char sub1_third = buffer.deQueue(1);
        cout << "sub1_third " << sub1_third << endl; 

        buffer.display();

        char sub1_fourth = buffer.deQueue(1);
        cout << "sub1_fourth " << sub1_fourth << endl; 

        buffer.display();

} 
