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

int npc_exec_once();
uint32_t npc_get_inv();
uint32_t npc_get_ebreak();
uint32_t npc_get_pc();
uint32_t npc_get_inst();

int isa_exec_once(Decode *s) {
  s->isa.inst = npc_get_inst();
  s->snpc = s->snpc + 4;
  // inst 通过dpi读取
  npc_exec_once();
  // gpr 通过dpi更新
  s->dnpc = npc_get_pc();
  if (npc_get_ebreak()) NPCTRAP(s->pc, gpr(10));
  if (npc_get_inv()) INV(s->pc);
  return 0;
  
}
