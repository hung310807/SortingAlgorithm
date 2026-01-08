#include<iostream>
#include<sstream>
#include<fstream>
#include<vector>
#include<queue>
using namespace std;

class Node{
    public:
        int data;
        Node *left; 
        Node *right;
    Node(int Key){
        data = Key;
        left = nullptr;
        right = nullptr;
    }
};

void FileRead(vector <int> &tree1, vector <int> &tree2){
    ifstream file("INP.txt");
    string inputData;
    getline(file, inputData);
    istringstream iss(inputData);
    int inp;
    while(iss>>inp){
        tree1.push_back(inp);
    }
    getline(file, inputData);
    istringstream is(inputData);
    while(is>>inp){
        tree2.push_back(inp);
    }
}

void toLinkTree(Node **root, vector <int> &tree){
    int i = 0;
    queue <Node*> Q;
    Q.push(*root);
    (*root)->data = tree[i];
    while(!Q.empty()){
        Node *node = Q.front();
        Q.pop();
        if(2 * i + 1 < tree.size()){
            node->left = new Node(tree[2 * i + 1]);
            Q.push(node->left);
        }
        if(2 * i + 2 < tree.size()){
            node->right = new Node(tree[2 * i + 2]);
            Q.push(node->right);
        }
        ++i;
    }
}

bool DFS(Node **root1, Node **root2){
    vector <pair<Node *, Node *>> cmp;
    cmp.reserve(1024);
    cmp.push_back({*root1, *root2});
    while(!cmp.empty()){
        auto [a, b] = cmp.back();
        cmp.pop_back();
        if(a == nullptr && b == nullptr){
            continue;
        }
        if(a == nullptr || b == nullptr){
            return false;
        }
        if(a->data != b->data){
            return false;
        }
        cmp.push_back({(*root1)->left, (*root2)->left});
        cmp.push_back({(*root1)->right, (*root2)->right});
    }
    return true;
}

int main(){
    vector <int> tree1, tree2;
    FileRead(tree1, tree2);
    Node *root1, *root2;
    root1 = new Node(0);
    root2 = new Node(0);
    toLinkTree(&root1, tree1);
    toLinkTree(&root2, tree2);
    cout<<(DFS(&root1, &root2) ? "true" : "false");
    cout<<"\nComplexity O(n)";
    return 0;
}