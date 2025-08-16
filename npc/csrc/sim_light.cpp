#include<stdio.h>
#include"Vlight.h"
#include<verilated_fst_c.h>
#include<nvboard.h>

static Vlight* dut = new Vlight;

void nvboard_bind_all_pins(Vlight* top);

void single_cycle() {
  dut->clk = 0; dut->eval();
  dut->clk = 1; dut->eval();
}

void reset(int n) {
  dut->rst = 1;
  while (n -- > 0) single_cycle();
  dut->rst = 0;
}

int main(){
	Verilated::traceEverOn(true);
	VerilatedFstC* tfp = new VerilatedFstC;
	dut->trace(tfp, 5);
	tfp->open("wave.fst");
	nvboard_bind_all_pins(dut);
	nvboard_init();
	
	reset(10);
	uint64_t sim_time = 100000000;
	uint64_t time = 0;
	while(sim_time > time){
		nvboard_update();
		single_cycle();
		tfp->dump(time);
		time ++;
	}
	tfp->close();
	nvboard_quit();
	dut->final();
	delete dut;
}
