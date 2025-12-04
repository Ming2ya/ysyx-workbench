#include <verilated_vcd_c.h>
#include "VTop.h"

#include "local-include/reg.h"
#include <cpu/decode.h>
#include <cpu/cpu.h>

static int sim_time;
static VTop *dut = NULL;
static VerilatedVcdC *vcd = NULL;

static void update_time(){
    sim_time += 5;
}

extern "C" {

void vaddr_write(vaddr_t addr, int len, word_t data);

void single_cyc() {
    dut->clock = 1; dut->eval(); vcd->dump(sim_time); update_time();
    dut->clock = 0; dut->eval(); vcd->dump(sim_time); update_time();
}

static void reset(int n){
    dut->reset = 1;
    while (n-- > 0) single_cyc();
    dut->reset = 0;
    dut->eval();
}

void init_sim(){
    dut = new VTop;
    vcd = new VerilatedVcdC;
    Verilated::traceEverOn(true);
    dut->trace(vcd, 5);
    vcd->open("build/wave.vcd");
    reset(10);
}

void exit_sim(){
    vcd->dump(sim_time);
    dut->final();
    vcd->close();
}

int decode_exec(Decode *s) {
    dut->io_inst = s->isa.inst;
    single_cyc();
    s->dnpc = dut->io_pc;
    // reg 通过DPI-C更新过
    if (dut->io_valid == 0) INV(s->pc);
    if (dut->io_ebreak) NPCTRAP(s->pc, gpr(10));
    return 0;
}

}
