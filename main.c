#include "bst.c"   // include algorithms

int main() {
    Resource* root = NULL;

    // Insert demo resources
    root = insertBST(root, "R001", "Ambulance", 5, "Sector 3");
    root = insertBST(root, "R002", "Boat", 3, "Sector 7");
    root = insertBST(root, "R003", "Medkit", 10, "Sector 1");

    printf("\n All Resources:\n");
    inorder(root);

    printf("\n Searching for 'Boat':\n");
    Resource* s = searchResource(root, "Boat");
    if (s) printf("Found %s at %s (Qty:%d)\n", s->name, s->location, s->quantity);

    printf("\n Allocating 2 Medkits:\n");
    allocateResource(root, "Medkit", 2);

    printf("\n Updated Resources:\n");
    inorder(root);

    printf("\n KMP search (pattern='kit' in 'Medkit'):\n");
    KMPSearch("kit", "Medkit");

    return 0;
}
