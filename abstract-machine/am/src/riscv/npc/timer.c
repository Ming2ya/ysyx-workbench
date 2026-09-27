#include <am.h>
#include "riscv/riscv.h"
#define RTC_ADDR 0xa0000040

static uint64_t boot_time;

void __am_timer_init() {
    boot_time = (uint64_t)inl(RTC_ADDR + 4) << 32 | (uint64_t)inl(RTC_ADDR);
}

void __am_timer_uptime(AM_TIMER_UPTIME_T *uptime) {
  uint64_t now = (uint64_t)inl(RTC_ADDR + 4) << 32 | (uint64_t)inl(RTC_ADDR);
  uptime->us = now - boot_time;
}

void __am_timer_rtc(AM_TIMER_RTC_T *rtc) {
  rtc->second = inl(RTC_ADDR + 0x08);
  rtc->minute = inl(RTC_ADDR + 0x0c);
  rtc->hour   = inl(RTC_ADDR + 0x10);
  rtc->day    = inl(RTC_ADDR + 0x14);
  rtc->month  = inl(RTC_ADDR + 0x18);
  rtc->year   = inl(RTC_ADDR + 0x1c);
}
