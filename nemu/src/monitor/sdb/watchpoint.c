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

#include "sdb.h"

#define NR_WP 32

typedef struct watchpoint {
  int NO;
  char expr[64];
  word_t pre_val;
  struct watchpoint *next;

  /* TODO: Add more members if necessary */

} WP;

static WP wp_pool[NR_WP] = {};
static WP *head = NULL, *free_ = NULL;

void init_wp_pool() {
  int i;
  for (i = 0; i < NR_WP; i ++) {
    wp_pool[i].NO = i;
    wp_pool[i].next = (i == NR_WP - 1 ? NULL : &wp_pool[i + 1]);
  }

  head = NULL;
  free_ = wp_pool;
}

/* TODO: Implement the functionality of watchpoint */

WP* new_wp(char* str){
    Assert(free_ != NULL, "Not enough watchpoint in the wp_pool");
    word_t val;
    bool success;
    val = expr(str, &success);
    if (success){
        WP* head_next = head;
        head = free_;
        free_ = free_->next;
        head->next = head_next;
        head->pre_val = val;
        strcpy(head->expr, str);
        Log("Watchpoint %d: %s", head->NO, head->expr);
        return head;
    }
    else {
        Log("Can't eval the wp, check and try again");
        return NULL;
    }
}

static void free_wp(WP *wp){
    WP* node, *pre_node;
    node = head;
    if (node == wp){
        pre_node = head;
        head = head->next;
        pre_node->next = free_;
        free_ = pre_node;
        return;
    }
    while (node->next != NULL){
        pre_node = node;
        node = node->next;
        if (node == wp){
            pre_node->next = node->next;
            node->next = free_->next;
            free_ = node;
            return;
        }
    }
    Log("unkown watchpoint, fail to free");
    return;
}

bool diff_wp(){
    WP* node = head;
    word_t val;
    while (node != NULL){
        val = expr(node->expr, NULL);
        if (val != node->pre_val){
            Log("Watchpoint %d: %s", node->NO, node->expr);
            Log("Old value = %u", node->pre_val);
            Log("New value = %u", val);
            return false;
        }
        node = node->next;
    }
    return true;
}

void info_wp(){
    if (head == NULL){
        printf("No watchpoints\n");
    }
    else {
        printf("%-10s%-10s%-15s\n", "Num", "Type", "What");
        WP* node = head;
        while (node != NULL){
            printf("%-10d%-10s%-15s\n", node->NO, "watchpoint", node->expr);
        }
    }
}

void del_wp(int num){
    WP* node = head;
    while (node != NULL){
        if (node->NO == num){
            free_wp(node);
            printf("delete watchpoint %d\n", num);
            return;
        }
    }
    printf("No watchpoint number %d\n", num);
}