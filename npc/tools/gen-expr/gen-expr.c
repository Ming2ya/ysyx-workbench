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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char expr[65536] = {};
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = (unsigned)%s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";
static int buf_index = 0;
static int expr_index = 0;
static int depth = 0;

static uint32_t choose(uint32_t n){
    return rand() % n;
}

static void gen_blank(){
    int num = choose(4);
    for (int i = 0; i < num; i++){
        expr[expr_index] = ' ';
        expr_index ++;
    }
}

static void gen(char a){
    buf[buf_index] = a;
    expr[expr_index] = a;
    buf_index ++;
    expr_index ++;
    gen_blank();
}

static void gen_unsign(){
    buf[buf_index] = 'U';
    buf_index ++;
}

static void gen_rand_op(){
    char op[]= {'+', '-', '*', '/'};
    gen(op[choose(4)]);
}

static void gen_rand_num(){
    gen(choose(10) + '0');
    gen_unsign();
}

static void gen_rand_expr() {
    if (depth > 10){
        gen_rand_num();
    }
    else {
        switch (choose(3)){
            case 0: gen_rand_expr(); gen_rand_op(); gen_rand_expr(); depth ++; break;
            case 1: gen('('); gen_rand_expr(); gen(')'); break;
            default: gen_rand_num();
        }
    }
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    depth = 0;
    buf_index = 0;
    expr_index = 0;
    gen_rand_expr();
    buf[buf_index] = '\0';
    expr[expr_index] = '\0';

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc -Werror /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    ret = fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, expr);
  }
  return 0;
}
