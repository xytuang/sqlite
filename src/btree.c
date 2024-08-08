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


B-Tree Node Format for this project

1 node corresponds to 1 page

Type: Internal or Leaf node, 4 bytes

Number of kv pairs: 4 bytes

One kv pair: key is row id and value is username + email

*/

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "vector.h"

const uint32_t MAX_KEYS = 3; //m is now 4 ie. max 4 children per node

typedef enum { INTERNAL_NODE, LEAF_NODE } NodeType;

typedef struct Node{
    NodeType type;
    uint32_t num_keys;
    uint32_t keys[MAX_KEYS + 1]; //+1 to allow temporary overflow of keys
    uint32_t values[MAX_KEYS + 1]; //used only by leaf nodes
    struct Node* children[MAX_KEYS + 1]; //used only by internal nodes
} Node;

typedef struct {
    Node* root;
} BPlusTree;


/**************************************************
Node methods
**************************************************/

Node* new_node(NodeType type) {
    Node* node = (Node*) malloc(sizeof(Node));
    if (node == NULL) {
        printf("Failed to make node\n");
        exit(EXIT_FAILURE);
    }
    node->num_keys = 0;
    node->type = type;
    for (int i = 0; i < MAX_KEYS + 1; i++) {
        node->children[i] = NULL;
    }

    return node;
}

void free_node(Node* node) {
    if (node == NULL) {
        return;
    }
    for (uint32_t i = 0; i < node->num_keys; i++) {
        free_node(node->children[i]);
    }
    free(node);
}

/*
TODO: Visualize tree using BFS. Shows keys/kv pairs on each level and node
*/
void traverse_tree(Node* node) {
    int level = 0;
    Vector* queue = new_vector();
    append(queue, node);
    while(queue->curr_length != 0) {
        printf("LEVEL %d\n", level);
        int len = queue->curr_length;
        for (int i = 0; i < len; i++) {
            Node* curr = queue->arr[0];
            pop(queue);
            printf("Number of keys: %d\n", curr->num_keys);
            printf("Keys: ");
            for (int j = 0; j <= curr->num_keys; j++) {
                if (j == curr->num_keys - 1) {
                    printf("%d\n", curr->keys[j]);
                }
                else if (j < curr->num_keys) {
                    printf("%d ", curr->keys[j]);
                }
                if (curr->children[j] != NULL) {
                    append(queue, curr->children[j]);
                    //printf("Number of child keys: %d\n", curr->children[j]->num_keys);
                }
            }
            if (curr->type == LEAF_NODE) {
                printf("Values: ");
                for (int j = 0; j < curr->num_keys; j++) {
                    if (j == curr->num_keys - 1) {
                        printf("%d\n", curr->values[j]);
                    }
                    else {
                        printf("%d ", curr->values[j]);
                    }
                }
            }
        }
        level++;
        printf("\n");
    }
    free_vector(queue);

}

void leaf_insert(Node* node, uint32_t key, uint32_t value) {
    int i;

    for (i = node->num_keys - 1; i >= 0; i--) {
        if (node->keys[i] > key) {
            break;
        }
        node->keys[i + 1] = node->keys[i];
    }
    if (i < 0) {
        i = 0;
    }

    node->keys[i] = key;
    node->values[i] = value;
    node->num_keys++;
}


Node* split_node(Node* node) {
    Node* left_node = new_node(LEAF_NODE);
    Node* right_node = new_node(LEAF_NODE);
    int middle = (MAX_KEYS + 1) / 2;

    for (int i = 0; i < middle; i++) {
        left_node->keys[i] = node->keys[i];
        if (node->type == LEAF_NODE) {
            left_node->values[i] = node->values[i];
        } else {
            left_node->children[i] = node->children[i];
        }
        left_node->num_keys++;
    }
    if (node->type != LEAF_NODE) {
        left_node->children[middle] = node->children[middle];
    }

    for (int i = 0; i <= MAX_KEYS - middle; i++) {
        right_node->keys[i] = node->keys[i + middle];
        if (node->type == LEAF_NODE) {
            right_node->values[i] = node->values[i + middle];
        } else {
            right_node->children[i] = node->children[i + middle];
        }
        right_node->num_keys++;
    }
    if (node->type != LEAF_NODE) {
        right_node->children[MAX_KEYS - middle] = node->children[MAX_KEYS];
    }



    Node* middle_node = new_node(INTERNAL_NODE);
    middle_node->keys[0] = node->keys[middle];
    middle_node->children[0] = left_node;
    middle_node->children[1] = right_node;
    middle_node->num_keys = 1;

    return middle_node;
}
/**************************************************
BPlusTree methods
**************************************************/
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

void free_tree(BPlusTree* tree) {
    free_node(tree->root);
    free(tree);
}
/*
Insertion procedure:
1. Traverse tree until you find the leaf node where kv pair should be added

2. If leaf contains fewer kv pairs than MAX_KEYS, insert the kv pair while maintaining order

3. Else, split the node into two nodes along a median. 
   Left child node contains keys less than median.
   Right child node contains keys greater than median.
   Median key gets inserted into parent node.
   Splitting can propagate upwards, such that a new root could be made if necessary.
*/


Node* insert_into_node(Node* node, int key, int value) {
    if (node->type == LEAF_NODE) {
        int i;
        for (i = node->num_keys - 1; i >= 0 && node->keys[i] > key; i--) {
            node->keys[i + 1] = node->keys[i];
            node->values[i + 1] = node->values[i];
        }
        node->keys[i + 1] = key;
        node->values[i + 1] = value;
        node->num_keys++;

        if (node->num_keys > MAX_KEYS) {
            return split_node(node);
        } else {
            return NULL;
        }
    } else {
        int i = node->num_keys - 1;
        while (i >= 0 && node->keys[i] > key) {
            i--;
        }
        i++;
        Node* new_child = insert_into_node(node->children[i], key, value);
        if (new_child) {
            for (int j = node->num_keys; j > i; j--) {
                node->keys[j] = node->keys[j - 1];
                node->children[j + 1] = node->children[j];
            }
            node->keys[i] = new_child->keys[0];
            node->children[i] = new_child->children[0];
            node->children[i + 1] = new_child->children[1];
            node->num_keys++;

            if (node->num_keys > MAX_KEYS) {
                return split_node(node);
            } else {
                return NULL;
            }
        } else {
            return NULL;
        }
    }
}

void insert(BPlusTree* tree, int key, int value) {
    Node* new_root = insert_into_node(tree->root, key, value);
    if (new_root) {
        tree->root = new_root;
        /*
        Node* old_root = tree->root;
        tree->root = new_root;
        tree->root->children[0] = old_root;
        */
    }
}
/**************************************************
MAIN
**************************************************/
int main(int argc, char** argv) {
    BPlusTree* tree = new_tree();
    insert(tree, 3, 4);
    traverse_tree(tree->root);
    insert(tree, 4, 5);
    traverse_tree(tree->root);


    insert(tree, 5, 6);
    traverse_tree(tree->root);


    insert(tree, 6, 7);
    traverse_tree(tree->root);

    insert(tree, 1, 2);
    insert(tree, 2, 3);
    traverse_tree(tree->root);

    free_tree(tree);
    return 0;
}
