#include "controller.h"
#include "memory.h"
#include <stdio.h>

static unsigned short registers[REGCNT];
static int halted = 0;

void controller_init(unsigned short pc_start, unsigned short sp_start, unsigned short bp_start)
{
    for (int i = 0; i < REGCNT; i++)
    {
        registers[i] = 0;
    }
    registers[PC] = pc_start;
    registers[SP] = sp_start;
    registers[BP] = bp_start;
    halted = 0;
}

int controller_is_halted(void)
{
    return halted;
}

unsigned short controller_get_register(regnames reg)
{
    if (reg >= 0 && reg < REGCNT)
    {
        return registers[reg];
    }
    return 0;
}

static void execute_instruction(unsigned short instruction, FILE *log_file)
{
    unsigned short group = (instruction >> 14) & 0x03;

    switch (group)
    {
    case 0:
    {
        unsigned short opcode = (instruction >> 11) & 0x07;
        switch (opcode)
        {
        case 0: // HALT

            halted = 1;
            if (log_file)
            {
                fprintf(log_file, "HALT executed. Controller halted.\n");
            }
            break;
        case 1:
            // NOP
            if (log_file)
            {
                fprintf(log_file, "NOP executed.\n");
            }
            break;
        case 2:
        {
            registers[SP] = registers[BP];
            registers[BP] = memory_get_word(registers[SP]);
            registers[SP] -= 2;
            registers[PC] = memory_get_word(registers[SP]);
            fprintf(log_file, "RET\n");
            break;
        }
        default:
            if (log_file)
            {
                fprintf(log_file, "Unknown opcode: %u\n", opcode);
            }
            break;
        }
        break;
    }
    case 1:
    {
        unsigned short mod = (instruction >> 12) & 0x01;
        unsigned short tt = (instruction >> 10) & 0x03;
        unsigned short rA = (instruction >> 8) & 0x07;

        if (mod == 0)
        {
            switch (tt)
            {
            case 0:
            {
                unsigned char value = instruction & 0xFF;
                registers[rA] = (unsigned short)value;
                fprintf(log_file, "LODI R%d, %d\n", rA, (signed char)value);
                break;
            }
            case 1:
            {
                unsigned char addr = instruction & 0xFF;
                registers[rA] = memory_get_word(addr);
                fprintf(log_file, "LOAD R%d, (0x%02x)\n", rA, addr);
                break;
            }
            case 2:
            {
                unsigned short rB = (instruction >> 5) & 0x07;
                registers[rA] = memory_get_word(registers[rB]);
                fprintf(log_file, "LOAD R%d, (R%d)\n", rA, rB);
                break;
            }
            case 3:
            {
                unsigned short rB = (instruction >> 5) & 0x07;
                unsigned char x = instruction & 0x1F;
                signed char offset = (x & 0x10) ? (x | 0xE0) : (signed char)x;
                registers[rA] = memory_get_word(registers[rB] + offset * 2);
                fprintf(log_file, "LOAD R%d, (R%d + %d)\n", rA, rB, offset);
                break;
            }
            }
        }
        else
        {
            switch (tt)
            {
            case 0:
            {
                unsigned char value = instruction & 0xFF;
                memory_set_word(value, registers[rA]);
                fprintf(log_file, "STOA (0x%02X), R%d\n", value, rA);
                break;
            }
            case 1:
            {
                unsigned char x = (instruction >> 5) & 0x07;
                memory_set_word(registers[x], registers[rA]);
                fprintf(log_file, "STOR (R%d), R%d\n", x, rA);
                break;
            }
            case 2:
            {
                unsigned short rB = (instruction >> 5) & 0x07;
                unsigned char x = instruction & 0x1F;
                signed char offset = (x & 0x10) ? (x | 0xE0) : (signed char)x;
                memory_set_word(registers[rB] + offset * 2, registers[rA]);
                fprintf(log_file, "STOR (R%d + %d), R%d\n", rB, offset, rA);
                break;
            }
            }
        }
        break;
    }
    case 2:
    {
        unsigned short mtype = (instruction >> 8) & 0x7;
        unsigned short rA = (instruction >> 8) & 0x7;

        switch (mtype)
        {
        case 0:
        {
            registers[AC] = -registers[rA];
            fprintf(log_file, "Negative R%d\n", rA);
            break;
        }
        case 1:
        {
            unsigned short resultrB = (instruction >> 5) & 0x7;
            registers[AC] = registers[rA] + registers[resultrB];
            fprintf(log_file, "ADD R%d, R%d\n", rA, resultrB);
            break;
        }
        case 2:
        {
            unsigned short result = instruction & 0xFF;
            registers[AC] = registers[rA] + result;
            fprintf(log_file, "ADDI R%d, %d\n", rA, result);
            break;
        }
        case 3:
        {
            unsigned short resultrB = (instruction >> 5) & 0x7;
            registers[AC] = registers[rA] - registers[resultrB];
            fprintf(log_file, "SUB R%d, R%d\n", rA, resultrB);
            break;
        }
        case 4:
        {
            unsigned char result = instruction & 0xFF;
            registers[AC] = registers[rA] - result;
            fprintf(log_file, "SUBI R%d, %d\n", rA, result);
            break;
        }
        case 7:
        {
            unsigned short resultrB = (instruction >> 5) & 0x7;
            registers[rA] = registers[resultrB];
            break;
        }
        }
        break;
    }
    case 3:
    {
        unsigned short jtype = (instruction >> 12) & 0x3;
        unsigned short address = instruction & 0xFFF;
        switch (jtype)
        {
        case 0:
        {
            registers[PC] = address;
            if (log_file)
            {
                fprintf(log_file, "JMP 0x%03X\n", address);
            }
            break;
        }
        case 1:
        {
            if (registers[AC] == 0)
            {
                registers[PC] = address;
            }
            if (log_file)
            {
                fprintf(log_file, "JMPZ 0x%03X\n", address);
            }
            break;
        }
        case 2:
        {
            if ((signed short)registers[AC] < 0)
            {
                registers[PC] = address;
            }
            if (log_file)
            {
                fprintf(log_file, "JMPN 0x%03X\n", address);
            }
            break;
        }
        case 3:
        {
            memory_set_word(registers[SP], registers[PC]);
            registers[SP] += 2;
            registers[PC] = address;
            memory_set_word(registers[SP], registers[BP]);
            registers[BP] = registers[SP];
            registers[SP] += 2;
            if (log_file)
            {
                fprintf(log_file, "CALL 0x%03X\n", address);
            }
            break;
        }
        }
        break;
    }
    }
}

int controller_step(FILE *log_file)
{
    if (halted)
    {
        fprintf(log_file, "Controller is halted. Cannot step.\n");
        return -1;
    }

    unsigned short instruction = memory_get_word(registers[PC]);
    fprintf(log_file, "PC = 0x%04X: IR = 0x%04X ", registers[PC], instruction);
    registers[PC] += 2;
    
    execute_instruction(instruction, log_file);
    return 1;
}

void controller_run_until_halt(FILE *log_file)
{
    while (!halted)
    {
        controller_step(log_file);
    }
}

void controller_display(void)
{
    printf("R0 = 0x%04X R1 = 0x%04X R2 = 0x%04X R3 = 0x%04X\n",
           registers[R0], registers[R1], registers[R2], registers[R3]);
    printf("AC = 0x%04X SP = 0x%04X BP = 0x%04X PC = 0x%04X\n",
           registers[AC], registers[SP], registers[BP], registers[PC]);
    printf("Halted: %s\n", halted ? "Yes" : "No");
}