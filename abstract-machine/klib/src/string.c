#include <klib.h>
#include <klib-macros.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  size_t len = 0;
  while (s[len] != '\0'){
    len += 1;
  }

  return len;
}                           //没有'\0'会怎样？

char *strcpy(char *dst, const char *src) {
  size_t i;
  size_t len = strlen(src) + 1; // '\0' 算入

  if ((dst <= src && dst + len > src) || (dst >= src && dst - len < src)){
    panic("strcpy strings must not overlap");
  }

  for (i = 0; src[i] != '\0'; i++){
    dst[i] = src[i];
  }
  dst[i] = '\0';
  
  return dst;
}

char *strncpy(char *dst, const char *src, size_t n) {
  size_t i;

  if ((dst <= src && dst + n > src) || (dst >= src && dst - n < src)){
    panic("strncpy strings must not overlap");
  }
  for (i = 0; i < n && src[i] != '\0'; i++){
    dst[i] = src[i];
  }
  for(; i < n; i++){
    dst[i] = '\0';
  }

  return dst;
}

char *strcat(char *dst, const char *src) {
  size_t dst_len = strlen(dst);
  size_t i;

  for (i = 0; src[i] != '\0'; i++){
    dst[dst_len + i] = src[i];
  }
  dst[dst_len + i] = '\0';

  return dst;
}

int strcmp(const char *s1, const char *s2) {
  size_t i = 0;
  while (s1[i] != '\0' && s2[i] != '\0'){
    if (s1[i] > s2[i]) return 1;
    else if (s1[i] < s2[i]) return -1;
    i += 1;
  }
  if (s1[i] != '\0' && s2[i] == '\0') return 1;
  else if (s1[i] == '\0' && s2[i] != '\0') return -1;
  else return 0;
}

int strncmp(const char *s1, const char *s2, size_t n) {
  size_t i = 0;
  while (s1[i] != '\0' && s2[i] != '\0' && i < n){
    if (s1[i] > s2[i]) return 1;
    else if (s1[i] < s2[i]) return -1;
    i += 1;
  }
  if (i == n) return 0;
  else if (s1[i] != '\0' && s2[i] == '\0') return 1;
  else if (s1[i] == '\0' && s2[i] != '\0') return -1;
  else return 0;
}

void *memset(void *s, int c, size_t n) {
  uint8_t* p = (uint8_t *)s;
  while (n--){
    *p++ = (uint8_t)c;
  }

  return s;
}

void *memmove(void *dst, const void *src, size_t n) {
  size_t i;
  uint8_t temp[n];

  for (i = 0; i < n; i ++){
    temp[i] = *((uint8_t *)src + i);
  }

  for (i = 0; i < n; i ++){
    *((uint8_t *)dst + i) = temp[i];
  }

  return dst;
}

void *memcpy(void *out, const void *in, size_t n) {
  if ((out >= in && out - n < in) || (out <= in && out + n > in))
    panic("memcpy mem must not overlap");

  size_t i;
  for (i = 0; i < n; i ++){
    *((uint8_t *)out + i) = *((uint8_t *)in + i);
  }

  return out;
}

int memcmp(const void *s1, const void *s2, size_t n) {
  size_t i;
  for (i = 0; i < n; i ++){
    unsigned char a, b;
    a = *((unsigned char *)s1 + i);
    b = *((unsigned char *)s2 + i);
    if (a > b) return 1;
    else if (a < b) return -1;
  }
  return 0;
}

#endif
