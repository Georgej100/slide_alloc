#pragma once
#include "libslide.h"

#define THREAD_COUNT 30
#define TOTAL_ALLOCATIONS 300
#define USE_SALLOC

#define RNG_SEED 131313

#ifdef NORMAL_ACCESS
#define GET_PTR(token) (token)
#define RELEASE_PTR(token) 
#else
#define GET_PTR(token) dereference(token)
#define RELEASE_PTR exit_dereference(token)
#endif

#ifdef USE_SALLOC
#define UNIT table_id_t
#define ALLOC(size) salloc(size)
#define FREE(token) sfree(token)
#endif

#ifdef USE_STD
#define UNIT void*
#define ALLOC(size) malloc(size)
#define FREE(token) free(token)
#endif

int stress(void);