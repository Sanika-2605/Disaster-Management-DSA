#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ---------------- RESOURCE STRUCT ----------------
typedef struct Resource {
    char id[10];
    char name[50];
    char category[30];
    int quantity;
    struct Resource *left, *right;
} Resource;

// ---------------- BST FUNCTIONS ----------------
Resource* createNode(char* id, char* name, char* category, int qty) {
    Resource* newNode = (Resource*)malloc(sizeof(Resource));
    strcpy(newNode->id, id);
    strcpy(newNode->name, name);
    strcpy(newNode->category, category);
    newNode->quantity = qty;
    newNode->left = newNode->right = NULL;
    return newNode;
}

Resource* insertBST(Resource* root, char* id, char* name, char* category, int qty) {
    if (root == NULL) return createNode(id, name, category, qty);
    if (strcmp(name, root->name) < 0)
        root->left = insertBST(root->left, id, name, category, qty);
    else if (strcmp(name, root->name) > 0)
        root->right = insertBST(root->right, id, name, category, qty);
    return root;
}

void inorder(Resource* root) {
    if (root != NULL) {
        inorder(root->left);
        printf("ID:%s | Name:%s | Category:%s | Qty:%d\n",
               root->id, root->name, root->category, root->quantity);
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
        printf("✅ Allocated %d of %s. Remaining: %d\n",
               qty, res->name, res->quantity);
    } else {
        printf("⚠️ Not enough quantity! Available: %d\n", res->quantity);
    }
}

// ---------------- LOAD CSV ----------------
Resource* loadResourcesCSV(Resource* root, const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Could not open file %s\n", filename);
        return root;
    }

    char line[256];
    fgets(line, sizeof(line), fp); // skip header

    while (fgets(line, sizeof(line), fp)) {
        char id[10], name[50], category[30];
        int qty;
        sscanf(line, "%[^,],%[^,],%[^,],%d", id, name, category, &qty);
        root = insertBST(root, id, name, category, qty);
    }

    fclose(fp);
    printf("✅ Resources loaded from %s\n", filename);
    return root;
}
