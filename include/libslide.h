#include <stdint.h>

#ifndef LIB_SLIDE_ALLOC
#define LIB_SLIDE_ALLOC

typedef int8_t table_id_t;

int heap_init(uint16_t size);

table_id_t salloc(uint16_t size);

int sfree(table_id_t id);

void* dereference(table_id_t id);

int exit_dereference(table_id_t id);

int bubble_compact();

void print_heap();

void print_master_table();

void print_master_free();

#endif