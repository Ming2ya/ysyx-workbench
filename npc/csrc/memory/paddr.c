#include <stdio.h>
#include "memory/paddr.h"

static uint8_t pmem[MSIZE] __attribute((aligned(4096)))= {};

static const uint32_t img [] = {
  0x00000297,  // auipc t0,0
  0x00028823,  // sb  zero,16(t0)
  0x0102c503,  // lbu a0,16(t0)
  0x00100073,  // ebreak (used as nemu_trap)
  0xdeadbeef,  // some data
};

uint8_t* guest_to_host(paddr_t paddr) { return pmem + paddr - MBASE; }
paddr_t host_to_guest(uint8_t *haddr) { return haddr - pmem + MBASE; }

static word_t host_read(void *addr, int len) {
  switch (len) {
    case 1: return *(uint8_t  *)addr;
    case 2: return *(uint16_t *)addr;
    case 4: return *(uint32_t *)addr;
    default: assert(0);         //不支持的读取长度
  }
}

word_t pmem_read(paddr_t addr, int len){
    word_t ret = host_read(guest_to_host(addr), len);
    return ret;
}

void init_mem(){
    memcpy(guest_to_host(RESET_VECTOR), img, sizeof(img));
}
