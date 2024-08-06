#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include "table.h"
#include "constants.h"

Pager* pager_open(const char* filename) {
    int fd = open(filename, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    //int fd = open(filename, O_RDWR | O_CREAT, S_IWUSR | S_IRUSR); //Create file if it does not exist. User has read and write permissions
    if (fd == -1) {
        printf("failed to open file: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    off_t file_length = lseek(fd, 0, SEEK_END);

    Pager* pager = (Pager*) malloc(sizeof(Pager));
    if (pager == NULL) {
        printf("failed to allocate pager\n");
        exit(EXIT_FAILURE);
    }
    pager->file_descriptor = fd;
    pager->file_length = file_length;

    pager->pages = malloc(TABLE_MAX_PAGES * 8);
    if (pager->pages == NULL) {
        printf("allocation for pages failed\n");
        exit(EXIT_FAILURE);
    }

    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        pager->pages[i] = NULL;
    }

    return pager;
}

Table* db_open(const char* filename) {
    Pager* pager = pager_open(filename);
    uint32_t num_rows = pager->file_length / ROW_SIZE;

    Table* table = (Table*)malloc(sizeof(Table));
    if (table == NULL) {
        printf("allocation for table failed\n");
        exit(EXIT_FAILURE);
    }
    table->pager = pager;
    table->num_rows = num_rows;

    return table;
}


void* get_page(Pager* pager, uint32_t page_num) {
    if (page_num > TABLE_MAX_PAGES) {
        printf("Tried to fetch page number out of bounds\n");
        exit(EXIT_FAILURE);
    }
    if (pager->pages[page_num] == NULL) {
        // need to load from file
        void* page = malloc(PAGE_SIZE);
        uint32_t num_pages = pager->file_length/PAGE_SIZE;
        if (pager->file_length % PAGE_SIZE) { //partial page at end of file
            num_pages++;
        }

        if (page_num <= num_pages) {
            lseek(pager->file_descriptor, page_num * PAGE_SIZE, SEEK_SET); //set file pointer to beginning of page on file
            ssize_t bytes_read = read(pager->file_descriptor, page, PAGE_SIZE); 
            if (bytes_read == -1) {
                printf("Error reading file: %d\n", errno);
                exit(EXIT_FAILURE);
            }
        }
        pager->pages[page_num] = page;
    }
    return pager->pages[page_num];

}

void* row_slot(Table* table, uint32_t row_num) {
  uint32_t page_num = row_num / ROWS_PER_PAGE;

  void* page = get_page(table->pager, page_num);

  uint32_t row_offset = row_num % ROWS_PER_PAGE;
  uint32_t byte_offset = row_offset * ROW_SIZE;
  return page + byte_offset;
}

void pager_flush(Pager* pager, uint32_t page_num, uint32_t size) {
    if (pager->pages[page_num] == NULL) {
        printf("Tried to flush NULL page\n");
        exit(EXIT_FAILURE);
    }

    lseek(pager->file_descriptor, page_num * size, SEEK_SET); //set file pointer to beginning of page on file
    ssize_t bytes_written = write(pager->file_descriptor, pager->pages[page_num], size); 
    if (bytes_written == -1) {
        printf("Error writing to file: %d\n", errno);
        exit(EXIT_FAILURE);
    }
}

//flush page cache to disk
//close db file
//free memory for pager and table structures
void db_close(Table* table) {
    Pager* pager = table->pager;
    uint32_t num_full_pages = table->num_rows / ROWS_PER_PAGE;

    for (uint32_t i = 0; i < num_full_pages; i++) {
        if (pager->pages[i] != NULL) {
            pager_flush(pager, i, PAGE_SIZE);
            pager->pages[i] = NULL;
        }
    }

    //take care of partial page
    if (pager->pages[num_full_pages] != NULL) {
        pager_flush(pager, num_full_pages, (table->num_rows % ROWS_PER_PAGE) * ROW_SIZE);
        pager->pages[num_full_pages] = NULL; 
    }

    int result = close(pager->file_descriptor);
    if (result != 0) {
        printf("Failed to close file\n");
        exit(EXIT_FAILURE);
    }

    for (uint32_t i = 0; i < TABLE_MAX_PAGES; i++) {
        if (pager->pages[i]) {
            free(pager->pages[i]);
        }
    }
    free(pager->pages);
    free(pager);
    free(table);
}
