#pragma once

#define CLEAR_FLAGS(flags) flags &= 0
#define SET_FLAG(flags, mask) flags |= mask
#define CLEAR_FLAG(flags, mask) flags &= ~mask
#define IS_FLAG(flags, mask) ((flags & mask) == mask)

