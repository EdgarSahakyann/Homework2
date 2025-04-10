#include <iostream>

template <typename T>
class Set {
public:
    struct Node {
        T data;
        Node* left;
        Node* right;
        Node(const T& value) : data(value), left(nullptr), right(nullptr) {}
    };

    Set() : root(nullptr), size(0) {}

    Set(const Set& other) : root(nullptr), size(0) {
        *this = other;
    }

    Set(Set&& other) : root(other.root), size(other.size) {
        other.root = nullptr;
        other.size = 0;
    }

    ~Set() {
        clear();
    }

    void insert(const T& value) {
        Node* new_node = new Node(value);

        if (root == nullptr) {
            root = new_node;
            size++;
            return;
        }

        Node* current = root;
        Node* parent = nullptr;

        while (current != nullptr) {
            parent = current;
            if (value < current->data) {
                current = current->left;
            }
            else if (value > current->data) {
                current = current->right;
            }
            else {
                delete new_node;
                return;
            }
        }

        if (value < parent->data) {
            parent->left = new_node;
        }
        else {
            parent->right = new_node;
        }
        size++;
    }

    void remove(const T& value) {
        Node* current = root;
        Node* parent = nullptr;

        while (current != nullptr && current->data != value) {
            parent = current;
            if (value < current->data) {
                current = current->left;
            }
            else {
                current = current->right;
            }
        }

        if (current == nullptr) {
            return;
        }

        if (current->left != nullptr && current->right != nullptr) {
            Node* successor = current->right;
            Node* successorParent = current;

            while (successor->left != nullptr) {
                successorParent = successor;
                successor = successor->left;
            }

            current->data = successor->data;

            if (successorParent->left == successor) {
                successorParent->left = successor->right;
            }
            else {
                successorParent->right = successor->right;
            }
            delete successor;
        }
        else {
            Node* child = (current->left != nullptr) ? current->left : current->right;

            if (parent == nullptr) {
                root = child;
            }
            else if (parent->left == current) {
                parent->left = child;
            }
            else {
                parent->right = child;
            }
            delete current;
        }
        size--;
    }

    bool contains(const T& value) const {
        Node* current = root;

        while (current != nullptr) {
            if (value > current->data) {
                current = current->right;
            }
            else if (value < current->data) {
                current = current->left;
            }
            else {
                return true;
            }
        }

        return false;
    }

    size_t get_size() const {
        return size;
    }

    void clear() {
        while (root != nullptr) {
            remove(root->data);
        }
    }

private:
    Node* root;
    size_t size;
};

int main() {
    Set<int> s1;

    s1.insert(10);
    s1.insert(5);
    s1.insert(20);
    s1.insert(15);

    std::cout << "Size: " << s1.get_size() << std::endl;

    if (s1.contains(10)) {
        std::cout << "There is 10 in set." << std::endl;
    }

    s1.remove(5);
    std::cout << "Size after removing 5: " << s1.get_size() << std::endl;

    s1.clear();
    std::cout << "Size after clearing: " << s1.get_size() << std::endl;

    return 0;
}
