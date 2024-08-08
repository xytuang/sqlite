#ifndef VECTOR_H
#define VECTOR_H
#include <stdio.h>

typedef struct {
    int max_length;
    int curr_length;
    struct Node** arr;
} Vector;

Vector* new_vector() {
    Vector* vec = (Vector*) malloc(sizeof(Vector));
    vec->arr = (struct Node**) malloc(80); //pointer is 8 bytes, 10 pointers is 80 bytes
    vec->max_length = 10;
    vec->curr_length = 0;
    return vec;
}

void append(Vector* vec, struct Node* node) {
    if (vec->curr_length >= vec->max_length) {
        //printf("expand\n");
        struct Node* new_arr[vec->max_length * 2];
        for (int i = 0; i < vec->curr_length; i++) {
            new_arr[i] = vec->arr[i];
        }
        vec->max_length = vec->max_length * 2;
        vec->arr = new_arr;
    }
    //printf("here\n");
    vec->arr[vec->curr_length] = node;
    vec->curr_length++;
}

//No error checking for empty array since it is assumed that user will check length before calling
void pop(Vector* vec) {
    for (int i = 0; i < vec->curr_length - 1; i++) {
        vec->arr[i] = vec->arr[i + 1];
    }
    vec->curr_length--;
}

void free_vector(Vector* vec) {
    free(vec->arr);
    free(vec);
}
#endif
