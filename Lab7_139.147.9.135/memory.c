#include <stdio.h>
#include "memory.h"
#include <string.h>

static unsigned char memory[MEMORY_SIZE];

void memory_init(void)
{
    memset(memory, 0, sizeof(memory));
}

int memory_load_file(FILE *file)
{
    if (!file)
    {
        printf("Error: File pointer is NULL\n");
        return -1;
    }

    unsigned char number[2];
    fread(number, 1, 2, file);
    int data_size = 2;

    while (!feof(file))
    {
        size_t bytes_read = fread(&memory[data_size], 1, 1, file);
        if (bytes_read == 1)
        {
            data_size += 1;
        }
        else
        {
            break;
        }
    }

    return data_size;
}

unsigned short memory_set_word(unsigned short address, unsigned short value)
{
    memory[address] = value & 0xFF;
    memory[address + 1] = (value >> 8) & 0xFF;
}

unsigned short memory_get_word(unsigned short address)
{
    unsigned short value = 0;
    value = memory[address];
    value |= (memory[address + 1] << 8);
    return value;
}

unsigned char memory_set_byte(unsigned short address, unsigned char value)
{
    memory[address] = value;
    return value;
}

unsigned char memory_get_byte(unsigned short address)
{
    return memory[address];
}

void memory_display(unsigned short start_address, unsigned short end_address)
{
    for (unsigned short addr = start_address; addr <= end_address; addr += 16)
    {
        printf("%04X: ", addr);
        for (unsigned short offset = 0; offset < 16 && (addr + offset) <= end_address; offset++)
        {
            printf("0x%04X: 0x%04X\n", addr + offset, memory_get_word(addr + offset));
        }
        printf("\n");
    }
}