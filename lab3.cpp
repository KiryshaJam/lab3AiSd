#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <queue>
#include <cmath>
#include <cctype>

using namespace std;

struct TreeNode {
    int value;
    TreeNode* left;
    TreeNode* right;

    TreeNode(int val) : value(val), left(nullptr), right(nullptr) {}
};

struct AVLNode {
    int value;
    AVLNode* left;
    AVLNode* right;
    int height;

    AVLNode(int val) : value(val), left(nullptr), right(nullptr), height(1) {}
};

TreeNode* parseTree(const string& input) {
    stack<TreeNode*> nodeStack;
    TreeNode* root = nullptr;
    TreeNode* currentNode = nullptr;

    size_t i = 0;
    while (i < input.length()) {
        char ch = input[i];

        if (ch == '(') {
            i++;
            continue;
        }
        else if (ch == ')') {
            nodeStack.pop();
            i++;
        }
        else if (isdigit(ch) || (ch == '-' && isdigit(input[i+1]))) {
            size_t j = i;
            while (j < input.length() && (isdigit(input[j]) || input[j] == '-')) {
                j++;
            }

            int value = 0;
            bool isNegative = false;
            if (input[i] == '-') {
                isNegative = true;
                i++;
            }

            for (; i < j; i++) {
                value = value * 10 + (input[i] - '0');
            }

            if (isNegative) {
                value = -value;
            }

            TreeNode* newNode = new TreeNode(value);
            
            if (!root) root = newNode;

            if (!nodeStack.empty()) {
                currentNode = nodeStack.top();
                if (!currentNode->left) {
                    currentNode->left = newNode;
                } else {
                    currentNode->right = newNode;
                }
            }

            nodeStack.push(newNode);
        } else {
            i++;
        }
    }

    return root;
}

int height(AVLNode* node) {
    return node ? node->height : 0;
}

int balanceFactor(AVLNode* node) {
    return node ? height(node->left) - height(node->right) : 0;
}

AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

AVLNode* rightLeftRotate(AVLNode* node) {
    node->right = rightRotate(node->right);
    return leftRotate(node);
}

AVLNode* leftRightRotate(AVLNode* node) {
    node->left = leftRotate(node->left);
    return rightRotate(node);
}

AVLNode* balance(AVLNode* node) {
    if (balanceFactor(node) > 1) {
        if (balanceFactor(node->left) < 0)
            return leftRightRotate(node);
        return rightRotate(node);
    }
    if (balanceFactor(node) < -1) {
        if (balanceFactor(node->right) > 0)
            return rightLeftRotate(node);
        return leftRotate(node);
    }
    return node;
}

AVLNode* insert(AVLNode* node, int value) {
    if (!node)
        return new AVLNode(value);

    if (value < node->value)
        node->left = insert(node->left, value);
    else if (value > node->value)
        node->right = insert(node->right, value);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));
    
    return balance(node);
}

AVLNode* buildAVLFromBST(TreeNode* root) {
    if (!root) return nullptr;
    AVLNode* avlRoot = nullptr;

    stack<TreeNode*> nodeStack;
    TreeNode* currentNode = root;

    while (currentNode || !nodeStack.empty()) {
        while (currentNode) {
            nodeStack.push(currentNode);
            currentNode = currentNode->left;
        }
        currentNode = nodeStack.top();
        nodeStack.pop();
        
        avlRoot = insert(avlRoot, currentNode->value);
        
        currentNode = currentNode->right;
    }
    return avlRoot;
}

void preorderTraversal(AVLNode* root) {
    if (!root) return;
    cout << root->value << " ";
    preorderTraversal(root->left);
    preorderTraversal(root->right);
}

void inorderTraversal(AVLNode* root) {
    if (!root) return;
    inorderTraversal(root->left);
    cout << root->value << " ";
    inorderTraversal(root->right);
}

void postorderTraversal(AVLNode* root) {
    if (!root) return;
    postorderTraversal(root->left);
    postorderTraversal(root->right);
    cout << root->value << " ";
}

void levelOrderTraversal(AVLNode* root) {
    if (!root) return;

    queue<AVLNode*> q;
    q.push(root);

    while (!q.empty()) {
        AVLNode* node = q.front();
        q.pop();
        cout << node->value << " ";
        if (node->left) q.push(node->left);
        if (node->right) q.push(node->right);
    }
}

string readFromFile(const string& filename) {
    ifstream inputFile(filename);
    string content;
    
    if (inputFile.is_open()) {
        getline(inputFile, content);
        inputFile.close();
    } else {
        cout << "Ошибка открытия файла!" << endl;
    }

    return content;
}

int main() {
    system("clear");
    string filename = "tree.txt";
    string input = readFromFile(filename);
    
    if (input.empty()) {
        cout << "Ошибка: Пустая строка или не удалось считать файл!" << endl;
        return 1;
    }

    TreeNode* root = parseTree(input);

    if (!root) {
        cout << "Ошибка парсинга дерева!" << endl;
        return 1;
    }

    AVLNode* avlRoot = buildAVLFromBST(root);

    cout << "Pre-order: ";
    preorderTraversal(avlRoot);
    cout << endl;

    cout << "In-order: ";
    inorderTraversal(avlRoot);
    cout << endl;

    cout << "Post-order: ";
    postorderTraversal(avlRoot);
    cout << endl;

    cout << "Level-order: ";
    levelOrderTraversal(avlRoot);
    cout << endl;

    return 0;
}
