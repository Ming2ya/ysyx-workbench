#include<stdio.h>
#include"Valu.h"
#include<nvboard.h>

void nvboard_bind_all_pins(Valu* top);

int main(){
    Valu* dut = new Valu;
    nvboard_bind_all_pins(dut);
    nvboard_init();

    while (true){
        nvboard_update();
        dut->eval();
    }
    nvboard_quit();
    dut->final();
}