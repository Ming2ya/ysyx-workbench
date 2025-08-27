#include<stdio.h>
#include"Vmux42.h"
#include<verilated_fst_c.h>

void my_trace(VerilatedFstC* fst){
    static uint32_t time = 0;
    for (int j = 0; j < 5; j ++){
        fst->dump(time);
        time += 1;
    }
}

int main(){
    Vmux42* dut = new Vmux42;
    Verilated::traceEverOn(true);
    VerilatedFstC* fst = new VerilatedFstC;
    dut->trace(fst, 5);
    fst->open("wave.fst");

    dut->x1 = 0;
    dut->x2 = 1;
    dut->x3 = 2;
    dut->x4 = 3;

    for (int i = 0; i < 4; i ++){
        dut->s = i;
        dut->eval();
        my_trace(fst);
    }

    fst->close();
    dut->final();
    return 0;
}

