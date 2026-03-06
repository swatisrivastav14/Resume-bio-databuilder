#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* next;
};

struct Node* createNode(int val) {
    struct Node* n = malloc(sizeof(struct Node));
    n->data = val; n->next = NULL;
    return n;
}

void insert(struct Node** head, int val) {
    struct Node* n = createNode(val);
    if (!*head) *head = n;
    else {
        struct Node* t = *head;
        while (t->next) t = t->next;
        t->next = n;
    }
}

void del(struct Node** head, int val) {
    if (!*head) return;
    struct Node *t = *head, *p = NULL;
    while (t && t->data != val) { p = t; t = t->next; }
    if (!t) return;
    if (!p) *head = t->next;
    else p->next = t->next;
    free(t);
}

int search(struct Node* head, int val) {
    while (head) {
        if (head->data == val) return 1;
        head = head->next;
    }
    return 0;
}

void reverse(struct Node** head) {
    struct Node *prev = NULL, *cur = *head, *next = NULL;
    while (cur) {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }
    *head = prev;
}

void concat(struct Node** h1, struct Node* h2) {
    if (!*h1) *h1 = h2;
    else {
        struct Node* t = *h1;
        while (t->next) t = t->next;
        t->next = h2;
    }
}

void show(struct Node* h) {
    while (h) { printf("%d->", h->data); h = h->next; }
    printf("NULL\n");
}

int main() {
    struct Node *l1 = NULL, *l2 = NULL;
    int ch, val;
    do {
        printf("\n1.Insert 2.Delete 3.Search 4.Reverse 5.Display 6.Ins2 7.Concatenate 0.Exit\n");
        scanf("%d", &ch);
        switch (ch) {
            case 1: printf("Val: "); scanf("%d", &val); insert(&l1, val); break;
            case 2: printf("Del: "); scanf("%d", &val); del(&l1, val); break;
            case 3: printf("Search: "); scanf("%d", &val);
                    printf(search(l1, val) ? "Found\n" : "Not Found\n"); break;
            case 4: reverse(&l1); break;
            case 5: show(l1); break;
            case 6: printf("Val2: "); scanf("%d", &val); insert(&l2, val); break;
            case 7: concat(&l1, l2); printf("After concat: "); show(l1); break;
        }
    } while (ch != 0);
}