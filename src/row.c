#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "constants.h"
#include "row.h"

Row* new_row() {
    Row* row = (Row*) malloc(sizeof(Row));
    row->username = (char*) malloc(COLUMN_USERNAME_SIZE);
    if (row->username == NULL) {
        free(row);
        exit(EXIT_FAILURE);
    }
    row->email = (char*) malloc(COLUMN_EMAIL_SIZE);
    if (row->username == NULL) {
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
  memcpy(destination + ID_OFFSET, &(source->id), ID_SIZE);
  memcpy(destination + USERNAME_OFFSET, &(source->username), USERNAME_SIZE);
  memcpy(destination + EMAIL_OFFSET, &(source->email), EMAIL_SIZE);
  free(source->username);
  free(source->email);
  free(source);
}

void deserialize_row(void* source, Row* destination) {
  memcpy(&(destination->id), source + ID_OFFSET, ID_SIZE);
  memcpy(&(destination->username), source + USERNAME_OFFSET, USERNAME_SIZE);
  memcpy(&(destination->email), source + EMAIL_OFFSET, EMAIL_SIZE);
}

void print_row(Row* row) {
    printf("%d %s %s\n", row->id, row->username, row->email);
}
