#include "NotationConverter.hpp"
#include <string>
#include <iostream>

template <typename T>
struct Node{ //Node struct
    T data;
    Node* next;
    Node* prev;
    Node(const T& data) : data(data), prev(nullptr), next(nullptr) {}
};

template <typename T>
class deque { //Deque class
private:
    Node<T> *head;
    Node<T> *tail;
    int length;

public:
    // Constructor
    deque() : head(nullptr), tail(nullptr), length(0) {} // Inline constructor

    // Destructor
    ~deque() {
        clear();
    }

    // Pushes an element to the front of the deque
    void push_front(const T &item) {
        Node<T> *newNode = new Node<T>(item);
        if (head == nullptr) {
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
    void push_back(const T &item) {
        Node<T> *newNode = new Node<T>(item);
        if (head == nullptr) {
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
        if (head == nullptr) {
            return;
        }
        Node<T> *temp = head;
        head = head->next;
        if (head == nullptr) {
            tail = nullptr;
        } else {
            head->prev = nullptr;
        }
        delete temp;
        --length;
    }

    // Removes the last element of the deque
    void pop_back() {
        if (tail == nullptr) {
            return;
        }
        Node<T> *temp = tail;
        tail = tail->prev;
        if (tail == nullptr) {
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

    // Returns the last element of the deque4
    T back_item() {
        return tail->data;
    }

    // Clears the deque
    void clear() {
        while (head != nullptr) {
            Node<T> *temp = head;
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

    bool empty(){ //Returns true if the deque is empty
        return length == 0;
    }
};


class NotationConverter : public NotationConverterInterface {
private:
    // returns true if the character is an operand
    bool isOperand(char c){
        return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
    }
    // returns true if the character is an operator
    bool isOperator(char c){
        return (c == '+' || c == '-' || c == '*' || c == '/');
    }

public:
    
    std::string postfixToInfix(std::string inStr) override {
        //Create a deque
        deque<std::string> d;

        //Iterate through the string
        for(int i = 0; i < inStr.length(); i++){
            //If the character is an operand, push it to the deque
            if(isOperand(inStr[i])){
                d.push_back(std::string(1, inStr[i]));
            }
            //If the character is an operator, pop the last two operands from the deque
            else if(isOperator(inStr[i])){
                std::string op2 = d.back_item();
                d.pop_back();

                std::string op1 = d.back_item();
                d.pop_back();

                std::string temp = "(" + op1 + " " + inStr[i] + " " + op2 + ")";
                d.push_back(temp);
            }
        }
        return d.back_item();
    }

    std::string postfixToPrefix(std::string inStr) override {
        return infixToPrefix(postfixToInfix(inStr));
    }

    std::string infixToPostfix(std::string inStr) override {
        deque<std::string> d; // (A + B) -> A B +

         // Traverse the string from right to left
        for (int i = inStr.size() - 1; i >= 0; --i) {
            std::string temp = std::string(1, inStr[i]);
             // If the character is an operand, push it to the deque
            if (temp == " ") {
                continue;
            }
            if (temp == ")") {
                d.push_front(temp);
            }
                 // If the character is an operator, pop the last two d from the deque and
                 // push the expression formed by the operator and the two d.
            else if (temp == "(") {
                 // Pop the d2 until the closing parenthesis is found
                while (!d.empty() && d.front_item() != ")") {
                    std::string op = d.front_item();
                    d.pop_front();

                    std::string op1 = d.back_item();
                    d.pop_back();

                    std::string op2 = d.back_item();
                     d.pop_back();

                     std::string expr = op1 + " " + op2 + " " + op;
                    d.push_back(expr);
                }
                 // Pop the closing parenthesis
                if (!d.empty() && d.front_item() == ")") {
                    d.pop_front();
                }
            }
                 // If the character is an operand, push it to the deque
            else if (isOperand(temp[0])) {
                d.push_back(temp);
            }
                 // If the character is an operator, push it to the deque
            else if (isOperator(temp[0])) {
                d.push_front(temp);
            }
        }
         // Return the last element of the deque
         return d.back_item();
    }

    std::string infixToPrefix(std::string inStr) override {
        deque<std::string> d;

         // Traverse the string from right to left
        for (int i = inStr.size() - 1; i >= 0; --i) {
            std::string temp = std::string(1, inStr[i]);

            if (temp == " ") {
                continue;
            }
            if (temp == ")") {
                d.push_front(temp);
            }
                 // If the character is an operator, pop the last two d from the deque and
                 // push the expression formed by the operator and the two d.
            else if (temp == "(") {
                 // Pop the d2 until the closing parenthesis is found
                while (!d.empty() && d.front_item() != ")") {
                    std::string op = d.front_item();
                    d.pop_front();

                    std::string op1 = d.back_item();
                    d.pop_back();

                    std::string op2 = d.back_item();
                    d.pop_back();

                    std::string expr = op + " " +op1 + " " + op2;
                    d.push_back(expr);
                }
                 // Pop the closing parenthesis
                if (!d.empty() && d.front_item() == ")") {
                    d.pop_front();
                }
            }
                 // If the character is an operand, push it to the deque
            else if (isOperand(temp[0])) {
                d.push_back(temp);
            }
                 // If the character is an operator, push it to the deque
            else if (isOperator(temp[0])) {
                d.push_front(temp);
            }
        }
         // Return the last element of the deque
        return d.back_item();
    }

    std::string prefixToInfix(std::string inStr) override {
         return postfixToInfix(prefixToPostfix(inStr));
    }

    std::string prefixToPostfix(std::string inStr) override {//+ab->ab+
        deque<std::string> d;

        for (int i = inStr.size() - 1; i >= 0; i--) {
            std::string temp = std::string(1, inStr[i]);

        if (temp == " ") { // Skip spaces
                continue;
            }

        if (isOperand(temp[0])) {
            // Push the operand onto the deque
            d.push_front(temp);
        } else if (isOperator(temp[0])) {
            // Pop two elements from the deque
            std::string op1 = d.front_item();
            d.pop_front();
            std::string op2 = d.front_item();
            d.pop_front();
            // Form the new postfix expression and push it onto the deque
            std::string out = op1 +" " +op2 +" "+ temp;
            d.push_front(out);
        }
    }
    // The final element of the deque is the postfix expression
    return d.front_item();
    }
};