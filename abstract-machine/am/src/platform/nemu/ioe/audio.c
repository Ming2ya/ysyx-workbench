#include <am.h>
#include <nemu.h>

#define AUDIO_FREQ_ADDR      (AUDIO_ADDR + 0x00)
#define AUDIO_CHANNELS_ADDR  (AUDIO_ADDR + 0x04)
#define AUDIO_SAMPLES_ADDR   (AUDIO_ADDR + 0x08)
#define AUDIO_SBUF_SIZE_ADDR (AUDIO_ADDR + 0x0c)
#define AUDIO_INIT_ADDR      (AUDIO_ADDR + 0x10)
#define AUDIO_COUNT_ADDR     (AUDIO_ADDR + 0x14)

static int sbuf_size, wfd;

void __am_audio_init() {
}

void __am_audio_config(AM_AUDIO_CONFIG_T *cfg) {
  cfg->present = true;
  sbuf_size = inl(AUDIO_SBUF_SIZE_ADDR);
  cfg->bufsize = sbuf_size;
}

void __am_audio_ctrl(AM_AUDIO_CTRL_T *ctrl) {
  outl(AUDIO_FREQ_ADDR, ctrl->freq);
  outl(AUDIO_CHANNELS_ADDR, ctrl->channels);
  outl(AUDIO_SAMPLES_ADDR, ctrl->samples);
  wfd = 0;
  outl(AUDIO_INIT_ADDR, 1);
}

void __am_audio_status(AM_AUDIO_STATUS_T *stat) {
  stat->count = inl(AUDIO_COUNT_ADDR);
}

void __am_audio_play(AM_AUDIO_PLAY_T *ctl) {
  int audio_size = ctl->buf.end - ctl->buf.start;
  // Wait until sbuf have enough space
  for (int i = 0; i < audio_size; i++) {
    while (sbuf_size < 1 + io_read(AM_AUDIO_STATUS).count);
    outb(AUDIO_SBUF_ADDR + wfd, ((uint8_t *)(ctl->buf.start))[i]);
    wfd = (wfd + 1) % sbuf_size;
    outl(AUDIO_COUNT_ADDR, 1 + inl(AUDIO_COUNT_ADDR));
  }
}
