#include <stdio.h>
#include "execute.h"
#include "registers.h"

static int parse_immediate(char *imm) {
    int out = 0;
    int idx = 0;
    int neg = 0;

    if (!imm) return 0;

    if (imm[0] == '0' && (imm[1] == 'b' || imm[1] == 'B')) {
        idx = 2;
        while (imm[idx] && imm[idx] != '\n' && imm[idx] != ' ') {
            out = (out << 1) + (imm[idx] == '1');
            idx++;
        }
        return out;
    }

    if (imm[0] == '0' && (imm[1] == 'x' || imm[1] == 'X')) {
        idx = 2;
        while (imm[idx] && imm[idx] != '\n' && imm[idx] != ' ') {
            char c = imm[idx];
            int val = (c >= '0' && c <= '9') ? c - '0' :
                      (c >= 'A' && c <= 'F') ? c - 'A' + 10 :
                      (c >= 'a' && c <= 'f') ? c - 'a' + 10 : 0;
            out = (out << 4) + val;
            idx++;
        }
        return out;
    }

    if (imm[0] == '-') {
        neg = 1;
        idx = 1;
    }
    for (; imm[idx] && imm[idx] != '\n' && imm[idx] != ' '; idx++) {
        if (imm[idx] >= '0' && imm[idx] <= '9') {
            out = out * 10 + (imm[idx] - '0');
        }
    }
    return neg ? -out : out;
}

void execute_display() {
    display_registers();
}

int execute_halt() {
    return 1;
}

void execute_clear() {
    clear_all();
}

void execute_nova(char *dest_reg) {
    char *acc = get_register("AC");
    char *dst = get_register(dest_reg);
    if (!dst) {
        return;
    }
    str_copy(dst, acc);
}

void execute_mov(char *dst_name, char *src_name) {
    char *dst = get_register(dst_name);
    char *src = get_register(src_name);
    if (!dst || !src) {
        return;
    }
    str_copy(dst, src);
}

void execute_load(char *reg_name, char *imm) {
    char *target = get_register(reg_name);
    char tmp[MAXREGSIZE];
    if (!target) {
        return;
    }
    int val = parse_immediate(imm);
    decimal_to_binary(val, tmp);
    str_copy(target, tmp);
}

void execute_add(char *ra, char *rb) {
    char *A = get_register(ra);
    char *B = get_register(rb);
    char *acc = get_register("AC");
    char sum[MAXREGSIZE];
    char of;
    if (!A || !B) {
        return;
    }
    add_binary(A, B, sum, &of);
    str_copy(acc, sum);
    set_overflow(of);
}

void execute_sub(char *ra, char *rb) {
    char *A = get_register(ra);
    char *B = get_register(rb);
    char *acc = get_register("AC");
    char diff[MAXREGSIZE];
    char of;
    if (!A || !B) {
        return;
    }
    sub_binary(A, B, diff, &of);
    str_copy(acc, diff);
    set_overflow(of);
}

void execute_and(char *ra, char *rb) {
    char *left = get_register(ra);
    char *right = get_register(rb);
    char *acc = get_register("AC");
    char res[MAXREGSIZE];
    if (!left || !right) return;
    and_binary(left, right, res);
    str_copy(acc, res);
}

void execute_or(char *a, char *b) {
    char *ra = get_register(a);
    char *rb = get_register(b);
    char *acc = get_register("AC");
    char res[MAXREGSIZE];
    if (!ra || !rb) return;
    or_binary(ra, rb, res);
    str_copy(acc, res);
}

void execute_xor(char *a, char *b) {
    char *ra = get_register(a);
    char *rb = get_register(b);
    char *acc = get_register("AC");
    char res[MAXREGSIZE];
    if (!ra || !rb) return;
    xor_binary(ra, rb, res);
    str_copy(acc, res);
}

void execute_neg(char *reg_name) {
    char *r = get_register(reg_name);
    if (!r) return;
    neg_binary(r);
}

void execute_shl(char *reg_name, char *amt) {
    char *r = get_register(reg_name);
    if (!r) return;
    int n = parse_immediate(amt);
    if (n < 0) return;
    shl_binary(r, n);
}

void execute_shr(char *reg_name, char *amt) {
    char *r = get_register(reg_name);
    if (!r) return;
    int n = parse_immediate(amt);
    if (n < 0) return;
    shr_binary(r, n);
}
