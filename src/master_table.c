#include <stddef.h>
#include <stdint.h>
#include <memory.h>
#include <string.h>
#include "master_table.h"
#include "masks.h"
#include "assert.h"

int initialize_master_table(master_table_t* master_table)
{
    ASSERT_RET(master_table != NULL, MASTER_INIT_NO_TABLE);

    for (table_id_t id = 0; id < master_table->amount_entries; id++) 
    {
        if (id == master_table->amount_entries)
        {
            master_table->entries[id].next = NULL;
            CLEAR_FLAG(master_table->entries[id].flags, ENTRY_USE_MASK);             
            return MASTER_INIT_SUCCESS;
        }
        
        SET_FLAG(master_table->entries[id].flags, ENTRY_USE_MASK);         
        master_table->entries[id].next = &master_table->entries[id + 1];
    }
     
    master_table->head = &master_table->entries[0];
    return MASTER_INIT_SUCCESS;
}

int get_next_id(master_table_t* master_table, table_id_t* id)
{
    table_entry_t* next = master_table->head;
    ASSERT_RET(next != NULL, MASTER_GET_NONE_LEFT);
    master_table->head = next->next;
    next->next = NULL;

    *id = (next - master_table->entries);
    SET_FLAG(next->flags, ENTRY_USE_MASK);

    return MASTER_GET_SUCCESS;
}

int return_id(master_table_t* master_table, table_id_t id)
{
    ASSERT_RET(!IS_FLAG(master_table->entries[id].flags, ENTRY_USE_MASK), MASTER_RETURN_UNUSED);

    table_entry_t* entry = &master_table->entries[id];
    entry->next = master_table->head;
    master_table->head = entry;
    CLEAR_FLAG(entry->flags, ENTRY_USE_MASK);

    return 0;
}
