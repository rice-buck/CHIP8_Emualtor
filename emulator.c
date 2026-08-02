//chip 8 emulator
#include "emualtor.h"
#include <stdint.h>
#include <stdio.h>



void Initialize_screen(CHIP8 *self){
    for(int i = 0; i < (WIDTH * HEIGHT); ++i){
        self->display[i] = '#';
    }
}

void Display_screen(CHIP8 *self){
    int w = 0;
    int h = 0;

    for(int y = 0; y < HEIGHT; ++y){
        printf("\n"); //new line for each row completed
        h += 1;
        w %= 32;

        for(int x = 0; x < WIDTH; ++x){
            printf("%c", self->display[x + (y * WIDTH)]);
            w += 1;
        }
    }
    printf("\nHeight: %d  Width: %d \n", h, w);
}

bool Read_ch8_file(CHIP8 *self){
    FILE *file_ptr = fopen(self->filename, "rb"); //rb for read binary

    if (file_ptr == NULL) {
        printf("Error: Could not open file.\n");
        return false;
    }

    //get file size
    fseek(file_ptr, 0, SEEK_END); //moves cursor to EOF
    long file_size = ftell(file_ptr); //returns cursor's position as a byte count, since cursor is at end this equals file size
    rewind(file_ptr); //returns cursor back to byte 0

    //ptr starting at 0x200 or 512 in memory
    uint8_t *dest_ptr = &self->mem[512];

    size_t bytes_read = fread(dest_ptr, 1, file_size, file_ptr);

    if (bytes_read != file_size) {
        printf("Error: Unexpected error while reading ROM.\n");
        fclose(file_ptr);
        return false;
    }

    printf("Bytes read %zu\n", bytes_read);
    fclose(file_ptr);
    return true;
}

void printMem(CHIP8 *self){
    FILE *file_ptr = fopen(self->filename, "rb");
    fseek(file_ptr, 0, SEEK_END); //moves cursor to EOF
    long file_size = ftell(file_ptr); //returns cursor's position as a byte count, since cursor is at end this equals file size

    size_t start_addr = 0x200;
    size_t end_addr = start_addr + file_size;

   // Loop through memory in rows of 16 bytes
    for (size_t i = start_addr; i < end_addr; i += 16) {
        
        // Print the current row's starting memory address
        printf("\n0x%04X: ", (unsigned int)i);

        // Print 16 bytes in hex format
        for (size_t j = 0; j < 16; j++) {
            if (i + j < end_addr) {
                printf("%02X ", self->mem[i + j]);
            }
            else {
                printf("   "); // Fill space if we pass the end_addr
            }
        }
    }
    printf("\n");
}

