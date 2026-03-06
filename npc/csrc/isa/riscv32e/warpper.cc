#include <verilated_vcd_c.h>
#include "VTop.h"

static int sim_time;
static VTop *dut = NULL;
static VerilatedVcdC *vcd = NULL;

static void update_time(){
    sim_time += 5;
}

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

extern "C" {

void init_sim(){
    dut = new VTop;
    vcd = new VerilatedVcdC;
    Verilated::traceEverOn(true);
    dut->trace(vcd, 5);
    vcd->open("build/wave.vcd");
    reset(3);
}

void exit_sim(){
    vcd->dump(sim_time);
    dut->final();
    vcd->close();
}

int npc_exec_once() {
    single_cyc();
    return 0;
}

uint32_t npc_get_pc() {
    return dut->io_pc;
}

uint32_t npc_get_inst() {
    return dut->io_inst;
}

uint32_t npc_get_ebreak() {
    return dut->io_ebreak;
}

uint32_t npc_get_inv() {
    return !dut->io_valid;
}
}