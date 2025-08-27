#include<stdio.h>
#include"VLFSR.h"
#include<nvboard.h>

void nvboard_bind_all_pins(VLFSR* top);

int main(){
    VLFSR* dut = new VLFSR;
    nvboard_bind_all_pins(dut);
    nvboard_init();

    while (true){
        nvboard_update();
        dut->eval();
    }
    nvboard_quit();
    dut->final();
}