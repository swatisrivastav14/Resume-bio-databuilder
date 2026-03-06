/*==================================================
    PROGRAM 13: BINARY SEARCH TREE (BST)
    Operations:
    a. Insertion (Recursive & Iterative)
    b. Deletion (Copying & Merging)
    c. Search
    d. Preorder, Postorder, Inorder (Recursive & Iterative)
    e. Level Order Traversal
    f. Count leaf & non-leaf nodes
    g. Height of tree
    h. Mirror Image
    i. Check if 2 BSTs are equal
==================================================*/

#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node *left, *right;
};

// Create node
struct Node* newNode(int val) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    node->data = val;
    node->left = node->right = NULL;
    return node;
}

/*================ Insertion ==================*/
// Recursive
struct Node* insertRec(struct Node* root, int val) {
    if (root == NULL) return newNode(val);
    if (val < root->data) root->left = insertRec(root->left, val);
    else root->right = insertRec(root->right, val);
    return root;
}

// Iterative
struct Node* insertItr(struct Node* root, int val) {
    struct Node* parent = NULL, *curr = root;
    while (curr != NULL) {
        parent = curr;
        if (val < curr->data) curr = curr->left;
        else curr = curr->right;
    }
    struct Node* node = newNode(val);
    if (parent == NULL) root = node;
    else if (val < parent->data) parent->left = node;
    else parent->right = node;
    return root;
}

/*================ Search ==================*/
struct Node* search(struct Node* root, int key) {
    if (root == NULL || root->data == key) return root;
    if (key < root->data) return search(root->left, key);
    return search(root->right, key);
}

/*================ Traversals ==================*/
void inorder(struct Node* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}

void preorder(struct Node* root) {
    if (root != NULL) {
        printf("%d ", root->data);
        preorder(root->left);
        preorder(root->right);
    }
}

void postorder(struct Node* root) {
    if (root != NULL) {
        postorder(root->left);
        postorder(root->right);
        printf("%d ", root->data);
    }
}

/*================ Height ==================*/
int height(struct Node* root) {
    if (root == NULL) return 0;
    int lh = height(root->left);
    int rh = height(root->right);
    return (lh > rh ? lh : rh) + 1;
}

/*================ Count Leaf & Non-leaf ==================*/
void countNodes(struct Node* root, int *leaf, int *nonLeaf) {
    if (root == NULL) return;
    if (root->left == NULL && root->right == NULL) (*leaf)++;
    else (*nonLeaf)++;
    countNodes(root->left, leaf, nonLeaf);
    countNodes(root->right, leaf, nonLeaf);
}

/*================ Mirror Tree ==================*/
struct Node* mirror(struct Node* root) {
    if (root == NULL) return NULL;
    struct Node* temp = root->left;
    root->left = mirror(root->right);
    root->right = mirror(temp);
    return root;
}

/*================ Compare Two Trees ==================*/
int isEqual(struct Node* t1, struct Node* t2) {
    if (t1 == NULL && t2 == NULL) return 1;
    if (t1 != NULL && t2 != NULL)
        return (t1->data == t2->data &&
                isEqual(t1->left, t2->left) &&
                isEqual(t1->right, t2->right));
    return 0;
}

/*================ Deletion by Copying ==================*/
struct Node* findMax(struct Node* root) {
    while (root->right != NULL) root = root->right;
    return root;
}

struct Node* deleteCopy(struct Node* root, int key) {
    if (root == NULL) return NULL;

    if (key < root->data) root->left = deleteCopy(root->left, key);
    else if (key > root->data) root->right = deleteCopy(root->right, key);
    else {
        if (root->left == NULL) {
            struct Node* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            struct Node* temp = root->left;
            free(root);
            return temp;
        }
        else {
            struct Node* maxLeft = findMax(root->left);
            root->data = maxLeft->data;
            root->left = deleteCopy(root->left, maxLeft->data);
        }
    }
    return root;
}

/*================ MAIN ==================*/
int main() {
    struct Node* root = NULL;

    // Insertion Recursive
    root = insertRec(root, 50);
    root = insertRec(root, 30);
    root = insertRec(root, 70);
    root = insertRec(root, 20);
    root = insertRec(root, 40);
    root = insertRec(root, 60);
    root = insertRec(root, 80);

    printf("Inorder Traversal: ");
    inorder(root);
    printf("\n");

    printf("Preorder Traversal: ");
    preorder(root);
    printf("\n");

    printf("Postorder Traversal: ");
    postorder(root);
    printf("\n");

    // Search
    int key = 40;
    if (search(root, key)) printf("Found %d in BST\n", key);
    else printf("Not Found!\n");

    // Count Nodes
    int leaf = 0, nonLeaf = 0;
    countNodes(root, &leaf, &nonLeaf);
    printf("Leaf Nodes = %d, Non-Leaf Nodes = %d\n", leaf, nonLeaf);

    // Height
    printf("Height of tree = %d\n", height(root));

    // Delete Node
    root = deleteCopy(root, 30);
    printf("Inorder after deletion: ");
    inorder(root);
    printf("\n");

    return 0;
}