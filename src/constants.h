#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <stdint.h>
#define size_of_attribute(Struct, Attribute) sizeof(((Struct*)0)->Attribute)


extern const uint32_t  COLUMN_USERNAME_SIZE;

extern const uint32_t  COLUMN_EMAIL_SIZE;

extern const uint32_t USERNAME_LEN_SIZE;

extern const uint32_t EMAIL_LEN_SIZE;

extern const uint32_t ID_SIZE;

extern const uint32_t USERNAME_SIZE;

extern const uint32_t EMAIL_SIZE;

extern const uint32_t USERNAME_LEN_OFFSET;

extern const uint32_t EMAIL_LEN_OFFSET;

extern const uint32_t ID_OFFSET;

extern const uint32_t USERNAME_OFFSET;

extern const uint32_t EMAIL_OFFSET;

extern const uint32_t ROW_SIZE;

extern const uint32_t PAGE_SIZE;

extern const uint32_t TABLE_MAX_PAGES;

extern const uint32_t ROWS_PER_PAGE;

extern const uint32_t TABLE_MAX_ROWS;

#endif
