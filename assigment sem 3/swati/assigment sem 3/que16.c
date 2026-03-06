#include <stdio.h>
#include <stdlib.h>

// Node structure for threaded binary tree
struct Node {
    int data;
    struct Node *left, *right;
    int lthread, rthread;   // Flags: 1 if left/right pointer is a thread
};

// Create a new node
struct Node* newNode(int val) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->data = val;
    node->left = node->right = NULL;
    node->lthread = node->rthread = 1; // Initially threads
    return node;
}

// Insert a new node into threaded binary tree
struct Node* insert(struct Node* root, int key) {
    struct Node* parent = NULL, *curr = root;

    // Find the position to insert
    while (curr != NULL) {
        if (key == curr->data) return root; // Duplicate keys not allowed
        parent = curr;
        if (key < curr->data) {
            if (curr->lthread == 0)
                curr = curr->left;
            else break;
        } else {
            if (curr->rthread == 0)
                curr = curr->right;
            else break;
        }
    }

    struct Node* node = newNode(key);

    // If tree is empty
    if (parent == NULL) root = node;
    else if (key < parent->data) {
        node->left = parent->left;   // Left thread
        node->right = parent;        // Right points to parent
        parent->lthread = 0;         // Left is no longer a thread
        parent->left = node;
    } else {
        node->right = parent->right; // Right thread
        node->left = parent;         // Left points to parent
        parent->rthread = 0;         // Right is no longer a thread
        parent->right = node;
    }
    return root;
}

// Find inorder successor
struct Node* inorderSuccessor(struct Node* ptr) {
    if (ptr->rthread == 1) return ptr->right; // If right is thread
    ptr = ptr->right;
    while (ptr->lthread == 0) // Go to leftmost node in right subtree
        ptr = ptr->left;
    return ptr;
}

// Inorder traversal without recursion
void inorder(struct Node* root) {
    if (root == NULL) return;

    struct Node* curr = root;
    while (curr->lthread == 0) // Go to leftmost node
        curr = curr->left;

    while (curr != NULL) {
        printf("%d ", curr->data);
        curr = inorderSuccessor(curr);
    }
}

// Delete a node from TBT
struct Node* deleteNode(struct Node* root, int key) {
    struct Node *parent = NULL, *curr = root;

    // Search for node to delete
    while (curr != NULL && curr->data != key) {
        parent = curr;
        if (key < curr->data) {
            if (curr->lthread == 0)
                curr = curr->left;
            else break;
        } else {
            if (curr->rthread == 0)
                curr = curr->right;
            else break;
        }
    }

    if (curr == NULL || curr->data != key) {
        printf("\nKey %d not found.\n", key);
        return root;
    }

    // Case 1: Node with two children
    if (curr->lthread == 0 && curr->rthread == 0) {
        struct Node* succ = inorderSuccessor(curr);
        int val = succ->data;
        root = deleteNode(root, succ->data); // Recursively delete successor
        curr->data = val;
        return root;
    }

    // Case 2 & 3: Node with 0 or 1 child
    struct Node* child = NULL;
    if (curr->lthread == 0)
        child = curr->left;
    else if (curr->rthread == 0)
        child = curr->right;

    if (parent == NULL) { // Deleting root
        free(curr);
        return child;
    }

    if (curr == parent->left) {
        if (child != NULL)
            parent->left = child;
        else {
            parent->lthread = 1;
            parent->left = curr->left;
        }
    } else {
        if (child != NULL)
            parent->right = child;
        else {
            parent->rthread = 1;
            parent->right = curr->right;
        }
    }

    free(curr);
    return root;
}

int main() {
    struct Node* root = NULL;

    // Insert nodes
    root = insert(root, 20);
    root = insert(root, 10);
    root = insert(root, 30);
    root = insert(root, 5);
    root = insert(root, 15);

    printf("Inorder Traversal before deletion: ");
    inorder(root);
    printf("\n");

    // Delete a node
    root = deleteNode(root, 10);

    printf("Inorder Traversal after deletion: ");
    inorder(root);
    printf("\n");

    return 0;
}
