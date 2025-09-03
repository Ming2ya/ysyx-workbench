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

#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#define EXPR_LOG 1

enum {
  TK_NOTYPE = 256, TK_EQ,

  /* TODO: Add more token types */
  TK_DECIMAL, TK_HEX, TK_NE, TK_REG, TK_DEREF
};

struct result {     //struct的static是只能在构造后面创建变量，有没有像函数static那样只能在本文件内访问的方法？
    word_t ok;
    bool err;
};

static struct result eval(int p, int q);
static int check_parentheses(int p, int q);
static int find_main_op(int p, int q);
word_t isa_reg_str2val(const char *s, bool *success);
word_t paddr_read(paddr_t addr, int len);

static struct rule {
  const char *regex;
  int token_type;
  int token_priority;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE, -2},            // spaces
  {"\\+", '+', 4},                  // plus
  {"==", TK_EQ, 7},                 // equal
  {"0x[0-9]+", TK_HEX, -1},         // hexdecimal_number
  {"[0-9]+", TK_DECIMAL, -1},       // decimal_number
  {"-", '-', 4},
  {"\\*", '*', 3},
  {"/", '/', 3},
  {"\\(", '(', -1},
  {"\\)", ')', -1},
  {"!=", TK_NE, 7},
  {"&&", '&', 11},
  {"\\$[a-z]*[0-9]*",TK_REG, -1},
  {"", TK_DEREF, 2}
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  int priority;
  word_t value;
} Token;

static Token tokens[1024] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

static void make_value(char* substr, int len){
    char str[32] = {};
    strncpy(str, substr, len);
    str[len] = '\0';
    tokens[nr_token].value = strtol(str, NULL, 0);
}
static void make_reg(char* substr, int len){
    char str[32] = {};
    bool success;
    strncpy(str, substr, len);
    str[len] = '\0';
    tokens[nr_token].value = isa_reg_str2val(str + 1, &success);
    if (!success){
        Log("Unknown register");
    }
}

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        IFONE(EXPR_LOG, Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start));

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        if (rules[i].token_type == TK_NOTYPE){
            break;
        }
        else if (rules[i].token_priority < 0){
            if (rules[i].token_type == TK_REG)
                make_reg(substr_start, substr_len);
            else
                make_value(substr_start, substr_len);
            
        }
        tokens[nr_token].type = rules[i].token_type;
        tokens[nr_token].priority = rules[i].token_priority;
        nr_token ++; 

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}


word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    Log("Fail to make tokens");
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  for (int i = 0; i < nr_token; i ++) {
    if (tokens[i].type == '*' && (i == 0 || tokens[i - 1].priority > 0) ) {
        tokens[i].type = TK_DEREF;
        tokens[i].priority = 2;
    }
  }
  struct result val = eval(0, nr_token - 1);
  *success = ! val.err;
  return val.ok;
}

static int check_parentheses(int p, int q){
    if (tokens[p].type != '(' || tokens[q].type != ')')
        return 0;
    int depth = 1;
    int flag = 1;
    for (int i = p + 1; i < q; i ++){
        if (tokens[i].type == '(')
            depth += 1;
        else if (tokens[i].type == ')')
            depth -= 1;
        
        if (depth == 0)
            flag = 0;
        else if (depth < 0){
            Log("Bad parentheses");
            return -1;
        }
    }
    if (depth == 1 && flag)
        return 1;
    else {
        Log("Bad parentheses");
        return -1;
    }
}

static int find_main_op(int p, int q){
    int depth = 0;
    int main_op = -1;
    int main_priority = 0;
    for (int i = p; i <= q; i ++){
        if (tokens[i].type == '(')
            depth += 1;
        else if (tokens[i].type == ')')
            depth -= 1;
        
        if (depth < 0){
            Log("Bad parentheses");
            return -2;
        }
        else if (depth > 0 || tokens[i].priority < main_priority )
            continue;
        else {
            main_op = i;
            main_priority = tokens[i].priority;
        }
    }
    if (main_op < 0){
        Log("Main op not found");
        return -1;
    } 
    IFONE(EXPR_LOG, printf("main op at %d\n", main_op));
    return main_op;
}

static struct result eval(int p, int q){
    struct result val;
    val.err = false;
    val.ok = 0;
    if (p > q){
        Log("Bad expression");
        val.err = true;
        return val;
    }
    else if (p == q){
        if (tokens[p].priority != -1){
            Log("Not a number at end");
            val.err = true;
        }
        val.ok = tokens[p].value;
        return val;
    }
    else if (check_parentheses(p, q) == 1){
        return eval(p + 1, q - 1);
    }
    else if (check_parentheses(p, q) == -1){
        val.err = true;
        return val;
    }
    else if (tokens[p].priority == 2){          //单目运算
        int op = p;
        struct result val1 = eval(p + 1, q);
        if (val1.err == true)
            return val1;
        switch(tokens[op].type){
            case TK_DEREF: val.ok = paddr_read(val1.ok, 4); break;
            default: 
                Log("No match operator");
                val.err = true;
        }
        return val;
    }
    else {
        int op = find_main_op(p, q);
        if (op < 0){
            val.err = true;
            return val;
        }
        if(tokens[op].priority < 0){
            Log("Not an operator as main op");
            val.err = true;
            return val;
        }
        struct result val1 = eval(p, op - 1);
        struct result val2 = eval(op + 1, q);
        if (val1.err == true)
            return val1;
        if (val2.err == true)
            return val2;
        IFONE(EXPR_LOG, printf("val1 = %u\n", val1.ok));
        IFONE(EXPR_LOG, printf("val2 = %u\n", val2.ok));
        switch (tokens[op].type){
            case '+': val.ok = val1.ok + val2.ok; break;
            case '-': val.ok = val1.ok - val2.ok; break;
            case '*': val.ok = val1.ok * val2.ok; break;
            case '/': 
                if (val2.ok == 0){
                    Log("div 0 error");
                    val.err = true;
                    return val;
                }
                val.ok =  val1.ok / val2.ok; break;
            case TK_EQ: val.ok = val1.ok == val2.ok; break;
            case TK_NE: val.ok = val1.ok != val2.ok; break;
            case '&': val.ok = val1.ok && val2.ok; break;
            default: 
                Log("No match operator");
                val.err = true;
        }
        return val;
    }
}
