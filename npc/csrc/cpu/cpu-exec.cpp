#include <verilated_vcd_c.h>
#include "VTop.h"
#include <memory/vaddr.h>
#include <isa.h>

CPU_state cpu = {}; // 占位符

static int sim_time;
static VTop *dut = NULL;
static VerilatedVcdC *vcd = NULL;

static void update_time(){
    sim_time += 5;
}

static void single_cyc() {
    dut->clock = 1; dut->eval(); vcd->dump(sim_time); update_time();
    dut->clock = 0; dut->eval(); vcd->dump(sim_time); update_time();
}

static void reset(int n){
    dut->reset = 1;
    while (n-- > 0) single_cyc();
    dut->reset = 0;
    dut->eval();
}

void init_sim(){
    dut = new VTop;
    vcd = new VerilatedVcdC;
    Verilated::traceEverOn(true);
    dut->trace(vcd, 5);
    vcd->open("build/wave.vcd");
    reset(10);
}

void exit_sim(){
    vcd->dump(sim_time);
    dut->final();
    vcd->close();
}

static void exec_once() {
    dut->io_inst = vaddr_ifetch(dut->io_pc, 4);
    single_cyc();
}

static void execute(uint64_t n){
    for (; n > 0; n --){
        exec_once();
        if (npc_state.state != NPC_RUNNING) break;
    }
}

void cpu_exec(uint64_t n){
    execute(n);
}

void assert_fail_msg() {
  //isa_reg_display();
  //statistic();
}