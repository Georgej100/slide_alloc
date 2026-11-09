#pragma once
#include <stdatomic.h>

#define SET_FLAG(flags, mask) flags |= mask
#define CLEAR_FLAG(flags, mask) flags &= ~mask
#define IS_FLAG(flags, mask) ((flags & mask) == mask)

#define ATOMIC_SET_FLAG(pflags, mask) atomic_fetch_or(pflags, mask)
#define ATOMIC_CLEAR_FLAG(pflags, mask) atomic_fetch_and(pflags, mask)
#define ATOMIC_IS_FLAG(pflags, mask) (atomic_load(pflags) & mask) 