#include <stdio.h>
#include "btree.h"

int main() {
    BTree* tree = create_btree();

    // Insertion
    insert(tree, 10, "Pratham");
    insert(tree, 5, "Ridin");
    insert(tree, 20, "Rahul");
    insert(tree, 2, "Hardik");
    insert(tree, 3, "Burhan");
    insert(tree, 12, "Yash");
    insert(tree, 17, "Divyansh");
    insert(tree, 30, "Shivam");

    // Searching
    char* val1 = search(tree, 20);
    char* val2 = search(tree, 100);
    printf("Search result for 20: %s\n", val1 ? val1 : "Not Found");
    printf("Search result for 100: %s\n", val2 ? val2 : "Not Found");

    // Level-order traversal
    printf("\nLevel-order traversal:\n");
    level_order_traversal(tree);

    // Deletion (logic not implemented yet)
    delete_key(tree, 10);

    // Free memory (logic not implemented yet)
    free_btree(tree);

    return 0;
}
