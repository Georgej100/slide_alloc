#pragma once
#include <stdint.h>

#define TAGGED_PTR(type, name)  union { void* raw; uintptr_t int_raw; type name; struct { uintptr_t address : 48; uintptr_t tag     : 16; } fields; }

#define UNTAG_PTR(tp) ((void*)((tp)->int_raw & 0x0000FFFFFFFFFFFFULL))

#define WRITE_ADDR(pstructure, data) pstructure->fields.address = (uintptr_t)data