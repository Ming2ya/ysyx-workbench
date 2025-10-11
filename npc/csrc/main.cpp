#include<stdio.h>
#include<verilated.h>
#include<verilated_fst_c.h>
#include"Vtop.h"

int main() {
    Vtop* top = new Vtop;
    Verilated::traceEverOn(true);
    VerilatedFstC* fst = new VerilatedFstC;
    top->trace(fst, 5);
    fst->open("wave.fst");
    int time = 0;


    fst->close();
    top->final();
    return 0;
}

