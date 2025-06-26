#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "btree.h"
#include "queue.h"
#define MAX_KEYS 4
#define MIN_KEYS 2

// Create a new node
BTreeNode* create_node(int is_leaf) {
    BTreeNode* node = (BTreeNode*)malloc(sizeof(BTreeNode));
    node->is_leaf = is_leaf;
    node->num_keys = 0;
    for (int i = 0; i < MAX_CHILDREN; i++) node->children[i] = NULL;
    return node;
}

// Create a new BTree
BTree* create_btree() {
    BTree* tree = (BTree*)malloc(sizeof(BTree));
    tree->root = create_node(1);
    return tree;
}

// Split the full child of a parent
void split_child(BTreeNode* parent, int index) {
    BTreeNode* full_child = parent->children[index];
    BTreeNode* new_child = create_node(full_child->is_leaf);

    int mid = MAX_KEYS / 2;

    new_child->num_keys = MAX_KEYS - mid - 1;
    for (int j = 0; j < new_child->num_keys; ++j) {
        new_child->keys[j] = full_child->keys[mid + 1 + j];
        strcpy(new_child->values[j], full_child->values[mid + 1 + j]);
    }

    if (!full_child->is_leaf) {
        for (int j = 0; j <= new_child->num_keys; ++j) {
            new_child->children[j] = full_child->children[mid + 1 + j];
        }
    }

    full_child->num_keys = mid;

    for (int j = parent->num_keys; j > index; --j) {
        parent->keys[j] = parent->keys[j - 1];
        strcpy(parent->values[j], parent->values[j - 1]);
        parent->children[j + 1] = parent->children[j];
    }

    parent->keys[index] = full_child->keys[mid];
    strcpy(parent->values[index], full_child->values[mid]);
    parent->children[index + 1] = new_child;
    parent->num_keys++;
}

// Insert into a non-full node
void insert_non_full(BTreeNode* node, int key, const char* value) {
    int i = node->num_keys - 1;

    if (node->is_leaf) {
        while (i >= 0 && key < node->keys[i]) {
            node->keys[i + 1] = node->keys[i];
            strcpy(node->values[i + 1], node->values[i]);
            i--;
        }
        node->keys[i + 1] = key;
        strcpy(node->values[i + 1], value);
        node->num_keys++;
    } else {
        while (i >= 0 && key < node->keys[i]) i--;
        i++;

        if (node->children[i]->num_keys == MAX_KEYS) {
            split_child(node, i);
            if (key > node->keys[i]) i++;
        }
        insert_non_full(node->children[i], key, value);
    }
}

// Insert into B-Tree
void insert(BTree* tree, int key, const char* value) {
    if (!tree->root) {
        tree->root = create_node(1);
        tree->root->keys[0] = key;
        strcpy(tree->root->values[0], value);
        tree->root->num_keys = 1;
        return;
    }

    BTreeNode* root = tree->root;

    if (root->num_keys == MAX_KEYS) {
        BTreeNode* new_root = create_node(0);
        new_root->children[0] = root;
        split_child(new_root, 0);
        tree->root = new_root;
    }

    insert_non_full(tree->root, key, value);
}

// Level-order traversal using queue
void level_order_traversal(BTree* tree) {
    if (!tree->root) return;
    Queue* q = create_queue();
    enqueue(q, tree->root);
    enqueue(q, NULL); // NULL marks end of current level

    while (!is_empty(q)) {
        BTreeNode* node = dequeue(q);

        if (node == NULL) {
            printf("\n");
            if (!is_empty(q))
                enqueue(q, NULL);
            continue;
        }

        printf("[");
        for (int i = 0; i < node->num_keys; ++i) {
            printf("%d,%s", node->keys[i], node->values[i]);
            if (i != node->num_keys - 1) printf(" ");
        }
        printf("] ");

        if (!node->is_leaf) {
            for (int i = 0; i <= node->num_keys; ++i) {
                enqueue(q, node->children[i]);
            }
        }
    }
    printf("\n");
    destroy_queue(q);
}

static void remove_key_from_node(BTreeNode* node, int idx) {
    for (int i = idx + 1; i < node->num_keys; ++i) {
        node->keys[i - 1] = node->keys[i];
        strcpy(node->values[i - 1], node->values[i]);
    }
    node->num_keys--;
}

static int find_key_index(BTreeNode* node, int key) {
    int idx = 0;
    while (idx < node->num_keys && node->keys[idx] < key)
        idx++;
    return idx;
}

static void merge_nodes(BTreeNode* parent, int idx) {
    BTreeNode* child = parent->children[idx];
    BTreeNode* sibling = parent->children[idx + 1];

    child->keys[MIN_KEYS] = parent->keys[idx];
    strcpy(child->values[MIN_KEYS], parent->values[idx]);

    for (int i = 0; i < sibling->num_keys; ++i) {
        child->keys[i + MIN_KEYS + 1] = sibling->keys[i];
        strcpy(child->values[i + MIN_KEYS + 1], sibling->values[i]);
    }

    if (!child->is_leaf) {
        for (int i = 0; i <= sibling->num_keys; ++i)
            child->children[i + MIN_KEYS + 1] = sibling->children[i];
    }

    for (int i = idx + 1; i < parent->num_keys; ++i) {
        parent->keys[i - 1] = parent->keys[i];
        strcpy(parent->values[i - 1], parent->values[i]);
        parent->children[i] = parent->children[i + 1];
    }

    child->num_keys += sibling->num_keys + 1;
    parent->num_keys--;
    free(sibling);
}

