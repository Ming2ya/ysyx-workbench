#include <verilated.h>
#include <verilated_vcd_c.h>
#include "Vtop.h"
#include "memory/paddr.h"

static int sim_time;
static Vtop *dut = NULL;
static VerilatedVcdC *vcd = NULL;

void init_sim(){
    dut = new Vtop;
    vcd = new VerilatedVcdC;
    Verilated::traceEverOn(true);
    dut->trace(vcd, 5);
    vcd->open("build/wave.vcd");
}

void close_sim(){
    vcd->dump(sim_time);
    dut->final();
    vcd->close();
}

static void update_time(){
    sim_time += 5;
}

static void single_cyc() {
    dut->clk = 1; dut->eval(); vcd->dump(sim_time); update_time();
    dut->clk = 0; dut->eval(); vcd->dump(sim_time); update_time();
}

static void cycle(int n) {
    for (int i = 0; i < n; i ++) {
        single_cyc();
    }
}

void restart_npc() {
    single_cyc();
    dut->clk = 0;
    dut->reset = 1;
    cycle(2);
    dut->reset = 0;
}

#include "cpu/ifetch.h"
static void exec_once() {
    dut->inst = vaddr_ifetch(dut->pc, 4);
    single_cyc();
}

static void execute(uint64_t n){
    for (; n > 0; n --){
        exec_once();
        if (dut->state != NPC_RUNNING) break;
    }
}

void cpu_exec(uint64_t n){
    execute(n);

    switch (dut->state) {
        case NPC_RUNNING: npc_state.state = NPC_STOP; break;

        case NPC_END: case NPC_ABORT:
            Log("NPC: %s at pc = " FMT_WORD,
                (dut->state == NPC_ABORT ? ANSI_FMT("ABORT", ANSI_FG_RED) :
                (npc_state.halt_ret == 0 ? ANSI_FMT("HIT GOOD TRAP", ANSI_FG_GREEN) :
                ANSI_FMT("HIT BAD TRAP", ANSI_FG_RED))),
                dut->pc);
      // fall through
    case NPC_QUIT: ;
  }
}
