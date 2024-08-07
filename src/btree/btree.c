/*
B-Tree defintion

A B-Tree of order m has the following properties:

1. Every node has at most m children.

2. Every node, except root and leaves, has at most ceil(m/2) children

3. Root node has at least 2 children unless it is a leaf.

4. All leaves are on the same level.

5. A non-leaf node with m children contains m - 1 keys


Information contained in both internal and leaf nodes:

1. Type of node (internal or leaf)

2. Number of keys (to maintain invariant of at most m children)

Information contained in internal node with m child nodes:

1. m - 1 keys

2. m pointers to child nodes

Information contained in leaf node:

1. An infinite number of key-value pairs

*/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>


const uint32_t MAX_KEYS = 3;

const uint32_t MAX_VALUES = 3;

typedef enum { ROOT_NODE, LEAF_NODE } NodeType;

typedef struct Node{
    NodeType type;
    uint32_t num_keys;
    uint32_t* keys;
    uint32_t* values; //used only by leaf nodes
    struct Node** children; //used only by internal nodes
} Node;

typedef struct {
    Node* root;
} BPlusTree;



Node* new_node(NodeType type) {
    Node* node = (Node*) malloc(sizeof(Node));
    if (node == NULL) {
        printf("Failed to make node\n");
        exit(EXIT_FAILURE);
    }
    node->num_keys = 0;
    uint32_t* keys = (uint32_t *) malloc(sizeof(uint32_t) * MAX_KEYS);

    if (node == NULL) {
        printf("Failed to make keys\n");
        exit(EXIT_FAILURE);
    }

    uint32_t* values = (uint32_t *) malloc(sizeof(uint32_t) * MAX_VALUES);

    if (values == NULL) {
        printf("Failed to make values\n");
        exit(EXIT_FAILURE);
    }

    Node** children = (Node**) malloc(sizeof(Node*) * (MAX_KEYS + 1));

    if (children == NULL) {
        printf("Failed to make children\n");
        exit(EXIT_FAILURE);
    }
    node->keys = keys;
    node->values = values;
    node->children = children;

    return node;
}

BPlusTree* new_tree() {
    BPlusTree* tree = (BPlusTree*) malloc(sizeof(BPlusTree));
    if (tree == NULL) {
        printf("Failed to make tree\n");
        exit(EXIT_FAILURE);
    }
    Node* node = new_node(LEAF_NODE);
    tree->root = node;
    return tree;
}


int main(int argc, char** argv) {
    BPlusTree* tree = new_tree();
    return 0;
}
