//source --> https://www.programiz.com/dsa/circular-queue

#include <iostream>
#include <string.h>

using namespace std;

class Queue {
        private:
                int rear, buffer_size, subs_num, empty_counter;
                int *front;
                char *items;

        public:
                Queue();
                Queue(int buffer_size, int num_of_subs);
                ~Queue();
                void set_attributes(int buffer_size, int num_of_subs);
                bool isFull();
                bool isEmpty();
                void enQueue(int element);
                char deQueue(int sub_id);
                void display();
};

Queue::Queue()
{
        empty_counter = 0;
}

Queue::Queue(int buff_size, int num_of_subs)
{
        empty_counter = 0;
        subs_num = num_of_subs;    
        buffer_size = buff_size;
        items = new char[buff_size];
        front = new int[num_of_subs];
        rear = -1;
        for (int i = 0; i < subs_num; i++)
        {
                front[i] = -1;
        }
}

void Queue::set_attributes(int buffer_size, int num_of_subs)
{
        subs_num = num_of_subs;    
        items = new char[buffer_size];
        front = new int[num_of_subs];
        rear = -1;
        for (int i = 0; i < subs_num; i++)
        {
                front[i] = -1;
        }
}

Queue::~Queue()
{
        delete front;
        delete items;
}

bool Queue::isFull()
{
        for (int i = 0; i < subs_num; i++)
        {
                if(front[i] == 0 && rear == buffer_size - 1) 
                {
                        return true;
                }
                if(front[i] == rear + 1)
                {
                        return true;
                }
        }
        return false;
}

bool Queue::isEmpty()
{
        for (int i = 0; i < subs_num; i++)
        {
                if(front[i] != -1)
                {
                        return false;
                }
        }
        return true;
}

void Queue::enQueue(int element) // add element to the buffer
{
        if(isFull())
        {
                for (int i = 0; i < subs_num; i++)
                {
                        front[i] = (front[i] + 1) % buffer_size; // update the front pointer relative to the sub_id subscriber
                }
        }


        for (int i = 0; i < subs_num; i++) // for all subscribers
        {
                if(front[i] == -1) // if the front pointer relative to the i-th subscriber is -1 
                {
                        front[i] = 0;
                }
        }
        rear = (rear + 1) % buffer_size; // update the rear pointer (it is common to all subscribers)
        items[rear] = element; // add the element to the buffer in the position pointed by the updated pointer rear
        //cout << endl << "Inserted " << elemnt << endl; 

}

char Queue::deQueue(int sub_id) // returns the next element in the queue and updates the front pointer relative to the subs_id subscriber    
{
        int element;
        if (isEmpty())
        {
                cout << "Queue is empty" << endl;
                element = -1; 
        }
        else
        {
                element = items[front[sub_id]]; // store the element pointed by the front pointer of the subs_id subscriber in element
                if (front[sub_id] == rear)
                {
                        empty_counter++;
                }
                else
                {
                        front[sub_id] = (front[sub_id] + 1) % buffer_size; // update the front pointer relative to the sub_id subscriber
                }

                if (empty_counter == subs_num) // if all the front pointer of the subscribers are equal to the rear pointer empty the queue
                {
                        for (int i = 0; i < subs_num; i++)
                        {
                                front[i] = -1;
                        }
                        rear = -1;
                        empty_counter = 0;
                }
        }
        return(element);
} 

void Queue::display()
{
        int i, j;
        if (isEmpty()) 
        {
                cout << endl << "Empty Queue" << endl;
        }
        else
        {
                for(i = 0; i < subs_num; i++)
                {
                        cout << endl << "SUB" << i << endl; 
                        {
                                cout << "Front --> " << front[i] << endl;
                                cout << "Items:" << endl;
                                for (j = front[i]; j != rear; j = (j+1) % buffer_size)
                                {
                                        cout << string(3, ' ') << items[j] << endl;
                                }
                                cout << string(3, ' ') << items[j] << endl;
                        }

                }
                cout << endl << "Rear --> " << rear << endl << endl;
        }
}


