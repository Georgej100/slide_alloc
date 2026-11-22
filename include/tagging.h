#pragma once
#include <stdint.h>

// Mask to isolate the valid 48-bit address space (Bits 0 to 47)
#define ADDRESS_MASK 0x0000FFFFFFFFFFFFULL

// Mask to isolate the upper 16 flag bits (Bits 48 to 63)
#define FLAG_MASK    0xFFFF000000000000ULL

#define PACK_FLAGS(ptr, flags)   ((void*)(((uintptr_t)(ptr) & ADDRESS_MASK) | ((uintptr_t)(flags) << 48)))
#define EXTRACT_FLAGS(tagged_ptr) ((uint16_t)((uintptr_t)(tagged_ptr) >> 48))

// Safe dereferencing relies on cleaning the pointer first
#define UNTAG_PTR(tagged_ptr)    ((void*)((uintptr_t)(tagged_ptr) & ADDRESS_MASK))