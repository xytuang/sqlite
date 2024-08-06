#include <stdlib.h>
#include <stdio.h>
#include "table.h"
#include "constants.h"

Table* new_table() {
    Table* table = (Table*)malloc(sizeof(Table));
    table->num_rows = 0;
    table->pages = malloc(TABLE_MAX_PAGES * 8);
    if (table->pages == NULL) {
        printf("allocation for pages failed\n");
        free(table);
        exit(EXIT_FAILURE);
    }
    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        table->pages[i] = NULL;
    }
    return table;
}


void free_table(Table* table) {
    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        if (table->pages[i] != NULL) {
            free(table->pages[i]);
        }
    }
    free(table->pages);
    free(table);
}


void* row_slot(Table* table, uint32_t row_num) {
  uint32_t page_num = row_num / ROWS_PER_PAGE;
  void* page = table->pages[page_num];
  if (page == NULL) {
    page = table->pages[page_num] = malloc(PAGE_SIZE);
    if (page == NULL) {
        printf("allocation for one page failed\n");
    }
  }
  uint32_t row_offset = row_num % ROWS_PER_PAGE;
  uint32_t byte_offset = row_offset * ROW_SIZE;
  return page + byte_offset;
}
