//Refer to nemu

#ifndef __MEMORY_PADDR_H__
#define __MEMORY_PADDR_H__

#include "common.h"

#define MSIZE 0x80000
#define MBASE 0x80000000
#define PC_RESET_OFFSET 0

#define PMEM_LEFT  ((paddr_t)MBASE)
#define PMEM_RIGHT ((paddr_t)MBASE + MSIZE - 1)
#define RESET_VECTOR (PMEM_LEFT + PC_RESET_OFFSET)

void init_mem();
uint8_t* guest_to_host(paddr_t paddr);
paddr_t host_to_guest(uint8_t *haddr);

word_t pmem_read(paddr_t addr, int len);
void pmem_write(paddr_t addr, int len, word_t data);

#endif