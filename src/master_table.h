#pragma once
#include <stdint.h>
#include "pointer_tags.h"

#define MASTER_INIT_SUCCESS 0
#define MASTER_INIT_NO_TABLE 1

#define MASTER_GET_NONE_LEFT 1
#define MASTER_GET_SUCCESS 0

#define MASTER_RETURN_UNUSED 1

#define ENTRY_USE_MASK (1 << 0)

typedef struct table_entry_t table_entry_t;
struct table_entry_t 
{
    union 
    {
        void* raw;
        table_entry_t* next;
    };
    uint16_t flags;
};

typedef uint32_t table_id_t;

typedef struct 
{
    table_entry_t* entries;
    TAGGED_PTR(table_entry_t*, head);
    uint32_t amount_entries;
} master_table_t;


int master_table_init(master_table_t* master_table, void* entries, unsigned int num);

int get_next_id(master_table_t* master_table, table_id_t* id);

int return_id(master_table_t* master_table, table_id_t id);
