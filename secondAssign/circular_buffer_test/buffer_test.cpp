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
        Queue buffer(BUFFER_SIZE, SUBS_NUM);

        buffer.display();

        buffer.enQueue(10);
        buffer.enQueue(20);
        buffer.enQueue(30);
        buffer.enQueue(40);
        buffer.display();

        int sub1_first = buffer.deQueue(1);
        cout << "sub1_first " << sub1_first << endl;

        buffer.display();

        int sub0_first = buffer.deQueue(0);
        cout << "sub0_first " << sub0_first << endl;
            
        buffer.display();

        int sub1_second = buffer.deQueue(1);
        cout << "sub1_second " << sub1_second << endl;

        buffer.display();

        int sub0_second = buffer.deQueue(0);
        cout << "sub0_second " << sub0_second << endl;

        buffer.display();

        int sub0_third = buffer.deQueue(0);
        cout << "sub0_third " << sub0_third << endl;

        buffer.display();

        int sub1_third = buffer.deQueue(1);
        cout << "sub1_third " << sub1_third << endl; 

        buffer.display();

        int sub0_fourth = buffer.deQueue(0);
        cout << "sub0_fourth " << sub0_fourth << endl;

} 
