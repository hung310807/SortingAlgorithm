#include <iostream>
using namespace std;

struct Node {
    int value;     
    Node* left;     
    Node* right;   

    // constructor
    Node(int v, Node* l = nullptr, Node* r = nullptr) {
        value = v;
        left  = l;
        right = r;
    }
};

class BSTree {
private:
    Node* root;

    void makeEmpty(Node*& t) {
        if (t == nullptr) return;
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
        t = nullptr;
    }

    void insert(int val1, Node*& t) {
        if (t == nullptr) {
            t = new Node(val1);
            return;
        }
        if (val1 < t->value) {
            insert(val1, t->left);
        } else if (val1 > t->value) {
            insert(val1, t->right);
        } else {
            return;
        }
    }

    // recursive search
    Node* search(int val2, Node* t) const {
        if (t == nullptr) return nullptr;
        if (val2 == t->value) return t;
        if (val2 < t->value) return search(val2, t->left);
        return search(val2, t->right);
    }

public:
    // constructor
    BSTree() : root(nullptr) {}

    // destructor
    ~BSTree() {
        makeEmpty();
    }

    bool isEmpty() const {
        return root == nullptr;
    }

    void makeEmpty() {
        makeEmpty(root);
    }

    void insert(int val1) {
        insert(val1, root);
    }

    Node* search(int val2) const {
        return search(val2, root);
    }
};

// Traversals
void PREORDER_travl(Node* t) {
    if (t == nullptr) return;
    cout << t->value << " ";
    PREORDER_travl(t->left);
    PREORDER_travl(t->right);
}

void INORDER_travl(Node* t) {
    if (t == nullptr) return;
    INORDER_travl(t->left);
    cout << t->value << " ";
    INORDER_travl(t->right);
}

void POSTORDER_travl(Node* t) {
    if (t == nullptr) return;
    POSTORDER_travl(t->left);
    POSTORDER_travl(t->right);
    cout << t->value << " ";
}

int main() {
    BSTree bst;

    // insert nodes
    bst.insert(5);
    bst.insert(6);
    bst.insert(3);
    bst.insert(8);
    bst.insert(7);
    bst.insert(4);
    bst.insert(2);
    Node* root = bst.search(5);
    cout << "duyet cay thu tu truoc\n";
    PREORDER_travl(root);
    cout << "\n";
    cout << "duyet cay thu tu giua\n";
    INORDER_travl(root);
    cout << "\n";
    cout << "duyet cay thu tu sau\n";
    POSTORDER_travl(root);
    cout << "\n";
    Node* n1 = bst.search(4);
    Node* n2 = bst.search(9);

    if (n1 != nullptr)
        cout << "Node co gia tri = 4 la " << n1->value << "\n";
    if (n2 == nullptr)
        cout << "Khong tim thay node co gia tri = 9\n";

    bst.makeEmpty();
    if (bst.isEmpty())
        cout << "Cay da bi xoa rong\n";

    return 0;
}
