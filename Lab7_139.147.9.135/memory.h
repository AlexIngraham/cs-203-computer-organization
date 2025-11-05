#ifndef MEMORY_H
#define MEMORY_H

#include <stdio.h>

#define MEMORY_SIZE 0x10000 // 64KB

void memory_init(void);

int memory_load_file(FILE *file);

unsigned short memory_set_word(unsigned short address, unsigned short value);
unsigned short memory_get_word(unsigned short address);
unsigned char memory_set_byte(unsigned short address, unsigned char value);
unsigned char memory_get_byte(unsigned short address);

void memory_display(unsigned short start_address, unsigned short end_address);

#endif