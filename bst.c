#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------------- RESOURCE STRUCT ----------------
typedef struct Resource {
    char id[10];
    char name[50];
    int quantity;
    char location[50];
    struct Resource *left, *right;
} Resource;

// ---------------- BST FUNCTIONS ----------------
Resource* createNode(char* id, char* name, int qty, char* location) {
    Resource* newNode = (Resource*)malloc(sizeof(Resource));
    strcpy(newNode->id, id);
    strcpy(newNode->name, name);
    newNode->quantity = qty;
    strcpy(newNode->location, location);
    newNode->left = newNode->right = NULL;
    return newNode;
}

Resource* insertBST(Resource* root, char* id, char* name, int qty, char* location) {
    if (root == NULL) return createNode(id, name, qty, location);
    if (strcmp(name, root->name) < 0)
        root->left = insertBST(root->left, id, name, qty, location);
    else if (strcmp(name, root->name) > 0)
        root->right = insertBST(root->right, id, name, qty, location);
    return root;
}

void inorder(Resource* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("ID:%s | Name:%s | Qty:%d | Location:%s\n",
               root->id, root->name, root->quantity, root->location);
        inorder(root->right);
    }
}

Resource* searchResource(Resource* root, char* name) {
    if (root == NULL || strcmp(name, root->name) == 0)
        return root;
    if (strcmp(name, root->name) < 0)
        return searchResource(root->left, name);
    return searchResource(root->right, name);
}

void allocateResource(Resource* root, char* name, int qty) {
    Resource* res = searchResource(root, name);
    if (res == NULL) {
        printf("Resource not found!\n");
        return;
    }
    if (res->quantity >= qty) {
        res->quantity -= qty;
        printf("Allocated %d of %s. Remaining: %d\n",
               qty, res->name, res->quantity);
    } else {
        printf("Not enough quantity! Available: %d\n", res->quantity);
    }
}

// ---------------- KMP FUNCTIONS ----------------
void computeLPS(char* pat, int M, int* lps) {
    int len = 0, i = 1;
    lps[0] = 0;
    while (i < M) {
        if (pat[i] == pat[len]) {
            lps[i++] = ++len;
        } else {
            if (len != 0)
                len = lps[len - 1];
            else
                lps[i++] = 0;
        }
    }
}

void KMPSearch(char* pat, char* txt) {
    int M = strlen(pat);
    int N = strlen(txt);
    int lps[M];
    computeLPS(pat, M, lps);

    int i = 0, j = 0;
    while (i < N) {
        if (pat[j] == txt[i]) {
            i++; j++;
        }
        if (j == M) {
            printf(" Found pattern at index %d in '%s'\n", i - j, txt);
            j = lps[j - 1];
        } else if (i < N && pat[j] != txt[i]) {
            if (j != 0) j = lps[j - 1];
            else i++;
        }
    }
}
