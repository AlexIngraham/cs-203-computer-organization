#ifndef REGISTERS_H
#define REGISTERS_H

#define MAXREGSIZE 17

void init_registers();
void display_registers();
char* get_register(char *reg_name);
char get_overflow();
void set_overflow(char val);
void clear_all();

void str_copy(char *dest, char *src);
int str_to_decimal(char *binary_str);
void decimal_to_binary(int val, char *binary_str);
void hex_to_binary(char *hex_str, char *binary_str);
void binary_to_hex(char *binary_str, char *hex_str);

void add_binary(char *a, char *b, char *result, char *overflow);
void sub_binary(char *a, char *b, char *result, char *overflow);
void and_binary(char *a, char *b, char *result);
void or_binary(char *a, char *b, char *result);
void xor_binary(char *a, char *b, char *result);
void neg_binary(char *reg);
void shl_binary(char *reg, int positions);
void shr_binary(char *reg, int positions);

#endif