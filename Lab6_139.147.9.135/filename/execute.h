#ifndef EXECUTE_H
#define EXECUTE_H

void execute_display();
int execute_halt();
void execute_clear();
void execute_nova(char *reg_name);
void execute_mov(char *reg_a, char *reg_b);
void execute_load(char *reg_name, char *immediate_val);
void execute_add(char *reg_a, char *reg_b);
void execute_sub(char *reg_a, char *reg_b);
void execute_and(char *reg_a, char *reg_b);
void execute_or(char *reg_a, char *reg_b);
void execute_xor(char *reg_a, char *reg_b);
void execute_neg(char *reg_name);
void execute_shl(char *reg_name, char *shift_amount);
void execute_shr(char *reg_name, char *shift_amount);

#endif