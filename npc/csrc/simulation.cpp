#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vtop.h"
#include "paddr.h"

static int sim_time;
static Vtop *dut = NULL;
static VerilatedVcdC *vcd = NULL;

void init_sim(){
    dut = new Vtop;
    vcd = new VerilatedVcdC;
    Verilated::traceEverOn(true);
    dut->trace(vcd, 5);
    vcd->open("build/wave.vcd");
}

void close_sim(){
    vcd->dump(sim_time);
    dut->final();
    vcd->close();
}

static void update_time(){
    sim_time += 5;
}

static void single_cyc() {
    dut->clk = 1; dut->eval(); vcd->dump(sim_time); update_time();
    dut->clk = 0; dut->eval(); vcd->dump(sim_time); update_time();
}

static void cycle(int n) {
    for (int i = 0; i < n; i ++) {
        single_cyc();
    }
}

void exec_once() {
    dut->inst = pmem_read(dut->pc, 4);
    single_cyc();
}

void init_cpu() {
    single_cyc();
    dut->clk = 0;
    dut->reset = 1;
    cycle(3);
    dut->reset = 0;
}