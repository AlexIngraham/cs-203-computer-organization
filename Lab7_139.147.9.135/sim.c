#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "controller.h"

#define PC_START 0x0610
#define SP_START 0x0400
#define BP_START 0x0400

FILE *log_file = NULL;

void display_state(void)
{
    controller_display();
    printf("\nMemory Dump (Stack area):\n");
    unsigned short sp = controller_get_register(SP);
    if (sp >= 4)
    {
        memory_display(sp - 4, sp + 16);
    }
    else
    {
        memory_display(0, sp + 16);
    }
}

void dump_state_to_file(const char *filename)
{
    FILE *dump = fopen(filename, "w");
    if (!dump)
    {
        fprintf(stderr, "Error: Could not open %s for writing\n", filename);
        return;
    }
    
    fprintf(dump, "=== Final CPU State ===\n");
    fprintf(dump, "R0: 0x%04X\n", controller_get_register(R0));
    fprintf(dump, "R1: 0x%04X\n", controller_get_register(R1));
    fprintf(dump, "R2: 0x%04X\n", controller_get_register(R2));
    fprintf(dump, "R3: 0x%04X\n", controller_get_register(R3));
    fprintf(dump, "AC: 0x%04X\n", controller_get_register(AC));
    fprintf(dump, "SP: 0x%04X\n", controller_get_register(SP));
    fprintf(dump, "BP: 0x%04X\n", controller_get_register(BP));
    fprintf(dump, "PC: 0x%04X\n", controller_get_register(PC));
    fprintf(dump, "Halted: %s\n", controller_is_halted() ? "YES" : "NO");
    
    fclose(dump);
    printf("State dumped to %s\n", filename);
}

void process_command(const char *command)
{
    for (int i = 0; command[i]; i++)
    {
        char cmd = command[i];

        switch (cmd)
        {
        case 'd':
            display_state();
            break;

        case 'q':
            printf("Quitting\n");
            return;

        case 'Q':
            printf("Quitting and dumping state to file\n");
            dump_state_to_file("dump_log.txt");
            return;

        case 'n':
            if (!controller_is_halted())
            {
                controller_step(log_file);
            }
            else
            {
                printf("Controller is halted. Cannot step.\n");
            }
            break;
            
        case 'N':
            if (!controller_is_halted())
            {
                controller_step(log_file);
                display_state();
            }
            else
            {
                printf("Controller is halted. Cannot step.\n");
            }
            break;

        case 'H':
            printf("Running until halt\n");
            controller_run_until_halt(log_file);
            printf("Controller halted\n");
            display_state();
            break;
            
        default:
            printf("Unknown command: %c\n", cmd);
            break;
        }

        if (log_file)
        {
            fprintf(log_file, "After command '%c':\n", cmd);
            fprintf(log_file, "R0 = 0x%04X R1 = 0x%04X R2 = 0x%04X R3 = 0x%04X\n",
                    controller_get_register(R0),
                    controller_get_register(R1),
                    controller_get_register(R2),
                    controller_get_register(R3));
            fprintf(log_file, "AC = 0x%04X SP = 0x%04X BP = 0x%04X PC = 0x%04X\n",
                    controller_get_register(AC),
                    controller_get_register(SP),
                    controller_get_register(BP),
                    controller_get_register(PC));
            fprintf(log_file, "\n");
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <binary_file>\n", argv[0]);
        return 1;
    }

    log_file = fopen("log_file.txt", "w");
    if (!log_file)
    {
        fprintf(stderr, "Error: Could not open log_file.txt\n");
        return 1;
    }

    fprintf(log_file, "=== SSAMv3.1 VM Simulator Log ===\n");
    fprintf(log_file, "Binary file: %s\n\n", argv[1]);
    
    memory_init();

    FILE *fh = fopen(argv[1], "rb");
    if (!fh)
    {
        fprintf(stderr, "Error: Could not open file %s\n", argv[1]);
        fprintf(log_file, "Error: Could not open file %s\n", argv[1]);
        fclose(log_file);
        return 1;
    }

    int bytes_loaded = memory_load_file(fh);
    fclose(fh);

    printf("Loaded %d bytes from %s\n", bytes_loaded, argv[1]);
    fprintf(log_file, "Loaded %d bytes\n\n", bytes_loaded);

    controller_init(PC_START, SP_START, BP_START);
    
    printf("SSAMv3.1 Virtual Machine\n");
    display_state();
    printf("\nCommands: q=quit, Q=quit+dump, d=display, n=step, N=step+display, H=run\n");
    printf("Enter commands (or type them as a string, e.g., 'nnndq'): ");

    char input[256];
    while (fgets(input, sizeof(input), stdin))
    {
        input[strcspn(input, "\n")] = 0;

        if (strlen(input) == 0)
        {
            continue;
        }

        process_command(input);

                if (strchr(input, 'q') || strchr(input, 'Q'))
                {
                    break;
                }
            }
        
            fclose(log_file);
            return 0;
        }