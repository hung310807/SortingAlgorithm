#include <iostream>
#include <functional>
#include <stdexcept>

template <typename T>
struct Node {
    T data;
    Node* left;
    Node* right;
};

template <typename T>
struct LinkedTree {
    Node<T>* root;
    void Init() {
        Clear(root);
        root = nullptr;
    }
    bool IsEmpty() const {
        return root == nullptr;
    }
    Node<T>* SetRoot(const T& v) {
        if (root) throw std::runtime_error("Root exists");
        root = NewNode(v);
        return root;
    }
    Node<T>* AddLeft(Node<T>* parent, const T& v) {
        Require(parent);
        if (parent->left) throw std::runtime_error("Left exists");
        parent->left = NewNode(v);
        return parent->left;
    }
    Node<T>* AddRight(Node<T>* parent, const T& v) {
        Require(parent);
        if (parent->right) throw std::runtime_error("Right exists");
        parent->right = NewNode(v);
        return parent->right;
    }
    void PreOrder (const std::function<void(const T&)>& Visit) const { Pre (root, Visit); }
    void InOrder  (const std::function<void(const T&)>& Visit) const { In  (root, Visit); }
    void PostOrder(const std::function<void(const T&)>& Visit) const { Post(root, Visit); }
    Node<T>* NewNode(const T& v) {
        Node<T>* n = new Node<T>;
        n->data = v;
        n->left = nullptr;
        n->right = nullptr;
        return n;
    }
    void Require(Node<T>* p) const {
        if (!p) throw std::runtime_error("Null parent");
    }
    void Clear(Node<T>* n) {
        if (!n) return;
        Clear(n->left);
        Clear(n->right);
        delete n;
    }
    void Pre(Node<T>* n, const std::function<void(const T&)>& Visit) const {
        if (!n) return;
        Visit(n->data);
        Pre(n->left, Visit);
        Pre(n->right, Visit);
    }
    void In(Node<T>* n, const std::function<void(const T&)>& Visit) const {
        if (!n) return;
        In(n->left, Visit);
        Visit(n->data);
        In(n->right, Visit);
    }
    void Post(Node<T>* n, const std::function<void(const T&)>& Visit) const {
        if (!n) return;
        Post(n->left, Visit);
        Post(n->right, Visit);
        Visit(n->data);
    }
};

int main() {
    LinkedTree<int> T;
    T.root = nullptr;
    T.Init();
    auto R  = T.SetRoot(1);
    auto L  = T.AddLeft(R, 2);
    auto RR = T.AddRight(R, 3);
    T.AddLeft(L, 4);
    T.AddRight(L, 5);
    std::cout << "PreOrder : ";
    T.PreOrder([](const int& x){ std::cout << x << ' '; });
    std::cout << "\nInOrder  : ";
    T.InOrder([](const int& x){ std::cout << x << ' '; });
    std::cout << "\nPostOrder: ";
    T.PostOrder([](const int& x){ std::cout << x << ' '; });
    std::cout << "\n";
    T.Init();
    return 0;
}
