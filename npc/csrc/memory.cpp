#include <stdio.h>
#include "memory.h"

static uint8_t pmem[MSIZE] = {};

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



void pmem_init(){
    *(uint32_t *)(pmem + 0) = 0x00000493;  // addi s1, zero, 0 (initialize s1 to 0)
    *(uint32_t *)(pmem + 4) = 0x00448493;  // addi s1, s1, 4 (add positive small imm, s1=4)
    *(uint32_t *)(pmem + 8) = 0xfff48493;  // addi s1, s1, -1 (add negative imm, s1=3)
    *(uint32_t *)(pmem + 12) = 0x7ff48493; // addi s1, s1, 2047 (add max positive imm, s1=2050)
    *(uint32_t *)(pmem + 16) = 0x80048493; // addi s1, s1, -2048 (add min negative imm, s1=2)
    *(uint32_t *)(pmem + 20) = 0x00048493; // addi s1, s1, 0 (add zero imm, no-op, s1=2)
    *(uint32_t *)(pmem + 24) = 0x00150513; // addi a0, a0, 1 (test different rd/rs1, a0 was 0, now 1)
    *(uint32_t *)(pmem + 28) = 0xffe50513; // addi a0, a0, -2 (negative on different reg, a0=-1)
    *(uint32_t *)(pmem + 32) = 0x40050513; // addi a0, a0, 1024 (mid positive, check sign ext, a0=1023)
    *(uint32_t *)(pmem + 36) = 0xc0050513; // addi a0, a0, -1024 (mid negative, a0=-1)
    *(uint32_t *)(pmem + 40) = 0x00158593; // addi a1, a1, 1 (another reg, a1=1)
    *(uint32_t *)(pmem + 44) = 0x7fe58593; // addi a1, a1, 2046 (near max, a1=2047)
    *(uint32_t *)(pmem + 48) = 0x00100593; // addi a1, zero, 1 (from zero to another reg)
    *(uint32_t *)(pmem + 52) = 0xfff00593; // addi a1, zero, -1 (negative from zero)
    *(uint32_t *)(pmem + 56) = 0x00000013; // addi zero, zero, 0 (test write to zero, should remain 0)
    *(uint32_t *)(pmem + 60) = 0x00100013; // addi zero, zero, 1 (attempt write to zero, ignored)
    *(uint32_t *)(pmem + 64) = 0x55548493; // addi s1, s1, 1365 (test arbitrary bit pattern in imm, s1=1367)
    *(uint32_t *)(pmem + 68) = 0xaaa48493; // addi s1, s1, -1366 (negative arbitrary, s1=1)
    *(uint32_t *)(pmem + 72) = 0x001f8f93; // addi t6, t6, 1 (test high reg, t6=1)
    *(uint32_t *)(pmem + 76) = 0xffef8f93; // addi t6, t6, -2 (negative on high reg, t6=-1)
    *(uint32_t *)(pmem + 80) = 0x7fff8f93; // addi t6, t6, 2047 (max on high reg, t6=2046)
    *(uint32_t *)(pmem + 84) = 0x800f8f93; // addi t6, t6, -2048 (min on high reg, t6=-2)
}
