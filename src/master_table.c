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

    for (uint32_t id = 0; id < master_table->amount_entries; id++) 
    {
        if (id == master_table->amount_entries - 1)
        {
            master_table->entries[id].next = NULL;
            memset(&master_table->entries[id].controls, 0, sizeof(controls_t));         
            atomic_store(&master_table->head, (_Atomic table_entry_t*)&master_table->entries[0]);
            return MASTER_INIT_SUCCESS;
        }
        
        memset(&master_table->entries[id].controls, 0, sizeof(controls_t));      
        master_table->entries[id].next = &master_table->entries[id + 1];

    }
    
    atomic_store(&master_table->head, (_Atomic table_entry_t*)&master_table->entries[0]);
    return MASTER_INIT_SUCCESS;
}

#define ENTRY_ID_FROM_ADDR(base, addr) (addr - base)   // Derives the ID using pointer arithmetic

int get_next_id(master_table_t* master_table, table_id_t* id) {
    void* old_head = atomic_load(&master_table->head);
    table_entry_t* new_node;

    do {
        new_node = UNTAG_PTR(old_head);
        if (new_node == NULL) {
            return -1; // List is empty
        }

        void* next_node = new_node->next; 
        void* new_head = PACK_FLAGS(next_node, EXTRACT_FLAGS(old_head) + 1);

        if (atomic_compare_exchange_weak(&master_table->head, &old_head, new_head)) {
            break; 
        }
        
    } while (1);

    
    *id = ENTRY_ID_FROM_ADDR(master_table->entries, new_node);

    controls_t controls = {
        .flags = ENTRY_USE_MASK,
        .reference_count = 0,
    };

    atomic_store(&new_node->controls, controls);

    return MASTER_GET_SUCCESS;
}

int return_id(master_table_t* master_table, table_id_t id)
{
    table_entry_t* entry = &master_table->entries[id];

    /*
    if (!IS_FLAG(entry->controls.flags, ENTRY_USE_MASK)) {
        return MASTER_RETURN_UNUSED;
    }
    */
    controls_t controls = {
        .flags = 0,
        .reference_count = 0,
    };

    atomic_store(&entry->controls, controls);

    void* old_head = atomic_load(&master_table->head);
    void* new_head;

    do {
        entry->next = UNTAG_PTR(old_head);

        new_head = PACK_FLAGS(entry, EXTRACT_FLAGS(old_head) + 1);

    } while (!atomic_compare_exchange_weak(&master_table->head, &old_head, new_head));

    return 0;
}
