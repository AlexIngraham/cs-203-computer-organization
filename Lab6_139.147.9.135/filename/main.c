#include <stdio.h>
#include "fetch_decode.h"
#include "registers.h"

int main() {
    int halt = 0;
    init_registers();
    while (!halt) {
        halt = fetch_decode_execute();
    }
    
    return 0;
}