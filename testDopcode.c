#include "emualtor.h"

int main(){

CHIP8 chip8_1;

Initialize_screen(&chip8_1);
Display_screen(&chip8_1);

uint8_t cmd2[] = {0xD6, 0x78};
chip8_1.V[6] = 50;
chip8_1.V[7] = 15;

chip8_1.I = 0x200;

    chip8_1.mem[0x200] = 0xFF;
    for(int i = 1; i < 6; ++i){
        chip8_1.mem[0x200 + i] = 0x18;
    }

    execute_command(&cmd2[0], &chip8_1);

    Display_screen(&chip8_1);
}