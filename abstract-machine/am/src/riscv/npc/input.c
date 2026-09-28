#include <am.h>
#include "riscv/riscv.h"

#define KBD_ADDR 0xa0000060
#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd) {
  uint32_t am_scancode = inl(KBD_ADDR);
  kbd->keydown = (bool)(am_scancode / KEYDOWN_MASK);
  kbd->keycode = (int)(am_scancode % KEYDOWN_MASK);
}
