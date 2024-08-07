#ifndef TABLE_H
#define TABLE_H

#include "constants.h"


/*
Pager is for persistence to disk/file
*/
typedef struct {
    int file_descriptor;
    uint32_t file_length;
    void** pages;
} Pager;


typedef struct {
  uint32_t num_rows;
  Pager* pager;
} Table;


//Cursor is meant for data set traversal in sqlite, similar to an iterator
typedef struct {
    Table* table;
    uint32_t row_num;
    int end_of_table;
} Cursor;

Pager* pager_open(const char* filename);

Table* db_open(const char* filename);

void* get_page(Pager* pager, uint32_t page_num);

void* cursor_value(Cursor* cursor);

void pager_flush(Pager* pager, uint32_t page_num, uint32_t size);

void db_close(Table* table);

Cursor* table_start(Table* table);

Cursor* table_end(Table* table);

void cursor_advance(Cursor* cursor);

#endif
