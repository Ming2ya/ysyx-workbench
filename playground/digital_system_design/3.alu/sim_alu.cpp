#include<stdio.h>
#include<verilated.h>
#include<verilated_fst_c.h>
#include"Valu.h"

int main(){
    Valu* dut = new Valu;
    Verilated::traceEverOn(true);
    VerilatedFstC* fst = new VerilatedFstC;
    dut->trace(fst, 5);
    fst->open("wave.fst");
    int time = 0;

    dut->x = 0xA; dut->y = 0x5; dut->op = 0b000;
    dut->eval(); fst->dump(time); time ++;

    dut->x = 0x1; dut->y = 0xF; dut->op = 0b000;
    dut->eval(); fst->dump(time); time ++;

    dut->x = 0xA; dut->y = 0xF; dut->op = 0b001;
    dut->eval(); fst->dump(time); time ++;

    dut->x = 0xA; dut->y = 0x5; dut->op = 0b001;
    dut->eval(); fst->dump(time); time ++;

    dut->x = 0x7; dut->y = 0x4; dut->op = 0b001;
    dut->eval(); fst->dump(time); time ++;

    dut->x = 0xA; dut->y = 0x5; dut->op = 0b010;
    dut->eval(); fst->dump(time); time ++;

    dut->x = 0xA; dut->y = 0x5; dut->op = 0b011;
    dut->eval(); fst->dump(time); time ++;

    dut->x = 0xA; dut->y = 0x5; dut->op = 0b100;
    dut->eval(); fst->dump(time); time ++;

    dut->x = 0xA; dut->y = 0x5; dut->op = 0b101;
    dut->eval(); fst->dump(time); time ++;

    dut->x = 0xA; dut->y = 0x5; dut->op = 0b110;
    dut->eval(); fst->dump(time); time ++;

    dut->x = 0xA; dut->y = 0xA; dut->op = 0b110;
    dut->eval(); fst->dump(time); time ++;

    dut->x = 0x5; dut->y = 0x5; dut->op = 0b110;
    dut->eval(); fst->dump(time); time ++;

    dut->x = 0xA; dut->y = 0x5; dut->op = 0b111;
    dut->eval(); fst->dump(time); time ++;

    dut->x = 0xA; dut->y = 0xA; dut->op = 0b111;
    dut->eval(); fst->dump(time); time ++;

    dut->x = 0x5; dut->y = 0x5; dut->op = 0b111;
    dut->eval(); fst->dump(time); time ++;

    fst->close();
    dut->final();
    return 0;
}