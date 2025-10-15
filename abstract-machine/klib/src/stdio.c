#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
char *itoa(int value, char* s);

int printf(const char *fmt, ...) {
  va_list ap;
  char *s;
  int d;
  char c;
  int i = 0;
  int cnt = 0;
  // char* flags = "+_#0";
  // int width;
  // 控制选项待实现 

  va_start(ap, fmt);
  while(fmt[i] != '\0'){
    if (fmt[i] != '%') {
      putch(fmt[i]);
      i += 1; cnt += 1;
    }
    else {
      i += 1;
      switch (fmt[i]) {
        case 's':
          s = va_arg(ap, char*);
          cnt += strlen(s);
          putstr(s);
          break;
        case 'd':
          d = va_arg(ap, int);
          char digit[12];
          s = itoa(d, digit);
          cnt += strlen(s);
          putstr(s);
          break;
        case 'c':
          c = va_arg(ap, int);
          cnt += 1;
          putch(c);
          break;
        case '%':
          putch('%');
          cnt += 1;
          break;
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
