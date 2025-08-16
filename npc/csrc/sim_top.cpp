#include<memory.h>
#include<stdio.h>
#include"Vtop.h"
#include<verilated_fst_c.h>
#include<assert.h>
#include<stdlib.h>

int main(){
	const std::unique_ptr<VerilatedContext> contextp{new VerilatedContext};
	contextp->traceEverOn(true);
	const std::unique_ptr<Vtop> top{new Vtop{contextp.get(), "TOP"}};
	VerilatedFstC* tfp = new VerilatedFstC;
	top->trace(tfp, 3);
	tfp->open("wave.fst");
	int i = 0;
	while (i < 10){
		i++;
		int a = rand() & 1;
		int b = rand() & 1;
		top->a = a;
		top->b = b;
		top->eval();
		printf("a = %d, b = %d, f = %d\n", a, b, top->f);
		assert(top->f == (a ^ b));
	}
	tfp->close();
	top->final();
}
