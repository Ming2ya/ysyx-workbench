#include<stdio.h>
#include<verilated.h>
#include<verilated_fst_c.h>
#include"Vencode.h"

int main(){
    Vencode* dut = new Vencode;
    Verilated::traceEverOn(true);
    VerilatedFstC* fst = new VerilatedFstC;
    dut->trace(fst, 5);
    fst->open("wave.fst");

    int time = 0;
    dut->en = 1;
    dut->x = 0b00000000; dut->eval(); fst->dump(time);
    dut->x = 0b00000001;
    for (int i = 0; i < 8; i ++){
        time ++;
        dut->eval();
        fst->dump(time);
        dut->x = dut->x * 2;
    }

    time ++; dut->en = 0;
    dut->x = 0b00000000; dut->eval(); fst->dump(time);
    dut->x = 0b00000001;
    for (int i = 0; i < 8; i ++){
        time ++;
        dut->eval();
        fst->dump(time);
        dut->x = dut->x * 2;
    }

    time ++; dut->en = 1;
    dut->x = 0b00000000; dut->eval(); fst->dump(time);
    dut->x = 0b00100001;
    for (int i = 0; i < 8; i ++){
        time ++;
        dut->eval();
        fst->dump(time);
        dut->x = dut->x * 2;
    }

    fst->close();
    dut->final();
    return 0;
}