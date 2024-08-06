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

Pager* pager_open(const char* filename);

Table* db_open(const char* filename);

void* get_page(Pager* pager, uint32_t page_num);

void* row_slot(Table* table, uint32_t row_num);

void pager_flush(Pager* pager, uint32_t page_num, uint32_t size);

void db_close(Table* table);

#endif
