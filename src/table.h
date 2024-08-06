#ifndef TABLE_H
#define TABLE_H

#include "constants.h"

typedef struct {
  uint32_t num_rows;
  void** pages;
} Table;

Table* new_table();

void free_table(Table* table);

void* row_slot(Table* table, uint32_t row_num);

#endif
