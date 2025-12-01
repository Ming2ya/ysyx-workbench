/***************************************************************************************
* Copyright (c) 2014-2024 Zihao Yu, Nanjing University
*
* NEMU is licensed under Mulan PSL v2.
* You can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
*
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
*
* See the Mulan PSL v2 for more details.
***************************************************************************************/

#include "local-include/reg.h"
#include <cpu/cpu.h>
#include <cpu/ifetch.h>
#include <cpu/decode.h>

#include <verilated_vcd_c.h>
#include "VTop.h"

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

static int decode_exec(Decode *s) {
  dut->io_inst = s->isa.inst;
  single_cyc();
  s->dnpc = dut->io_pc;
  // reg 通过DPI-C更新过
  if (dut->io_valid == 0) INV(s->pc);
  if (dut->io_ebreak) NPCTRAP(s->pc, gpr(10));
  return 0;
}

int isa_exec_once(Decode *s) {
  s->isa.inst = inst_fetch(&s->snpc, 4);
  return decode_exec(s);
}
