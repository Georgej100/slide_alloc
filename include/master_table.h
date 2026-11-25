#pragma once
#include <stdint.h>
#include <stdatomic.h>

#define MASTER_TABLE_SIZE 400

#define MASTER_INIT_SUCCESS 0
#define MASTER_INIT_NO_TABLE -1

#define MASTER_GET_NONE_LEFT -1
#define MASTER_GET_SUCCESS 0

#define MASTER_RETURN_UNUSED -1

#define ENTRY_USE_MASK (1 << 0)

typedef struct 
{
    uint8_t flags;
    int8_t reference_count;
} controls_t;

typedef struct table_entry_t table_entry_t;
struct table_entry_t 
{
    _Atomic controls_t controls;
    
    union 
    {
        void* raw;
        table_entry_t* next;
    };
};

typedef int8_t table_id_t;

typedef struct 
{
    table_entry_t* entries;
    _Atomic table_entry_t* head;
    uint32_t amount_entries;
} master_table_t;


int master_table_init(master_table_t* master_table, void* entries, unsigned int num);

int get_next_id(table_id_t* id);

int return_id(table_id_t id);
