#include <common.h>

static char ringbuf[CONFIG_RINGTRACE_LENGTH][128];
static int ringhead = 0;

void ringtrace_add(char* log) {
    strcpy(ringbuf[ringhead], log);
    ringhead = (ringhead + 1) % CONFIG_RINGTRACE_LENGTH;
}

void ringtrace_print() {
    printf("Instruction Ring Trace:");
    for (int i = 0; i < CONFIG_RINGTRACE_LENGTH; i ++) {
        puts(ringbuf[ringhead]);
        ringhead = (ringhead + 1) % CONFIG_RINGTRACE_LENGTH;
    }
}