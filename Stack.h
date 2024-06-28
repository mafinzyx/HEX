#pragma once
#include <iostream>
#include <string.h>
#include <vector>

using namespace std;

struct Node {
    int data;
    Node* next;

    Node(int value) : data(value), next(nullptr) {}
    ~Node() {}
};

class Stack {
private:
    Node* top;

public:
    Stack() { top = nullptr; }
    ~Stack();
    void push(int value);
    int pop();
    int topValue() const;
    bool isEmpty() const;
    int size() const;
};