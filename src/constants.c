#include <stdint.h>
#include "row.h"
#include "constants.h"

const uint32_t  COLUMN_USERNAME_SIZE = 32;

const uint32_t  COLUMN_EMAIL_SIZE = 255;

const uint32_t USERNAME_LEN_SIZE = size_of_attribute(Row, username_len);

const uint32_t EMAIL_LEN_SIZE = size_of_attribute(Row, email_len);

const uint32_t ID_SIZE = size_of_attribute(Row, id);

const uint32_t USERNAME_SIZE = size_of_attribute(Row, username);

const uint32_t EMAIL_SIZE = size_of_attribute(Row, email);

const uint32_t USERNAME_LEN_OFFSET = 0;

const uint32_t EMAIL_LEN_OFFSET = USERNAME_LEN_OFFSET + USERNAME_LEN_SIZE;

const uint32_t ID_OFFSET = EMAIL_LEN_OFFSET + EMAIL_LEN_SIZE;

const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;

const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;

const uint32_t ROW_SIZE = USERNAME_LEN_SIZE + EMAIL_LEN_SIZE + ID_SIZE + COLUMN_USERNAME_SIZE + COLUMN_EMAIL_SIZE;

const uint32_t PAGE_SIZE = 4096;

const uint32_t TABLE_MAX_PAGES = 100;

const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;

const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE * TABLE_MAX_PAGES;

