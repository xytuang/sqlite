#ifndef ROW_H
#define ROW_H

typedef struct {
    uint32_t id;
    char* username;
    char* email;
} Row;

Row* new_row();

void free_row(Row* row);

void serialize_row(Row* source, void* destination);

void deserialize_row(void* source, Row* destination);

void print_row(Row* row);

#endif
