#include <am.h>
#include <klib.h>
#include <klib-macros.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
static unsigned long int next = 1;

int rand(void) {
  // RAND_MAX assumed to be 32767
  next = next * 1103515245 + 12345;
  return (unsigned int)(next/65536) % 32768;
}

void srand(unsigned int seed) {
  next = seed;
}

int abs(int x) {
  return (x < 0 ? -x : x);
}

int atoi(const char* nptr) {
  int x = 0;
  while (*nptr == ' ') { nptr ++; }
  while (*nptr >= '0' && *nptr <= '9') {
    x = x * 10 + *nptr - '0';
    nptr ++;
  }
  return x;
}

char *itoa(int value, char* s) {
    int i = 0, k = 0;
    char temp[12];
    if (value < 0){
        value = -value;
        s[i] = '-';
        i++;
    }
    if (value == 0){
        s[0] = '0';
        s[1] = '\0';
        return s;
    }
    while (value > 0){
        temp[k] = '0' + value % 10;
        value = value / 10;
        k ++;
    }
    while (k > 0) {
        s[i ++] = temp[--k];
    }
    s[i] = '\0';
    return s;
}                       //非stdlib函数，自己添加的

void *malloc(size_t size) {
  // On native, malloc() will be called during initializaion of C runtime.
  // Therefore do not call panic() here, else it will yield a dead recursion:
  //   panic() -> putchar() -> (glibc) -> malloc() -> panic()
#if !(defined(__ISA_NATIVE__) && defined(__NATIVE_USE_KLIB__))
  panic("Not implemented");
#endif
  return NULL;
}

void free(void *ptr) {
}

#endif
