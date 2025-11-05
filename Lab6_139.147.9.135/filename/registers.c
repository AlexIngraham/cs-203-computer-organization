#include <stdio.h>
#include "registers.h"

static char register1[MAXREGSIZE];
static char register2[MAXREGSIZE];
static char register3[MAXREGSIZE];
static char register4[MAXREGSIZE];
static char register5[MAXREGSIZE];
static char overflow_flag = 0;

void init_registers() {
    for (size_t i = 0; i < MAXREGSIZE; i++) {
        register1[i] = '0';
        register2[i] = '0';
        register3[i] = '0';
        register4[i] = '0';
        register5[i] = '0';
    }
    register1[16] = '\0';
    register2[16] = '\0';
    register3[16] = '\0';
    register4[16] = '\0';
    register5[16] = '\0';
    overflow_flag = 0;
}

char* get_register(char *reg_name) {
    if (reg_name[0] == 'R' && reg_name[1] >= '1' && reg_name[1] <= '5' && reg_name[2] == '\0') {
        switch (reg_name[1]) {
            case '1': return register1;
            case '2': return register2;
            case '3': return register3;
            case '4': return register4;
            case '5': return register5;
        }
    }
    return NULL;
}

char get_overflow() {
    return overflow_flag;
}

void set_overflow(char value) {
    overflow_flag = value;
}

void clear_all() {
    init_registers();
}

void str_copy(char *dest, const char *src, size_t n) {
    for (size_t i = 0; i < n; i++) {
        dest[i] = src[i];
    }
    dest[n] = '\0';
}

int str_to_decimal(const char *str) {
    int result = 0;
    
    if (str[0] == '1') {
        for (int i = 0; i < 16; i++) {
            result = result * 2;
            if (str[i] == '0') result += 1;
        }
        result = -(result + 1);
    } else {
        for (int i = 0; i < 16; i++) {
            result = result * 2;
            if (str[i] == '1') result += 1;
        }
    }
    return result;
}

void decimal_to_binary(int val, char *binary_str) {
    unsigned short uval;
    
    if (val < 0) {
        uval = (unsigned short)val;
    } else {
        uval = (unsigned short)val;
    }
    
    for (int i = 15; i >= 0; i--) {
        binary_str[i] = (uval & 1) ? '1' : '0';
        uval >>= 1;
    }
    binary_str[16] = '\0';
}

void hex_to_binary(const char *hex_str, char *binary_str) {
    const char *hex_digits = "0123456789ABCDEF";
    for (size_t i = 0; hex_str[i] != '\0'; i++) {
        char hex_char = hex_str[i];
        const char *pos = strchr(hex_digits, hex_char);
        if (pos) {
            int val = pos - hex_digits;
            for (int j = 3; j >= 0; j--) {
                binary_str[i * 4 + j] = (val % 2) ? '1' : '0';
                val /= 2;
            }
        }
    }
    binary_str[strlen(hex_str) * 4] = '\0';
}

void binary_to_hex(const char *binary_str, char *hex_str) {
    const char *hex_digits = "0123456789ABCDEF";
    size_t len = strlen(binary_str);
    size_t hex_index = 0;
    for (size_t i = 0; i < len; i += 4) {
        int val = 0;
        for (size_t j = 0; j < 4 && (i + j) < len; j++) {
            val = val * 2 + (binary_str[i + j] - '0');
        }
        hex_str[hex_index++] = hex_digits[val];
    }
    hex_str[hex_index] = '\0';
}

void display_registers() {
    char hex_str[5];
    int decimal_val;
    char *regs[] = {register1, register2, register3, register4, register5};
    char *names[] = {"R1", "R2", "R3", "R4", "R5"};
    
    printf("bin-------------- hex--- dec--- overflow: %c\n", overflow_flag ? '1' : '0');
    
    for (int i = 0; i < 5; i++) {
        printf("%s: ", names[i]);
        
        for (int j = 0; j < 8; j++) {
            printf("%c", regs[i][j]);
        }
        printf(" ");
        for (int j = 8; j < 16; j++) {
            printf("%c", regs[i][j]);
        }
        printf(" ");
        
        binary_to_hex(regs[i], hex_str);
        printf("%s ", hex_str);
        
        decimal_val = str_to_decimal(regs[i]);
        printf("%d\n", decimal_val);
    }
}

void add_binary(const char *bin1, const char *bin2, char *result) {
    char carry = '0';
    
    for (int i = 15; i >= 0; i--) {
        int bit1 = bin1[i] - '0';
        int bit2 = bin2[i] - '0';
        int bit_c = carry - '0';
        
        int sum = bit1 + bit2 + bit_c;
        result[i] = (sum % 2) + '0';
        carry = (sum >= 2) ? '1' : '0';
    }
    result[16] = '\0';
    
    if ((bin1[0] == bin2[0]) && (bin1[0] != result[0])) {
        set_overflow('1');
    } else {
        set_overflow('0');
    }
}

void sub_binary(const char *bin1, const char *bin2, char *result) {
    char b_neg[MAXREGSIZE];
    char carry = '1'; 
    
    for (int i = 0; i < 16; i++) {
        b_neg[i] = (bin2[i] == '0') ? '1' : '0';
    }
    b_neg[16] = '\0';
    
    for (int i = 15; i >= 0; i--) {
        if (carry == '1') {
            if (b_neg[i] == '0') {
                b_neg[i] = '1';
                carry = '0';
            } else {
                b_neg[i] = '0';
            }
        }
    }
    
    add_binary(bin1, b_neg, result);
}

void and_binary(const char *bin1, const char *bin2, char *result) {
    for (size_t i = 0; i < MAXREGSIZE - 1; i++) {
        result[i] = (bin1[i] == '1' && bin2[i] == '1') ? '1' : '0';
    }
    result[MAXREGSIZE - 1] = '\0';
}

void or_binary(const char *bin1, const char *bin2, char *result) {
    for (size_t i = 0; i < MAXREGSIZE - 1; i++) {
        result[i] = (bin1[i] == '1' || bin2[i] == '1') ? '1' : '0';
    }
    result[MAXREGSIZE - 1] = '\0';
}

void xor_binary(const char *bin1, const char *bin2, char *result) {
    for (size_t i = 0; i < MAXREGSIZE - 1; i++) {
        result[i] = (bin1[i] != bin2[i]) ? '1' : '0';
    }
    result[MAXREGSIZE - 1] = '\0';
}

void reg_binary(char *reg) {
    for (int i = 0; i < 16; i++) {
        reg[i] = (reg[i] == '0') ? '1' : '0';
    }
    
    char carry = '1';
    for (int i = 15; i >= 0; i--) {
        if (carry == '1') {
            if (reg[i] == '0') {
                reg[i] = '1';
                carry = '0';
            } else {
                reg[i] = '0';
            }
        }
    }
}

void shl_binary(char *reg, int positions) {
    for (int i = 0; i < 16 - positions; i++) {
        reg[i] = reg[i + positions];
    }
    for (int i = 16 - positions; i < 16; i++) {
        reg[i] = '0';
    }
}

void shr_binary(char *reg, int positions) {
    char sign_bit = reg[0]; 
    
    for (int i = 15; i >= positions; i--) {
        reg[i] = reg[i - positions];
    }
    for (int i = 0; i < positions; i++) {
        reg[i] = sign_bit; 
    }
}