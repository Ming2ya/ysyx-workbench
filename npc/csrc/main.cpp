#include <stdio.h>
#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vtop.h"
#include "memory.h"

void exec_once(Vtop* top, VerilatedVcdC* vcd);
void cpu_init(Vtop* top, VerilatedVcdC* vcd);

int main() {
    Vtop* top = new Vtop;
    Verilated::traceEverOn(true);
    VerilatedVcdC* vcd = new VerilatedVcdC;
    top->trace(vcd, 5);
    vcd->set_time_unit("1ns");
    vcd->set_time_resolution("1ns");
    vcd->open("wave.vcd");

    pmem_init();
    cpu_init(top, vcd);

    int i = 0;
    while (i < 30){
        i ++;
        exec_once(top, vcd);
    }


    vcd->close();
    top->final();
    return 0;
}
