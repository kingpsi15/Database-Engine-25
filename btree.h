#ifndef BTREE_H
#define BTREE_H

#define MAX_KEYS 4
#define MAX_CHILDREN 5
#define MAX_VALUE_LEN 100

typedef struct BTreeNode {
    int keys[MAX_KEYS];
    char values[MAX_KEYS][MAX_VALUE_LEN];
    struct BTreeNode* children[MAX_CHILDREN];
    int num_keys;
    int is_leaf;
} BTreeNode;

typedef struct BTree {
    BTreeNode* root;
} BTree;

// BTree functions
BTree* create_btree();
void insert(BTree* tree, int key, const char* value);
void delete_key(BTree* tree, int key);
char* search(BTree* tree, int key);
void level_order_traversal(BTree* tree);
void free_btree(BTree* tree);
BTreeNode* create_node(int is_leaf);


#endif
