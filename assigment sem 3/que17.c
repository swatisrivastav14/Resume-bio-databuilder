#include <stdio.h>
#include <stdlib.h>

// Node structure for AVL Tree
struct Node {
    int data;
    struct Node *left;
    struct Node *right;
    int height;
};

// Utility function to get maximum
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Get height of node
int height(struct Node* N) {
    if (N == NULL) return 0;
    return N->height;
}

// Create new node
struct Node* newNode(int key) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->data = key;
    node->left = node->right = NULL;
    node->height = 1; // New node is initially at leaf
    return node;
}

// Right rotate
struct Node* rightRotate(struct Node* y) {
    struct Node* x = y->left;
    struct Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

// Left rotate
struct Node* leftRotate(struct Node* x) {
    struct Node* y = x->right;
    struct Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

// Get balance factor
int getBalance(struct Node* N) {
    if (N == NULL) return 0;
    return height(N->left) - height(N->right);
}

// Insert node
struct Node* insert(struct Node* node, int key) {
    if (node == NULL) return newNode(key);

    if (key < node->data) node->left = insert(node->left, key);
    else if (key > node->data) node->right = insert(node->right, key);
    else return node; // Duplicate keys not allowed

    node->height = 1 + max(height(node->left), height(node->right));
    int balance = getBalance(node);

    // LL Case
    if (balance > 1 && key < node->left->data) return rightRotate(node);
    // RR Case
    if (balance < -1 && key > node->right->data) return leftRotate(node);
    // LR Case
    if (balance > 1 && key > node->left->data) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    // RL Case
    if (balance < -1 && key < node->right->data) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Find minimum value node
struct Node* minValueNode(struct Node* node) {
    struct Node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

// Delete node
struct Node* deleteNode(struct Node* root, int key) {
    if (root == NULL) return root;

    if (key < root->data)
        root->left = deleteNode(root->left, key);
    else if (key > root->data)
        root->right = deleteNode(root->right, key);
    else {
        // Node with one child or no child
        if (root->left == NULL || root->right == NULL) {
            struct Node* temp = root->left ? root->left : root->right;

            if (temp == NULL) { // No child
                temp = root;
                root = NULL;
            } else // One child
                *root = *temp;

            free(temp);
        } else {
            // Node with two children
            struct Node* temp = minValueNode(root->right);
            root->data = temp->data;
            root->right = deleteNode(root->right, temp->data);
        }
    }

    if (root == NULL) return root;

    // Update height
    root->height = 1 + max(height(root->left), height(root->right));
    int balance = getBalance(root);

    // Balance the tree
    // LL Case
    if (balance > 1 && getBalance(root->left) >= 0) return rightRotate(root);
    // LR Case
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    // RR Case
    if (balance < -1 && getBalance(root->right) <= 0) return leftRotate(root);
    // RL Case
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Inorder traversal
void inorder(struct Node* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}

// Preorder traversal
void preorder(struct Node* root) {
    if (root != NULL) {
        printf("%d ", root->data);
        preorder(root->left);
        preorder(root->right);
    }
}

// Postorder traversal
void postorder(struct Node* root) {
    if (root != NULL) {
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->data);
    }
}

// Main function
int main() {
    struct Node* root = NULL;

    // Insertion
    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 30);
    root = insert(root, 40);
    root = insert(root, 50);
    root = insert(root, 25);

    printf("Inorder traversal: ");
    inorder(root);
    printf("\n");

    printf("Preorder traversal: ");
    preorder(root);
    printf("\n");

    printf("Postorder traversal: ");
    postorder(root);
    printf("\n");

    // Deletion
    root = deleteNode(root, 40);
    printf("Inorder after deleting 40: ");
    inorder(root);
    printf("\n");

    return 0;
}
