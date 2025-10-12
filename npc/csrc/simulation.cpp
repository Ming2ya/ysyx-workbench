#include <verilated_vcd_c.h>
#include "Vtop.h"
#include "memory.h"

static int sim_time = 0;

static void update_time(){
    sim_time += 5;
}

static void single_cyc(Vtop* top, VerilatedVcdC* vcd) {
    top->clk = 1; top->eval(); vcd->dump(sim_time); update_time();
    top->clk = 0; top->eval(); vcd->dump(sim_time); update_time();
}

static void cycle(Vtop* top, VerilatedVcdC* vcd, int n) {
    for (int i = 0; i < n; i ++) {
        single_cyc(top, vcd);
    }
}

void exec_once(Vtop* top, VerilatedVcdC* vcd) {
    top->inst = pmem_read(top->pc, 4);
    single_cyc(top, vcd);
}

void cpu_init(Vtop* top, VerilatedVcdC* vcd) {
    top->clk = 0;
    top->reset = 1;
    cycle(top, vcd, 3);
    top->reset = 0;
    single_cyc(top, vcd);
}