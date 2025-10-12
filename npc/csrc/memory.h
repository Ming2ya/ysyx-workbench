#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdint.h>
#include <assert.h>

#define MSIZE 0x8000
#define MBASE 0x80000000

typedef uint32_t word_t;
typedef uint32_t paddr_t;
typedef uint32_t vaddr_t;

word_t pmem_read(paddr_t addr, int len);
void pmem_init();

#endif