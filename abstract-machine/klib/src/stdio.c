#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
char *itoa(int value, char* s);
#define is_digit(c) c >= '0' && c <= '9'

int printf(const char *fmt, ...) {
  va_list ap;
  char *s;
  int d;
  char c;
  int i = 0;
  int cnt = 0;
  // printf格式 %[argument$][flags][width][.precision][length modifier]conversion
  char* flags = "#0- +";
  struct {
    char pad;
    char align;
    int width;
  } padding;
  //char* conversions = "sdc%";
  // argument$ precision length-modifier 未实现

  va_start(ap, fmt);
  while(fmt[i] != '\0'){
    if (fmt[i] != '%') {
      putch(fmt[i]);
      i += 1; cnt += 1;
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
      char digit[8];
      while (is_digit(fmt[i])) {
        digit[len] = fmt[i];
        i ++; len ++;
      }
      digit[len] = '\0';
      padding.width = atoi(digit);
      // conversion
      switch (fmt[i]) {
        int len;
        case 's':
          s = va_arg(ap, char*);
          len = strlen(s);
          if (len >= padding.width) {
            cnt += len;
            putstr(s);
          }
          else {
            cnt += padding.width;
            if (padding.align == '+') for (int i = 0; i < padding.width - len; i ++) putch(padding.pad);
            putstr(s);
            if (padding.align == '-') for (int i = 0; i < padding.width - len; i ++) putch(padding.pad);
          }
          break;
        case 'd':
          d = va_arg(ap, int);
          char digit[12];
          s = itoa(d, digit);
          len = strlen(s);
          if (len >= padding.width) {
            cnt += len;
            putstr(s);
          }
          else {
            cnt += padding.width;
            if (padding.align == '+') for (int i = 0; i < padding.width - len; i ++) putch(padding.pad);
            putstr(s);
            if (padding.align == '-') for (int i = 0; i < padding.width - len; i ++) putch(padding.pad);
          }
          break;
        case 'c':
          c = va_arg(ap, int);
          if (padding.width <= 1){
            cnt += 1;
            putch(c);
          }
          else {
            cnt += padding.width;
            if (padding.align == '+') for (int i = 0; i < padding.width - 1; i ++) putch(padding.pad);
            putch(c);
            if (padding.align == '-') for (int i = 0; i < padding.width - 1; i ++) putch(padding.pad);
          }
          break;
        case '%':
          putch('%');
          cnt += 1;
          break;
        default: assert(0);  // 未实现或未知类型
      }
      i += 1;
    }
  }
  va_end(ap);
  return cnt;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  int i = 0, k = 0;
  char *s;
  int d;
  char c;

  while(fmt[i] != '\0'){
    if (fmt[i] != '%') {
      out[k] = fmt[i];
      k ++; i ++;
    }
    else {
      i += 1;
      switch (fmt[i]) {
        case 's':
          s = va_arg(ap, char*);
          while (*s != '\0') {
            out[k++] = *s++;
          }
          break;
        case 'd':
          d = va_arg(ap, int);
          char digit[12];
          s = itoa(d, digit);
          while (*s != '\0') {
            out[k++] = *s++;
          }
          break;
        case 'c':
          c = va_arg(ap, int);
          out[k] = c;
          k += 1;
          break;
        case '%':
          out[k] = '%';
          k += 1;
          break;
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
