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

#include <device/map.h>
#include <device/alarm.h>
#include <utils.h>
#include <time.h>

static uint32_t *rtc_port_base = NULL;

static void rtc_io_handler(uint32_t offset, int len, bool is_write) {
  assert(offset % 4 == 0);
  if (!is_write && offset == 4) {
    uint64_t us = get_uptime();
    rtc_port_base[0] = (uint32_t)us;
    rtc_port_base[1] = us >> 32;
  }
  if (!is_write && offset == 8) {
    time_t rtc_time = get_rtc_time() / 1000000;
    struct tm calendar;
    gmtime_r(&rtc_time, &calendar);
    rtc_port_base[2] = calendar.tm_sec;
    rtc_port_base[3] = calendar.tm_min;
    rtc_port_base[4] = calendar.tm_hour;
    rtc_port_base[5] = calendar.tm_mday;
    rtc_port_base[6] = calendar.tm_mon + 1;
    rtc_port_base[7] = calendar.tm_year + 1900;
  }
}

#ifndef CONFIG_TARGET_AM
static void timer_intr() {
  if (npc_state.state == NPC_RUNNING) {
    extern void dev_raise_intr();
    dev_raise_intr();
  }
}
#endif

void init_timer() {
  rtc_port_base = (uint32_t *)new_space(32);
#ifdef CONFIG_HAS_PORT_IO
  add_pio_map ("rtc", CONFIG_RTC_PORT, rtc_port_base, 32, rtc_io_handler);
#else
  add_mmio_map("rtc", CONFIG_RTC_MMIO, rtc_port_base, 32, rtc_io_handler);
#endif
  IFNDEF(CONFIG_TARGET_AM, add_alarm_handle(timer_intr));
}
