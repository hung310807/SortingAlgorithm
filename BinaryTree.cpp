#include<iostream>
#include<vector>
#include<queue>
using namespace std;

class Node {
public:
    int data;
    Node* left, * right;
//node properties initializing
    Node(int key) {
        data = key;
        left = nullptr;
        right = nullptr;
    }//initialize node - constructor

    void inOrderTreeTraversal(Node* root){
        if(!root){
            return ;
        }
        inOrderTreeTraversal(root->left);
        cout<<root->data<<" ";
        inOrderTreeTraversal(root->right);
    }//in-Order Tree Traversal - duyệt giữa cây

    void postOrderTreeTraversal(Node *root){
        if(!root){
            return ;
        }
        postOrderTreeTraversal(root->left);
        postOrderTreeTraversal(root->right);
        cout<<root->data<<" ";
    }//post-Order Tree Traversal - duyệt sau cây

    void preOrderTreeTraversal(Node *root){
        if(!root){
            return ;
        }
        cout<<root->data<<" ";
        preOrderTreeTraversal(root->left);
        preOrderTreeTraversal(root->right);
    }//pre-Order Tree Traversal - duyệt trước cây

    void toLinkTree(Node **root, int i, vector <int> &tree){
        if(i == 0){
            (*root)->data = tree[i];
        }
        if(i * 2 + 1 < tree.size()){
            (*root)->left->data = tree[2*i + 1];
            toLinkTree(&((*root)->left), 2 * i + 1, tree);
        }
        if(i * 2 + 2 < tree.size()){
            (*root)->right->data = tree[2*i + 2];
            toLinkTree(&((*root)->right), 2 * i + 2, tree);
        }
        return;
    }

    
    vector <int> linearBinaryTree(Node *root){
        vector <int> tree;
        queue<Node*> Q;
        Q.push(root);
        while(!Q.empty()){
            Node* currentRoot = Q.front();
            Q.pop();
            tree.push_back(currentRoot->data);
            if(currentRoot->left != nullptr){
                Q.push(currentRoot->left);
            }
            if(currentRoot->right != nullptr){
                Q.push(currentRoot->right);
            }
        }
        //linked-list-based tree to linear-memory-based tree
        return tree;
    }

    void heapify(vector<int>& arr, int n, int i){
        int largest = i;
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        //lay toa do node cha va con
        if (l < n && arr[l] > arr[largest]){
            largest = l;
        }
        //node left > node cha -> doi largest
        if (r < n && arr[r] > arr[largest]){
            largest = r;
        }
        //node right > node cha -> doi largest
        if (largest != i) {
            swap(arr[i], arr[largest]);
            heapify(arr, n, largest);
        }
        //doi data node cha va lon nhat neu khac
    }

    void heapSort(Node *root, vector<int> arr){
        int n = arr.size();
        for (int i = n / 2 - 1; i >= 0; i--){
            heapify(arr, n, i);
        }
        for (int i = n - 1; i > 0; i--){
            swap(arr[0], arr[i]);
            heapify(arr, i, 0);
        }
         for(auto it = arr.begin(); it != arr.end(); ++it){
            cout<<*it<<" ";
        }
        toLinkTree(&root, 0, arr);
        return;
    }
};

int main(){
    Node *root = new Node(4);
    root->left = new Node(1);
    root->right = new Node(3);
    root->left->left = new Node(2);
    root->left->right = new Node(16);
    root->right->left = new Node(9);
    root->right->right = new Node(10);
    root->left->left->left = new Node(14);
    root->left->left->right = new Node(8);
    root->left->right->left = new Node(7);

    cout<<"duyet giua cay: ";root->inOrderTreeTraversal(root);cout<<endl;
    cout<<"duyet sau cay: ";root->postOrderTreeTraversal(root);cout<<endl;
    cout<<"duyet truoc cay: ";root->preOrderTreeTraversal(root);cout<<endl;
    cout<<"Sort: ";root->heapSort(root, root->linearBinaryTree(root));cout<<endl;
    cout<<"duyet truoc cay sau sort: ";root->preOrderTreeTraversal(root);cout<<endl;
}