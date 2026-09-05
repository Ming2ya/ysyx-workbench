#include <svdpi.h>
#include <isa.h>
#include "local-include/reg.h"

extern "C"{

word_t paddr_read(paddr_t addr, int len);
word_t pmem_read(paddr_t addr, int len);
void pmem_write(paddr_t addr, int len, word_t data);
void mtrace_read(vaddr_t addr, int len, word_t data);
void mtrace_write(vaddr_t addr, int len, word_t data);
void mtrace_reg(const char* name, uint32_t data);

void npc_update_reg(uint32_t addr, uint32_t data){
#ifdef CONFIG_MTRACE_COND
    if (MTRACE_COND) { mtrace_reg(reg_name(addr), data); }
#endif
    gpr(addr) = data;
}

word_t npc_inst_fetch(uint32_t addr){
    return paddr_read(addr, 4);
}

word_t npc_pmem_read(uint32_t addr){
    uint32_t data = pmem_read(addr, 4);
    return data;
}

void npc_mtrace_read(uint32_t addr, uint32_t data){
#ifdef CONFIG_MTRACE_COND
    if (MTRACE_COND) { mtrace_read(addr, 4, data); }
#endif
}

void npc_pmem_write(uint32_t waddr, uint32_t wdata, char wmask){
    uint32_t mask =
        ((wmask & 0x1) ? 0x000000ffu : 0) |
        ((wmask & 0x2) ? 0x0000ff00u : 0) |
        ((wmask & 0x4) ? 0x00ff0000u : 0) |
        ((wmask & 0x8) ? 0xff000000u : 0);

    for (int i = 0; i < 4; i++) {
        if (wmask & (1u << i)) {
            uint32_t byte = (wdata >> (i * 8)) & 0xff;
            pmem_write(waddr + i, 1, byte);
        }
    }
#ifdef CONFIG_MTRACE_COND
    if (MTRACE_COND) { mtrace_write(waddr, 4, wdata & mask); }
#endif
}
}