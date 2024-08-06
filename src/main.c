#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "constants.h"
#include "row.h"
#include "table.h"

typedef struct {
    char* buffer;
    size_t buffer_len;
    ssize_t input_len;
} InputBuffer;


typedef enum { STATEMENT_INSERT, STATEMENT_SELECT } StatementType;

typedef struct {
    Row* row_to_insert;
    StatementType type;
} Statement;

typedef enum { META_COMMAND_SUCCESS, META_COMMAND_UNRECOGNIZED_COMMAND } MetaCommandResult;

typedef enum { PREPARE_SUCCESS, PREPARE_SYNTAX_ERROR, PREPARE_UNRECOGNIZED_STATEMENT } PrepareResult;

typedef enum { EXECUTE_SUCCESS, EXECUTE_TABLE_FULL, EXECUTE_FAILURE } ExecuteResult;

InputBuffer* new_input_buffer() {
    InputBuffer* input_buffer = (InputBuffer*)malloc(sizeof(InputBuffer));
    input_buffer->buffer = NULL;
    input_buffer->buffer_len = 0;
    input_buffer->input_len = 0;
    return input_buffer;
}

void print_prompt() {
    printf(">> ");
}

void read_input(InputBuffer* input_buffer) {
    ssize_t bytes_read = getline(&(input_buffer->buffer), &(input_buffer->buffer_len), stdin);
    if (bytes_read <= 0) {
        printf("Error reading input\n");
        exit(EXIT_FAILURE);
    }

    input_buffer->input_len = bytes_read - 1;
    input_buffer->buffer[input_buffer->input_len] = 0;
}


void close_input_buffer(InputBuffer* input_buffer) {
    free(input_buffer->buffer);
    free(input_buffer);
}

MetaCommandResult do_meta_command(InputBuffer* input_buffer) {
    if (strcmp(input_buffer->buffer, ".exit") == 0) {
        close_input_buffer(input_buffer);
        exit(EXIT_SUCCESS);
    }
    else {
        return META_COMMAND_UNRECOGNIZED_COMMAND;
    }
}

PrepareResult prepare_statement(InputBuffer* input_buffer, Statement* statement) {
    if (strncmp(input_buffer->buffer, "select", 6) == 0) {
        statement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }
    else if (strncmp(input_buffer->buffer, "insert", 6) == 0) {
        statement->type = STATEMENT_INSERT;
        statement->row_to_insert = new_row();
        int args_assigned = sscanf(input_buffer->buffer, "insert %d %s %s", &(statement->row_to_insert->id),
	statement->row_to_insert->username, statement->row_to_insert->email);
        if (args_assigned < 3) {
            return PREPARE_SYNTAX_ERROR;
        }
        return PREPARE_SUCCESS;
    }
    else {
        return PREPARE_UNRECOGNIZED_STATEMENT;
    }
}

ExecuteResult execute_select(Table* table, Statement* statement) {
    Row row;
    for (uint32_t i = 0; i < table->num_rows; i++) {
        deserialize_row(row_slot(table, i), &row);
        print_row(&row);
    }
    return EXECUTE_SUCCESS;
}


ExecuteResult execute_insert(Table* table, Statement* statement) {
    if (table->num_rows >= TABLE_MAX_ROWS) {
        return EXECUTE_TABLE_FULL;
    }
    Row* row_to_insert = statement->row_to_insert;
    serialize_row(row_to_insert, row_slot(table, table->num_rows));
    table->num_rows++;
    return EXECUTE_SUCCESS;
}

ExecuteResult execute_statement(Table* table, Statement* statement) {
    if (statement->type == STATEMENT_SELECT) {
        return execute_select(table, statement);
    }
    else if (statement->type == STATEMENT_INSERT) {
        return execute_insert(table, statement);
    }
    return EXECUTE_FAILURE;
}

int main(int argc, char** argv) {
    Table* table = new_table();
    InputBuffer* input_buffer = new_input_buffer();
    while(1) {
        print_prompt();
        read_input(input_buffer);
        if (input_buffer->buffer[0] == '.') {
            switch (do_meta_command(input_buffer)) {
                case (META_COMMAND_SUCCESS):
                        continue;
                case (META_COMMAND_UNRECOGNIZED_COMMAND):
                        printf("Unrecognized meta command: '%s'\n", input_buffer->buffer);
                        continue;
            }
        }

        Statement statement;
        switch (prepare_statement(input_buffer, &statement)) {
            case (PREPARE_SUCCESS):
                break;
            case (PREPARE_SYNTAX_ERROR):
                printf("Syntax error: '%s'\n", input_buffer->buffer);
                continue;
            case (PREPARE_UNRECOGNIZED_STATEMENT):
                printf("Unrecognized statement: '%s'\n", input_buffer->buffer);
                continue;
        }

        execute_statement(table, &statement);
        printf("Executed\n");
    }
    free_table(table);
}
