#include <stddef.h>
#include <stdint.h>
#include <memory.h>
#include "master_table.h"
#include "masks.h"
#include "assert.h"
#include "tagging.h"

#include <stdio.h>

int master_table_init(master_table_t* master_table, void* entries, unsigned int num)
{
    ASSERT_RET(master_table != NULL, MASTER_INIT_NO_TABLE);

    master_table->amount_entries = num;
    master_table->entries = entries;

    for (table_id_t id = 0; id < master_table->amount_entries; id++) 
    {
        if (id == master_table->amount_entries - 1)
        {
            master_table->entries[id].next = NULL;
            CLEAR_FLAG(master_table->entries[id].flags, ENTRY_USE_MASK);         
            master_table->head = &master_table->entries[0];
            return MASTER_INIT_SUCCESS;
        }
        
        CLEAR_FLAG(master_table->entries[id].flags, ENTRY_USE_MASK);         
        master_table->entries[id].next = &master_table->entries[id + 1];

    }
    
    master_table->head = &master_table->entries[0];
    return MASTER_INIT_SUCCESS;
}

#define ENTRY_ID_FROM_ADDR(base, addr) (addr - base)   // Derives the ID using pointer arithmetic


int get_next_id(master_table_t* master_table, table_id_t* id)
{
    // Get first entry in linked list and check if NULL
    table_entry_t* new = UNTAG_PTR(master_table->head);
    ASSERT_RET(new != NULL, -1);

    // Assigns head to next in line
    master_table->head = new->next;
    new->next = NULL;
    master_table->head = PACK_FLAGS(master_table->head, EXTRACT_FLAGS(master_table->head) + 1);

    *id = ENTRY_ID_FROM_ADDR(master_table->entries, new);

    // Marks entry as inuse
    SET_FLAG(new->flags, ENTRY_USE_MASK);

    return MASTER_GET_SUCCESS;
}

int return_id(master_table_t* master_table, table_id_t id)
{
    // Check if entry is free
    ASSERT_RET(!IS_FLAG(master_table->entries[id].flags, ENTRY_USE_MASK), MASTER_RETURN_UNUSED);

    // Get entry addr
    table_entry_t* entry = &master_table->entries[id];
    
    // Re-add to linked list
    entry->next = master_table->head;
    master_table->head = entry;
    CLEAR_FLAG(entry->flags, ENTRY_USE_MASK);
    master_table->head = PACK_FLAGS(master_table->head, EXTRACT_FLAGS(master_table->head) + 1);

    return 0;
}
