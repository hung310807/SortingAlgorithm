#include <iostream>
#include <string>
#include <memory>

using namespace std;

struct Node {
    int index;         
    string title;       
    unique_ptr<Node> left;
    unique_ptr<Node> right;

    Node(int idx, string t) : index(idx), title(std::move(t)) {}
};

bool isSameTree(const Node* a, const Node* b) {
    if (a == nullptr && b == nullptr) return true;
    if (a == nullptr || b == nullptr) return false;

    if (a->index != b->index) return false;
    if (a->title != b->title) return false;

    return isSameTree(a->left.get(), b->left.get()) &&
           isSameTree(a->right.get(), b->right.get());
}

unique_ptr<Node> buildBookA() {
    auto root = make_unique<Node>(1, "Chapter 1");
    root->left = make_unique<Node>(11, "Section 1");
    root->right = make_unique<Node>(12, "Section 2");
    root->left->left = make_unique<Node>(111, "Subsection 1");
    return root;
}

unique_ptr<Node> buildBookB_sameAsA() {
    auto root = make_unique<Node>(1, "Chapter 1");
    root->left = make_unique<Node>(11, "Section 1");
    root->right = make_unique<Node>(12, "Section 2");
    root->left->left = make_unique<Node>(111, "Subsection 1");
    return root;
}

unique_ptr<Node> buildBookB_different() {
    auto root = make_unique<Node>(1, "Chapter 1");
    root->left = make_unique<Node>(11, "Section 1");
    root->right = make_unique<Node>(12, "Section X");
    root->left->left = make_unique<Node>(111, "Subsection 1");
    return root;
}

int main() {
    auto book1 = buildBookA();
    auto book2 = buildBookB_sameAsA();    

    if (isSameTree(book1.get(), book2.get())) {
        cout << "hai cuon sach la ban copy cua nhau\n";
    } else {
        cout << "hai cuon sach KHONG la ban copy cua nhau\n";
    }
    return 0;
}
