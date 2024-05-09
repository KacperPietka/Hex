#pragma once
#include <iostream>


class Stack
{
private:
    struct Node {
        int data;
        Node* next;

    };
    Node* top;
public:
    Stack() : top(nullptr) {}

    bool isEmpty() {
        return top == nullptr;
    }

    void push(int size)
    {
        Node* newNode = new Node();
        newNode->data = size;
        newNode->next = top;
        top = newNode;
    }

    int top_element() {
        return top->data;
    }

    int pop() {
        int pair = top->data;
        Node* temp = top;
        top = top->next;
        delete temp;
        return pair;
    }

    ~Stack() {
        while (!isEmpty()) {
            pop();
        }
    }
};