static void borrow_from_prev(BTreeNode* node, int idx) {
    BTreeNode* child = node->children[idx];
    BTreeNode* sibling = node->children[idx - 1];

    for (int i = child->num_keys - 1; i >= 0; --i) {
        child->keys[i + 1] = child->keys[i];
        strcpy(child->values[i + 1], child->values[i]);
    }

    if (!child->is_leaf) {
        for (int i = child->num_keys; i >= 0; --i)
            child->children[i + 1] = child->children[i];
    }

    child->keys[0] = node->keys[idx - 1];
    strcpy(child->values[0], node->values[idx - 1]);
    if (!child->is_leaf)
        child->children[0] = sibling->children[sibling->num_keys];

    node->keys[idx - 1] = sibling->keys[sibling->num_keys - 1];
    strcpy(node->values[idx - 1], sibling->values[sibling->num_keys - 1]);

    child->num_keys++;
    sibling->num_keys--;
}

static void borrow_from_next(BTreeNode* node, int idx) {
    BTreeNode* child = node->children[idx];
    BTreeNode* sibling = node->children[idx + 1];

    child->keys[child->num_keys] = node->keys[idx];
    strcpy(child->values[child->num_keys], node->values[idx]);

    if (!child->is_leaf)
        child->children[child->num_keys + 1] = sibling->children[0];

    node->keys[idx] = sibling->keys[0];
    strcpy(node->values[idx], sibling->values[0]);

    for (int i = 1; i < sibling->num_keys; ++i) {
        sibling->keys[i - 1] = sibling->keys[i];
        strcpy(sibling->values[i - 1], sibling->values[i]);
    }

    if (!sibling->is_leaf) {
        for (int i = 1; i <= sibling->num_keys; ++i)
            sibling->children[i - 1] = sibling->children[i];
    }

    child->num_keys++;
    sibling->num_keys--;
}

static void fill(BTreeNode* node, int idx) {
    if (idx > 0 && node->children[idx - 1]->num_keys >= MIN_KEYS + 1)
        borrow_from_prev(node, idx);
    else if (idx < node->num_keys && node->children[idx + 1]->num_keys >= MIN_KEYS + 1)
        borrow_from_next(node, idx);
    else {
        if (idx != node->num_keys)
            merge_nodes(node, idx);
        else
            merge_nodes(node, idx - 1);
    }
}

static BTreeNode* get_predecessor(BTreeNode* node) {
    BTreeNode* cur = node;
    while (!cur->is_leaf)
        cur = cur->children[cur->num_keys];
    return cur;
}

static BTreeNode* get_successor(BTreeNode* node) {
    BTreeNode* cur = node;
    while (!cur->is_leaf)
        cur = cur->children[0];
    return cur;
}

static void delete_from_node(BTreeNode* node, int key);

static void delete_from_internal(BTreeNode* node, int idx) {
    int key = node->keys[idx];
    if (node->children[idx]->num_keys >= MIN_KEYS + 1) {
        BTreeNode* pred = get_predecessor(node->children[idx]);
        node->keys[idx] = pred->keys[pred->num_keys - 1];
        strcpy(node->values[idx], pred->values[pred->num_keys - 1]);
        delete_from_node(node->children[idx], pred->keys[pred->num_keys - 1]);
    } else if (node->children[idx + 1]->num_keys >= MIN_KEYS + 1) {
        BTreeNode* succ = get_successor(node->children[idx + 1]);
        node->keys[idx] = succ->keys[0];
        strcpy(node->values[idx], succ->values[0]);
        delete_from_node(node->children[idx + 1], succ->keys[0]);
    } else {
        merge_nodes(node, idx);
        delete_from_node(node->children[idx], key);
    }
}

static void delete_from_node(BTreeNode* node, int key) {
    int idx = find_key_index(node, key);

    if (idx < node->num_keys && node->keys[idx] == key) {
        if (node->is_leaf)
            remove_key_from_node(node, idx);
        else
            delete_from_internal(node, idx);
    } else {
        if (node->is_leaf) return;

        int flag = (idx == node->num_keys);
        if (node->children[idx]->num_keys == MIN_KEYS)
            fill(node, idx);

        if (flag && idx > node->num_keys)
            delete_from_node(node->children[idx - 1], key);
        else
            delete_from_node(node->children[idx], key);
    }
}

void delete_key(BTree* tree, int key) {
    if (!tree->root) return;

    delete_from_node(tree->root, key);

    if (tree->root->num_keys == 0) {
        BTreeNode* old_root = tree->root;
        if (tree->root->is_leaf) {
            free(tree->root);
            tree->root = NULL;
        } else {
            tree->root = tree->root->children[0];
            free(old_root);
        }
    }
}

char* search(BTree* tree, int key) {
    BTreeNode* current = tree->root;
    while (current) {
        int i = 0;
        while (i < current->num_keys && key > current->keys[i]) i++;
        if (i < current->num_keys && key == current->keys[i])
            return current->values[i];
        if (current->is_leaf) break;
        current = current->children[i];
    }
    return NULL;
}

static void free_btree_node(BTreeNode* node) {
    if (!node) return;

    if (!node->is_leaf) {
        for (int i = 0; i <= node->num_keys; ++i) {
            free_btree_node(node->children[i]);
        }
    }

    free(node);
}

void free_btree(BTree* tree) {
    if (!tree) return;
    free_btree_node(tree->root);
    free(tree);
}

