#include "emualtor.h"
#include <cstdio>


int main(){
    CHIP8 chip8_1;

    //test .ch8 file read and screen intialization and clearing. 

    // strncpy(chip8_1.filename, "2-ibm-logo.ch8", sizeof(chip8_1.filename) - 1); 
    // chip8_1.filename[sizeof(chip8_1.filename) - 1] = '\0'; // Ensure null termination

    // if(Read_ch8_file(&chip8_1)){
    //     printf("File read successfully\n");
    // }
    // else{ printf("ERROR: file no worky");}

    // printf("Printing commands: \n");
    // printMem(&chip8_1);
    
    // printf("\n========================\n");
    // printf("Printing screen\n");

    // Initialize_screen(&chip8_1);
    // Display_screen(&chip8_1);

    // printf("Clearing screen\n");
    // execute_command(&chip8_1.mem[0x200], &chip8_1); //each 16 bytes will go up by 10 in HEX, so 0x200 for 00 E0, then 0x210 for A2 2A
    // Display_screen(&chip8_1);


    
    uint8_t cmd[] = {0x11, 0x11, 0x22, 0x22, 0x30, 
        0x00, 0x40, 0x02, 0x50, 0x00, 0x64, 0x64,
        0x74, 0x11, 0x84, 0x00};

    //test 0x1nnn
    execute_command(&cmd[0], &chip8_1);
    printf("Testing command 0x1nnn\n");
    printf("PC: %04X\n", chip8_1.pc);
    if(chip8_1.skip_next_instruction) printf("Skip next instruction!\n");
    else {printf("Don't skip next instruction\n");}
    printf("===========================================\n");

    //test 0x2nnn
    execute_command(&cmd[2], &chip8_1);
    printf("Testing command 0x2nnn\n");
    printf("Sp: %d\n", chip8_1.sp);
    printf("Top of stack: %04X\n", chip8_1.stack[chip8_1.sp]);
    printf("PC: %04X\n", chip8_1.pc);
    if(chip8_1.skip_next_instruction) printf("Skip next instruction!\n");
    else {printf("Don't skip next instruction\n");}
    printf("===========================================\n");

    //test 0x3xkk
    execute_command(&cmd[4], &chip8_1);
    printf("Testing command 0x3xkk\n");
    printf("PC: %04X\n", chip8_1.pc);
    if(chip8_1.skip_next_instruction) printf("Skip next instruction!\n");
    else {printf("Don't skip next instruction\n");}
    printf("===========================================\n");

    //test 4xkk 
    execute_command(&cmd[6], &chip8_1);
    printf("Testing command 0x4xkk\n");
    printf("PC: %04X\n", chip8_1.pc);
    if(chip8_1.skip_next_instruction) printf("Skip next instruction!\n");
    else {printf("Don't skip next instruction\n");}
    printf("===========================================\n");

    //test 5xy0
    execute_command(&cmd[8], &chip8_1);
    printf("Testing command 0x5xkk\n");
    printf("PC: %04X\n", chip8_1.pc);
    if(chip8_1.skip_next_instruction) printf("Skip next instruction!\n");
    else {printf("Don't skip next instruction\n");}
    printf("===========================================\n");

    //test 6xkk
    execute_command(&cmd[10], &chip8_1);
    printf("Testing command 0x6xkk\n");
    uint8_t xsix = cmd[10] & 0x0F;
    printf("V[%02X] = %02X\n", xsix, chip8_1.V[xsix]);
    if(chip8_1.skip_next_instruction) printf("Skip next instruction!\n");
    else {printf("Don't skip next instruction\n");}
    printf("===========================================\n");

    //test 7xkk
    execute_command(&cmd[12], &chip8_1);
    printf("Testing command 0x6xkk\n");
    uint8_t xseven = cmd[12] & 0x0F;
    printf("V[%02X] = %02X\n", xseven, chip8_1.V[xseven]);
    if(chip8_1.skip_next_instruction) printf("Skip next instruction!\n");
    else {printf("Don't skip next instruction\n");}
    printf("===========================================\n");

    //test 8xkk
    execute_command(&cmd[14], &chip8_1);
    printf("Testing command 0x6xkk\n");
    uint8_t xeight = cmd[14] & 0x0F; 
    uint8_t yeight = cmd[15] & 0xF0; 
    printf("V[%02X] = %02X\n", xeight, chip8_1.V[xeight]);
    printf("V[%02X] = %02X\n", yeight, chip8_1.V[yeight]);
    if(chip8_1.skip_next_instruction) printf("Skip next instruction!\n");
    else {printf("Don't skip next instruction\n");}
    printf("===========================================\n");


}