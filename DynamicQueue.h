//
// Created by Chase on 4/7/2021.
//

template <typename T>
class DynamicQueue {
private:
    struct Node {
        T value;
        Node *next;

        explicit Node(T d){
            value = d;
            next = nullptr;
        }
    };

    Node *head;
    Node *tail;

public:
    DynamicQueue(){
        head = nullptr;
        tail = nullptr;
    }

    ~DynamicQueue(){
        Node *ptr = tail;
        while (ptr != nullptr){
            Node *current = ptr;
            ptr = ptr->next;
            delete current;
        }

        head = nullptr;
        tail = nullptr;
    }

    void push(T value); // Inserts into stack

    T pop(); // Removes first inserted element

    T front(); // Returns first inserted element;

    int size();

    bool isEmpty();
};

/*
 *   <-- next
 *   ---> previous
 *
 * 20 (head) <- 15 <- 10 <- 5 (tail)
 */

template<typename T>
void DynamicQueue<T>::push(T value) {
    Node *node = new Node(value);

    if (head != nullptr){
        head->next = node;
    }

    head = node;

    if (tail == nullptr){
        tail = head;
    }
}

template<typename T>
T DynamicQueue<T>::pop() {
    if (tail == nullptr){
        return NULL;
    }

    Node *node = tail;
    tail = node->next;

    T value = node->value;
    delete node;

    return value;
}

template<typename T>
T DynamicQueue<T>::front() {
    return &tail->value;
}

template<typename T>
int DynamicQueue<T>::size() {
    int size = 0;
    Node *ptr = tail;

    while (ptr != nullptr){
        ptr = ptr->next;
        size++;
    }

    return size;
}

template<typename T>
bool DynamicQueue<T>::isEmpty() {
    return tail == nullptr;
}