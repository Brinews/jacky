#include <stdexcept>
#include <sstream>
/**
 * Copy constructor
 */
template <typename T>
Stack<T>::Stack(const Stack<T> &stack): size(0), top(NULL) {
    Node *tailer;
    Node *p = stack.top;
    while (p) {
        if (isEmpty()) {
            top = tailer = new Node(p->value, NULL);
        } else {
            tailer->next = new Node(p->value, NULL);
            tailer = tailer->next;
        }
        size++;
        p = p->next;
    }
}


/**
 * Default constructor
 */
template <typename T>
Stack<T>::Stack() : size(0), top(NULL) {
}

/**
 * Destructor
 */
template <typename T>
Stack<T>::~Stack() {
    while (!isEmpty()) {
        pop();
    }
}

/**
 * Swaps two stacks
 */
template <typename T>
void Stack<T>::swap(Stack<T> &stack) {
    std::swap(size, stack.size);
    std::swap(top, stack.top);
}

/**
 * Pushes value
 */
template <typename T>
void Stack<T>::push(T value) {
    top = new Node(value, top);
    size++;
}

/**
 * Removes the top value from the stack.
 */
template <typename T>
T Stack<T>::pop() {
    if (isEmpty()) {
        throw std::runtime_error("Stack empty.");
    } else {
        T value = top->value;
        Node *p = top;
        top = top->next;
        size--;
        delete p;
        return value;
    }
}
/**
 * Returns true iff the stack is empty.
 */
template <typename T>
bool Stack<T>::isEmpty() const {
    return size == 0;
}

/**
 * Returns the size of the stack.
 */
template <typename T>
unsigned int Stack<T>::getSize() const {
    return size;
}

/**
 * Returns the top value from the stack.
 * It throws a runtime error if the stack is empty.
 */
template <typename T>
T Stack<T>::peek() const {
    if (isEmpty()) {
        throw std::runtime_error("Stack empty.");
    } else {
        return top->value;
    }
}


/**
 * Returns a string which contains the values in the stack.
 */
template <typename T>
String Stack<T>::toString() const {
    std::ostringstream ostr;
    ostr << *this;
    return ostr.str().c_str();
}

/**
 * << override
 */
template <typename T>
std::ostream &operator << (std::ostream &out, Stack<T> &s) {
    typename Stack<T>::Node *pnode = s.top;
    while (pnode) {
        out << pnode->value;
        pnode = pnode->next;
        if (pnode != NULL)
            out << " ";
    }
    return out;
}

/**
 * Assignment function
 */
template <typename T>
Stack<T> &Stack<T>::operator = (Stack<T> stack) {
    // Clear the stack
    while (!isEmpty()) {
        pop();
    }
    // Copy from back to front
    Node *tailer = NULL;
    Node *p = stack.top;
    while (p) {
        if (isEmpty()) {
            top = tailer = new Node(p->value, NULL);
        } else {
            tailer->next = new Node(p->value, NULL);
            tailer = tailer->next;
        }
        size++;
        p = p->next;
    }
    return *this;
}

