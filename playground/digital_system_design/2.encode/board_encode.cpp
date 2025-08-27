#include<stdio.h>
#include"Vencode.h"
#include<nvboard.h>

void nvboard_bind_all_pins(Vencode* top);

int main(){
    Vencode* dut = new Vencode;
    nvboard_bind_all_pins(dut);
    nvboard_init();

    while (true){
        nvboard_update();
        dut->eval();
    }
    nvboard_quit();
    dut->final();
}