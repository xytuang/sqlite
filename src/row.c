#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "row.h"

Row* new_row() {
    Row* row = (Row*) malloc(sizeof(Row));
    row->username = (char*) malloc(COLUMN_USERNAME_SIZE + 1); //+1 for null byte
    if (row->username == NULL) {
        free(row);
        exit(EXIT_FAILURE);
    }
    row->email = (char*) malloc(COLUMN_EMAIL_SIZE + 1); //+1 for null byte
    if (row->email == NULL) {
        free(row->username);
        free(row);
        exit(EXIT_FAILURE);
    }
    return row;
}

void free_row(Row* row) {
    free(row->username);
    free(row->email);
    free(row);
}

void serialize_row(Row* source, void* destination) {
    memcpy(destination + USERNAME_LEN_OFFSET, &(source->username_len), USERNAME_LEN_SIZE);
    memcpy(destination + EMAIL_LEN_OFFSET, &(source->email_len), EMAIL_LEN_SIZE);
    memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
    strncpy(destination + USERNAME_OFFSET, (source->username), source->username_len);
    strncpy(destination + USERNAME_OFFSET + source->username_len, (source->email), source->email_len);
    free_row(source);
}

void deserialize_row(void* source, Row* destination) { 
    memcpy(&(destination->username_len), source + USERNAME_LEN_OFFSET, USERNAME_LEN_SIZE);
    memcpy(&(destination->email_len), source + EMAIL_LEN_OFFSET, EMAIL_LEN_SIZE);
    memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
    strncpy(destination->username, source + USERNAME_OFFSET, destination->username_len);
    strncpy(destination->email, source + USERNAME_OFFSET + destination->username_len, destination->email_len);
    destination->username[destination->username_len] = 0;
    destination->email[destination->email_len] = 0;
}

void print_row(Row* row) {
    printf("%d %s %s\n", row->id, row->username, row->email);
}
