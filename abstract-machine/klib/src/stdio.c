#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
char *itoa(int value, char* s, int base);
#define is_digit(c) c >= '0' && c <= '9'

int printf(const char *fmt, ...) {
  char buf[1024];

  va_list ap;
  va_start(ap, fmt);
  int i = vsprintf(buf, fmt, ap);
  va_end(ap);

  putstr(buf);
  return i;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  int i = 0, k = 0;
  char *s;
  int d;
  char c;
  // printf格式 %[argument$][flags][width][.precision][length modifier]conversion
  //char* argument = "*$";
  char* flags = "#0- +";
  struct {
    char pad;
    char align;
    int width;
  } padding;
  //char* conversions = "sdxc%";
  // argument$ precision length-modifier 未实现

  while(fmt[i] != '\0'){
    if (fmt[i] != '%') {
      out[k] = fmt[i];
      k ++; i ++;
    }
    else {
      i += 1;
      padding.pad = ' ';  
      padding.align = '+';
      padding.width = 0;
      // flags
      while (strchr(flags, fmt[i]) != NULL) {
        switch (fmt[i]) {
          case '#':
            assert(0);  //与#相关的类型未实现
          case '0':
            padding.pad = '0';
            break;
          case '-':
            padding.align = '-';
            break;
          case ' ':
            assert(0);  // 未实现
          case '+':
            assert(0);  // 未实现
        }
        i += 1;
      }
      // field width
      int len = 0;
      char digit[32];
      while (is_digit(fmt[i])) {
        digit[len] = fmt[i];
        i ++; len ++;
      }
      digit[len] = '\0';
      padding.width = atoi(digit);
      //conversion
      switch (fmt[i]) {
        int len;
        case 's':
          s = va_arg(ap, char*);
          len = strlen(s);
          if (len >= padding.width) {
            while(*s) out[k++] = *s++;
          }
          else {
            if (padding.align == '+') for (int i = 0; i < padding.width - len; i ++) out[k++] = padding.pad;
            while(*s) out[k++] = *s++;
            if (padding.align == '-') for (int i = 0; i < padding.width - len; i ++) out[k++] = padding.pad;
          }
          break;
        case 'd':
          d = va_arg(ap, int);
          s = itoa(d, digit, 10);
          len = strlen(s);
          if (len >= padding.width) {
            while(*s) out[k++] = *s++;
          }
          else {
            if (padding.align == '+') for (int i = 0; i < padding.width - len; i ++) out[k++] = padding.pad;
            while(*s) out[k++] = *s++;
            if (padding.align == '-') for (int i = 0; i < padding.width - len; i ++) out[k++] = padding.pad;
          }
          break;
        case 'x':
          d = va_arg(ap, int);
          s = itoa(d, digit, 16);
          len = strlen(s);
          if (len >= padding.width) {
            while(*s) out[k++] = *s++;
          }
          else {
            if (padding.align == '+') for (int i = 0; i < padding.width - len; i ++) out[k++] = padding.pad;
            while(*s) out[k++] = *s++;
            if (padding.align == '-') for (int i = 0; i < padding.width - len; i ++) out[k++] = padding.pad;
          }
          break;
        case 'c':
          c = va_arg(ap, int);
          if (padding.width <= 1){
            out[k++] = c;
          }
          else {
            if (padding.align == '+') for (int i = 0; i < padding.width - 1; i ++) out[k++] = padding.pad;
            out[k++] = c;
            if (padding.align == '-') for (int i = 0; i < padding.width - 1; i ++) out[k++] = padding.pad;
          }
          break;
        case '%':
          out[k++] = '%';
          break;
        default: assert(0);  // 未实现或未知类型
      }
      i += 1;
    }
  }
  out[k] = '\0';
  return k;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int i = vsprintf(out, fmt, ap);
  va_end(ap);
  return i;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  panic("Not implemented");
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  panic("Not implemented");
}

#endif
