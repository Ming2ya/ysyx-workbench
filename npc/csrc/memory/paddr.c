#include "host.h"
#include "memory/paddr.h"
#include "isa.h"

static uint8_t pmem[MSIZE] __attribute((aligned(4096)))= {};

uint8_t* guest_to_host(paddr_t paddr) { return pmem + paddr - MBASE; }
paddr_t host_to_guest(uint8_t *haddr) { return haddr - pmem + MBASE; }

word_t pmem_read(paddr_t addr, int len){
    word_t ret = host_read(guest_to_host(addr), len);
    return ret;
}

static void pmem_write(paddr_t addr, int len, word_t data) {
    host_write(guest_to_host(addr), len, data);
}

void init_mem(){
    
}
