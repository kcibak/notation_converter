#include "NotationConverter.hpp"
#include <string>
#include <iostream>
#include <deque>

// ==========================
// Doubly Linked List Node
// ==========================
template <typename T>
struct Node { // Node struct to store data and links
    T data;
    Node* next;
    Node* prev;
    Node(const T& data) : data(data), prev(nullptr), next(nullptr) {}
};

// ==========================
// Custom Deque Class
// ==========================
template <typename T>
class deque { // Deque implemented using a doubly linked list
private:
    Node<T>* head; // Pointer to front node
    Node<T>* tail; // Pointer to back node
    int length;    // Keeps track of size

public:
    // Constructor
    deque() : head(nullptr), tail(nullptr), length(0) {} // Initializes an empty deque

    // Destructor
    ~deque() {
        clear();
    }

    // Pushes an element to the front of the deque
    void push_front(const T& item) {
        Node<T>* newNode = new Node<T>(item);
        if (head == nullptr) { // If deque is empty
            head = newNode;
            tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        ++length;
    }

    // Pushes an element to the back of the deque
    void push_back(const T& item) {
        Node<T>* newNode = new Node<T>(item);
        if (head == nullptr) { // If deque is empty
            head = newNode;
            tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        ++length;
    }

    // Removes the first element of the deque
    void pop_front() {
        if (head == nullptr) return; // If deque is empty, do nothing
        Node<T>* temp = head;
        head = head->next;
        if (head == nullptr) { // If the deque is now empty
            tail = nullptr;
        } else {
            head->prev = nullptr;
        }
        delete temp;
        --length;
    }

    // Removes the last element of the deque
    void pop_back() {
        if (tail == nullptr) return; // If deque is empty, do nothing
        Node<T>* temp = tail;
        tail = tail->prev;
        if (tail == nullptr) { // If the deque is now empty
            head = nullptr;
        } else {
            tail->next = nullptr;
        }
        delete temp;
        --length;
    }

    // Returns the first element of the deque
    T front_item() {
        return head->data;
    }

    // Returns the last element of the deque
    T back_item() {
        return tail->data;
    }

    // Clears the deque
    void clear() {
        while (head != nullptr) {
            Node<T>* temp = head;
            head = head->next;
            delete temp;
        }
        tail = nullptr;
        length = 0;
    }

    // Returns the size of the deque
    int size() {
        return length;
    }

    // Returns true if the deque is empty
    bool empty() {
        return length == 0;
    }
};

// ==========================
// NotationConverter Class
// ==========================
class NotationConverter : public NotationConverterInterface {
private:
    // Returns true if the character is an operand (A-Z, a-z)
    bool isOperand(char c) {
        return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
    }

    // Returns true if the character is an operator (+, -, *, /)
    bool isOperator(char c) {
        return (c == '+' || c == '-' || c == '*' || c == '/');
    }

public:
    // Converts postfix expression to infix
    std::string postfixToInfix(std::string inStr) {
        using std::deque;
        deque<std::string> d;

        for (int i = 0; i < inStr.length(); i++) {
            if (isOperand(inStr[i])) {
                d.push_back(std::string(1, inStr[i]));
            } else if (isOperator(inStr[i])) {
                std::string op2 = d.back(); d.pop_back();
                std::string op1 = d.back(); d.pop_back();
                std::string temp = "(" + op1 + " " + inStr[i] + " " + op2 + ")";
                d.push_back(temp);
            }
        }
        return d.back();
    }

    // Converts postfix to prefix by first converting to infix
    std::string postfixToPrefix(std::string inStr) {
        return infixToPrefix(postfixToInfix(inStr));
    }

    // Converts infix to postfix
    std::string infixToPostfix(std::string inStr) {
        using std::deque;
        deque<std::string> d;

        for (int i = inStr.size() - 1; i >= 0; --i) {
            std::string temp(1, inStr[i]);
            if (temp == " ") continue;
            if (temp == ")") d.push_front(temp);
            else if (temp == "(") {
                while (!d.empty() && d.front() != ")") {
                    std::string op = d.front(); d.pop_front();
                    std::string op1 = d.back(); d.pop_back();
                    std::string op2 = d.back(); d.pop_back();
                    d.push_back(op1 + " " + op2 + " " + op);
                }
                if (!d.empty() && d.front() == ")") d.pop_front();
            } else if (isOperand(temp[0])) d.push_back(temp);
            else if (isOperator(temp[0])) d.push_front(temp);
        }
        return d.back();
    }

    // Converts infix to prefix
    std::string infixToPrefix(std::string inStr) {
        return infixToPostfix(inStr); // Prefix is handled similarly
    }

    // Converts prefix to infix
    std::string prefixToInfix(std::string inStr) {
        return postfixToInfix(prefixToPostfix(inStr));
    }

    // Converts prefix to postfix
    std::string prefixToPostfix(std::string inStr) {
        using std::deque;
        deque<std::string> d;

        for (int i = inStr.size() - 1; i >= 0; i--) {
            std::string temp(1, inStr[i]);
            if (temp == " ") continue;

            if (isOperand(temp[0])) d.push_front(temp);
            else if (isOperator(temp[0])) {
                std::string op1 = d.front(); d.pop_front();
                std::string op2 = d.front(); d.pop_front();
                d.push_front(op1 + " " + op2 + " " + temp);
            }
        }
        return d.front();
    }
};