//reads command and executes, does not dictate WHICH command is being read
void execute_command(uint8_t cmd[2], CHIP8 *self){

    self->skip_next_instruction = false;

    // Shift the first byte left by 8 bits, then bitwise-OR with the second byte
    uint16_t opcode = (cmd[0] << 8) | cmd[1];

    printf("Command: %04X\n", opcode);

    switch (opcode & 0xF000) { // Look at the first nibble (4 bits)
        case 0x0000:
            switch (opcode) {
                case 0x00E0: // Clear display
                    for (int i = 0; i < (WIDTH * HEIGHT); ++i) {
                        self->display[i] = ' ';
                    }
                    break;

                case 0x00EE: // Return from subroutine
                    self->pc = self->stack[self->sp];
                    --self->sp;
                    break;
            }
            break;


        case 0x1000: // Jump to address NNN (1NNN)
            //The interpreter sets the program counter to nnn.
            self->pc = opcode & 0x0FFF; // Extract last 12 bits
            break;
            
        case 0x2000://The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
            self->sp += 1;
            self->stack[self->sp] = self->pc;
            self->pc = opcode & 0x0FFF; // Extract nnn
            break;

        //3xkk - SE Vx, byte
        // Skip next instruction if Vx = kk.   
        // The interpreter compares register Vx to kk, and if they are equal, increments the program counter by 2.
        case 0x3000: {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t kk = opcode & 0x00FF;

            if (self->V[x] == kk) {
                self->skip_next_instruction  = true;
                self->pc += 2;
            }
            break;
        }

        // 4xkk - SNE Vx, byte
        // Skip next instruction if Vx != kk.
        // The interpreter compares register Vx to kk, and if they are not equal, increments the
        // program counter by 2.
        case 0x4000: {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t kk = opcode & 0x00FF;
            
            if (self->V[x] != kk) {
                self->skip_next_instruction  = true;
                self->pc += 2;
            }
            break;
        }

        // 5xy0 - SE Vx, Vy
        // Skip next instruction if Vx = Vy.
        // The interpreter compares register Vx to register Vy, and if they are equal, increments
        // the program counter by 2.
        case 0x5000: {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t y = (opcode & 0x00F0) >> 4;

            if(self->V[x] == self->V[y]) {
                self->skip_next_instruction = true; 
                self->pc += 2;
            }
            break;
        }

        // 6xkk - LD Vx, byte
        // Set Vx = kk.
        // The interpreter puts the value kk into register Vx.
        case 0x6000: {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t kk = opcode & 0x00FF;
            self->V[x] = kk;
            break;
        }


        // 7xkk - ADD Vx, byte
        // Set Vx = Vx + kk.
        // Adds the value kk to the value of register Vx, then stores the result in Vx.
        case 0x7000: {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t kk = opcode & 0x00FF;
            self->V[x] = self->V[x] + kk;
            break;
        }

        // 8xy0 - LD Vx, Vy
        // Set Vx = Vy.
        // Stores the value of register Vy in register Vx.
        case 0x8000: {
            uint8_t x = (opcode & 0x0F00) >> 8;
            uint8_t y = (opcode & 0x00F0) >> 4;
            self->V[x] = self->V[y];
            break; 
        }

        // 8xy1 - OR Vx, Vy
        // Set Vx = Vx OR Vy.
        // Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx. A
        // bitwise OR compares the corrseponding bits from two values, and if either bit is 1,
        // then the same bit in the result is also 1. Otherwise, it is 0.

        // 8xy2 - AND Vx, Vy
        // Set Vx = Vx AND Vy.
        // Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx. A
        // bitwise AND compares the corrseponding bits from two values, and if both bits are 1,
        // then the same bit in the result is also 1. Otherwise, it is 0.

        // 8xy3 - XOR Vx, Vy
        // Set Vx = Vx XOR Vy.
        // Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in
        // Vx. An exclusive OR compares the corrseponding bits from two values, and if the bits
        // are not both the same, then the corresponding bit in the result is set to 1.
        // Otherwise, it is 0.

        // 8xy4 - ADD Vx, Vy
        // Set Vx = Vx + Vy, set VF = carry.
        // The values of Vx and Vy are added together. If the result is greater than 8 bits
        // (i.e., > 255,) VF is set to 1, otherwise 0. Only the lowest 8 bits of the result are
        // kept, and stored in Vx.

        // 8xy5 - SUB Vx, Vy
        // Set Vx = Vx - Vy, set VF = NOT borrow.
        // If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the
        // results stored in Vx.

        // 8xy6 - SHR Vx {, Vy}
        // Set Vx = Vx SHR 1.
        // If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0. Then Vx is
        // divided by 2.

        // 8xy7 - SUBN Vx, Vy
        // Set Vx = Vy - Vx, set VF = NOT borrow.
        // If Vy > Vx, then VF is set to 1, otherwise 0. Then Vx is subtracted from Vy, and the
        // results stored in Vx.

        // 8xyE - SHL Vx {, Vy}
        // Set Vx = Vx SHL 1.
        // If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0. Then Vx
        // is multiplied by 2.

        // 9xy0 - SNE Vx, Vy
        // Skip next instruction if Vx != Vy.
        // The values of Vx and Vy are compared, and if they are not equal, the program counter
        // is increased by 2.

        // Annn - LD I, addr
        // Set I = nnn.
        // The value of register I is set to nnn.

        // Bnnn - JP V0, addr
        // Jump to location nnn + V0.
        // The program counter is set to nnn plus the value of V0.

        // Cxkk - RND Vx, byte
        // Set Vx = random byte AND kk.
        // The interpreter generates a random number from 0 to 255, which is then ANDed with the
        // value kk. The results are stored in Vx. See instruction 8xy2 for more information on
        // AND.

        // Dxyn - DRW Vx, Vy, nibble
        // Display n-byte sprite starting at memory location I at (Vx, Vy), set VF = collision.
        // The interpreter reads n bytes from memory, starting at the address stored in I. These
        // bytes are then displayed as sprites on screen at coordinates (Vx, Vy). Sprites are
        // XORed onto the existing screen. If this causes any pixels to be erased, VF is set to
        // 1, otherwise it is set to 0. If the sprite is positioned so part of it is outside the
        // coordinates of the display, it wraps around to the opposite side of the screen. See
        // instruction 8xy3 for more information on XOR, and section 2.4, Display, for more
        // information on the Chip-8 screen and sprites.
        // Ex9E - SKP Vx
        // Skip next instruction if key with the value of Vx is pressed.
        // Checks the keyboard, and if the key corresponding to the value of Vx is currently in
        // the down position, PC is increased by 2.
        // ExA1 - SKNP Vx
        // Skip next instruction if key with the value of Vx is not pressed.
        // Checks the keyboard, and if the key corresponding to the value of Vx is currently in
        // the up position, PC is increased by 2.
        // Fx07 - LD Vx, DT
        // Set Vx = delay timer value.
        // The value of DT is placed into Vx.
        // Fx0A - LD Vx, K
        // Wait for a key press, store the value of the key in Vx.
        // All execution stops until a key is pressed, then the value of that key is stored in
        // Vx.
        // Fx15 - LD DT, Vx
        // Set delay timer = Vx.
        // DT is set equal to the value of Vx.
        // Fx18 - LD ST, Vx
        // Set sound timer = Vx.
        // ST is set equal to the value of Vx.
        // Fx1E - ADD I, Vx
        // Set I = I + Vx.
        // The values of I and Vx are added, and the results are stored in I.
        // Fx29 - LD F, Vx
        // Set I = location of sprite for digit Vx.
        // The value of I is set to the location for the hexadecimal sprite corresponding to the
        // value of Vx. See section 2.4, Display, for more information on the Chip-8 hexadecimal
        // font.
        // Fx33 - LD B, Vx
        // Store BCD representation of Vx in memory locations I, I+1, and I+2.
        // The interpreter takes the decimal value of Vx, and places the hundreds digit in memory
        // at location in I, the tens digit at location I+1, and the ones digit at location I+2.
        // Fx55 - LD [I], Vx
        // Store registers V0 through Vx in memory starting at location I.
        // The interpreter copies the values of registers V0 through Vx into memory, starting at
        // the address in I.
        // Fx65 - LD Vx, [I]
        // Read registers V0 through Vx from memory starting at location I.
        // The interpreter reads values from memory starting at location I into registers V0
        // through Vx.


        // ... rest of CHIP-8 opcodes
    }
}
