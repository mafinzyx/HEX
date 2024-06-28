#include "Stack.h"

Stack::~Stack() {
    while (!isEmpty()) {
        pop();
    }
}

int Stack::size() const {
    int count = 0;
    Node* current = top;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

void Stack::push(int value) {
    Node* newNode = new Node(value);
    newNode->next = top;
    top = newNode;
}

int Stack::pop() {
    if (isEmpty()) {
        return 0;
    }
    int value = top->data;
    Node* temp = top;
    top = top->next;
    delete temp;
    return value;
}

int Stack::topValue() const {
    if (isEmpty()) {
        return 0;
    }
    return top->data;
}

bool Stack::isEmpty() const {
    return top == nullptr;
}