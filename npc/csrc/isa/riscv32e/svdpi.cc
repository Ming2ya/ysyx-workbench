#include <svdpi.h>
#include <isa.h>
#include "local-include/reg.h"

extern "C"{
void npc_update_reg(uint32_t addr, uint32_t data){
    Log("Write gpr%d: 0x%08x", addr, data);
    gpr(addr) = data;
}

word_t paddr_read(paddr_t addr, int len);
word_t npc_inst_fetch(uint32_t addr){
    return paddr_read(addr, 4);
}
}