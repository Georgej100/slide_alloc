#pragma once
#include <stdint.h>

#define MASTER_INIT_SUCCESS 0
#define MASTER_INIT_NO_TABLE 1

#define MASTER_GET_NONE_LEFT 1
#define MASTER_GET_SUCCESS 0

#define MASTER_RETURN_UNUSED 1

#define ENTRY_USE_MASK (1 << 0)

typedef struct table_entry_t table_entry_t;
struct table_entry_t 
{
    void* raw;
    table_entry_t* next;
    uint16_t flags;
};

typedef uint32_t table_id_t;

typedef struct 
{
    table_entry_t* entries;
    table_entry_t* head;
    uint32_t amount_entries;
} master_table_t;


int initialize_master_table(master_table_t* master_table);

int get_next_id(master_table_t* master_table, table_id_t* id);

int return_id(master_table_t* master_table, table_id_t id);
