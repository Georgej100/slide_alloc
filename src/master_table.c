#include <stddef.h>
#include <stdint.h>
#include <memory.h>
#include <string.h>
#include "master_table.h"
#include "masks.h"


int initialize_master_table(master_table_t* master_table)
{
    for (table_id_t id = 0; id < master_table->amount_entries; id++) 
    {
        if (id == master_table->amount_entries)
        {
            master_table->entries[id].next = NULL;
            SET_FLAG(master_table->entries[id].flags, ENTRY_USE_MASK);             
            return MASTER_INIT_SUCCESS;
        }

        master_table->entries[id].next = &master_table->entries[id + 1];
    }
    
    master_table->head = &master_table->entries[0];
    return MASTER_INIT_SUCCESS;
}

