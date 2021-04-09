//
// Created by Chase on 4/7/2021.
//

template <typename T>
class DynamicStack {
private:
    struct Node {
        T value;
        Node *next;

        explicit Node(T d, Node* n = nullptr){
            value = d;
            next = n;
        }
    };

    Node *head;

public:
    DynamicStack(){
        head = nullptr;
    }

    ~DynamicStack(){
        Node *ptr = head;
        while (ptr != nullptr){
            Node *current = ptr;
            ptr = ptr->next;
            delete current;
        }

        head = nullptr;
    }

    void push(T value); // Inserts into stack

    T pop(); // Removes last inserted element

    T peek(); // Returns last inserted element;

    int size();

    bool isEmpty();
};

template<typename T>
void DynamicStack<T>::push(T value) {
    Node *node = new Node(value, head);
    head = node;
}

template<typename T>
T DynamicStack<T>::pop() {
    if (head == nullptr){
        return NULL;
    }

    Node *node = head;
    head = head->next;

    T value = node->value;
    delete node;

    return value;
}

template<typename T>
T DynamicStack<T>::peek() {
    return head->value;
}

template<typename T>
int DynamicStack<T>::size() {
    int size = 0;
    Node *ptr = head;

    while (ptr != nullptr){
        ptr = ptr->next;
        size++;
    }

    return size;
}

template<typename T>
bool DynamicStack<T>::isEmpty() {
    return head == nullptr;
}
