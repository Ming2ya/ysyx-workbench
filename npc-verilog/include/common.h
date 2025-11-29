//Refer to nemu

#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <string.h>
#include "macro.h"
#include <assert.h>
#include <stdlib.h>

#include "Vtop.h"

typedef uint32_t word_t;
typedef int32_t sword_t;
#define FMT_WORD "0x%08" PRIx32

typedef uint32_t paddr_t;
typedef uint32_t vaddr_t;
#define FMT_PADDR "0x%08" PRIx32

#include "debug.h"

#endif