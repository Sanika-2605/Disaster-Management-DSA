#include "bst.c"

int main() {
    Resource* root = NULL;

    // Load resources from CSV
    root = loadResourcesCSV(root, "resources.csv");

    printf("\nAll Resources:\n");
    inorder(root);

    // Example search
    printf("\nSearching for 'Medical Kits':\n");
    Resource* s = searchResource(root, "Medical Kits");
    if (s) printf("Found %s (%s) Qty:%d\n", s->name, s->category, s->quantity);

    // Example allocation
    printf("\nAllocating 5 Medical Kits:\n");
    allocateResource(root, "Medical Kits", 5);

    printf("\nUpdated Resources:\n");
    inorder(root);
    return 0;
}
