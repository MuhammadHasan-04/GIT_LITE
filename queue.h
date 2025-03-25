#include <iostream>

using namespace std;

template <typename t>
struct node
{
    t data;
    node *next;

    node(t val, node *n = nullptr) : data(val), next(n) {}
};

template <typename t>
class Queue
{
public:
    node<t> *front;
    node<t> *rear;
    int size;
    Queue(node<t> *f = nullptr, node<t> *r = nullptr, int s = 0) : front(f), rear(r), size(s) {}

    bool isEmpty()
    {
        if (front == nullptr && rear == nullptr)
        {
            return true;
        }

        return false;
    }
    void enqueue(t val)
    {
        if (isEmpty())
        {
            node<t> *temp = new node<t>(val);

            rear = temp;
            front = temp;
            size = 1;
        }
        else
        {
            node<t> *temp = new node<t>(val);
            rear->next = temp;
            rear = temp;
            size++;
        }
    }
    t dequeue()
    {

        if (isEmpty())
        {
            cout << "Queue is empty";
            return 0;
        }

        node<t> *temp = front;
        front = front->next;

        if (front == nullptr)
            rear = nullptr;
        size--;
        return temp->data;
    }
    t peek()
    {
        if (front)
        {
            return front->data;
        }
        return -1;
    }
    t Front()
    {
        if (front)
            return front->data;
        return nullptr;
    }
    int get_size()
    {
        return size;
    }
    void print()
    {
        node<t> *temp = front;

        while (temp)
        {
            cout << temp->data << ' ';
            temp = temp->next;
        }
        cout << endl;
    }
};