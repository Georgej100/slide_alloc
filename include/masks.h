#pragma once
#include <stdatomic.h>

#define SET_FLAG(flags, mask) flags |= mask
#define CLEAR_FLAG(flags, mask) flags &= ~mask
#define IS_FLAG(flags, mask) ((flags & mask) == mask)
 
#define SET_FLAG_ATOMIC(pflags, mask) atomic_fetch_or(pflags, mask) 
#define CLEAR_FLAG_ATOMIC(pflags, mask) atomic_fetch_and(pflags, ~mask)
#define IS_FLAG_ATOMIC(pflags, mask) (atomic_fetch_and(pflags, mask) > 0)