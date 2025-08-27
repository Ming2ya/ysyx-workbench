#include<stdio.h>
#include"Vmux42.h"
#include<nvboard.h>

void nvboard_bind_all_pins(Vmux42* top);

int main(){
    Vmux42* dut = new Vmux42;
    nvboard_bind_all_pins(dut);
    nvboard_init();

    while (true){
        nvboard_update();
        dut->eval();
    }
    nvboard_quit();
}