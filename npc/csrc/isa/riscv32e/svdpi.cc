#include <svdpi.h>
#include <isa.h>
#include "local-include/reg.h"

extern "C"{
void npc_update_reg(uint32_t addr, uint32_t data){
#ifdef CONFIG_MTRACE_COND
    Log("Write %-3s: 0x%08x", reg_name(addr), data);
#endif
    gpr(addr) = data;
}

word_t paddr_read(paddr_t addr, int len);
word_t pmem_read(paddr_t addr, int len);
void pmem_write(paddr_t addr, int len, word_t data);
void mtrace_read(vaddr_t addr, int len, word_t data);
void mtrace_write(vaddr_t addr, int len, word_t data);

word_t npc_inst_fetch(uint32_t addr){
    return paddr_read(addr, 4);
}

word_t npc_pmem_read(uint32_t addr){
    uint32_t data = pmem_read(addr, 4);
#ifdef CONFIG_MTRACE_COND
    if (MTRACE_COND) { mtrace_read(addr, 4, data); }
#endif
    return data;
}

void npc_pmem_write(uint32_t waddr, uint32_t wdata, char wmask){
    uint32_t bitMask;
    if (wmask == 0x1) bitMask = 0x000000FF;
    else if (wmask == 0x3) bitMask = 0x0000FFFF;
    else if (wmask == 0xF) bitMask = 0xFFFFFFFF;
    else panic("unexpected wmask");

    uint32_t oldData = paddr_read(waddr, 4);
    wdata = (wdata & bitMask) | (oldData & ~bitMask);
#ifdef CONFIG_MTRACE_COND
    if (MTRACE_COND) { mtrace_write(waddr, 4, wdata); }
#endif
    pmem_write(waddr, 4, wdata);
}
}