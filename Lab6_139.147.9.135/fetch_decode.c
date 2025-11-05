#include <stdio.h>
#include <string.h>
#include "fetch_decode.h"
#include "execute.h"

static int str_equal(const char *a, const char *b) {
    while (*a && *b) {
        if (*a != *b) return 0;
        a++;
        b++;
    }
    return *a == '\0' && *b == '\0';
}

void remove_newline(char *str) {
    while (*str) {
        if (*str == '\n') {
            *str = '\0';
            return;
        }
        str++;
    }
}

void remove_extra_spaces(char *str) {
    char *src = str;
    char *dst = str;
    int in_space = 0;

    while (*src == ' ') src++;

    while (*src) {
        if (*src == ' ') {
            if (!in_space) {
                *dst++ = ' ';
                in_space = 1;
            }
        } else {
            *dst++ = *src;
            in_space = 0;
        }
        src++;
    }

    if (dst > str && *(dst - 1) == ' ') dst--;

    *dst = '\0';
}

int fetch_decode_execute() {
    char line[MAXLINE];
    if (!fgets(line, MAXLINE, stdin)) return 1;

    remove_newline(line);
    remove_extra_spaces(line);

    if (line[0] == '\0') return 0; 
    char *token = strtok(line, " ");
    char *op1 = strtok(NULL, " ");
    char *op2 = strtok(NULL, " ");

    if (str_equal(token, "DISPLAY")) {
        execute_display();
    } else if (str_equal(token, "HALT")) {
        return execute_halt();
    } else if (str_equal(token, "CLEAR")) {
        execute_clear();
    } else if (str_equal(token, "NOVA")) {
        if (op1) execute_nova(op1);
    } else if (str_equal(token, "MOV")) {
        if (op1 && op2) execute_mov(op1, op2);
    } else if (str_equal(token, "MOVA")) {
        if (op1) execute_mova(op1);
    } else if (str_equal(token, "LOAD")) {
        if (op1 && op2) execute_load(op1, op2);
    } else if (str_equal(token, "ADD")) {
        if (op1 && op2) execute_add(op1, op2);
    } else if (str_equal(token, "SUB")) {
        if (op1 && op2) execute_sub(op1, op2);
    } else if (str_equal(token, "AND")) {
        if (op1 && op2) execute_and(op1, op2);
    } else if (str_equal(token, "OR")) {
        if (op1 && op2) execute_or(op1, op2);
    } else if (str_equal(token, "XOR")) {
        if (op1 && op2) execute_xor(op1, op2);
    } else if (str_equal(token, "NEG")) {
        if (op1) execute_neg(op1);
    } else if (str_equal(token, "SHL")) {
        if (op1 && op2) execute_shl(op1, op2);
    } else if (str_equal(token, "SHR")) {
        if (op1 && op2) execute_shr(op1, op2);
    }

    return 0; 
}
