#pragma once
#include "alloc.h"
#include "master_table.h"

// Declared in alloc.c
extern heap_t global_heap;
extern master_table_t global_table_state;
extern table_entry_t* global_table;