#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <SDL2/SDL.h>

#define WIDTH 64
#define HEIGHT 32
#define FONT_MEM_BASE 0x50


typedef struct{

    uint8_t mem[4096];
    uint8_t V[16]; //general purpose registers
    uint16_t I; //store memory address
    uint8_t delay_timer; //Timers register
    uint8_t sound_timer; //sound register
    uint16_t pc; //program counter, tracks current address of the command 
    uint8_t sp; //stack pointer
    uint16_t stack[16]; //stores address that the interpereter should return to when finished with a subroutine
    uint8_t display[WIDTH * HEIGHT]; //use the formula x +(y * width) to find pixel
    char filename[256]; //store the filename of the rom
    long file_size; //store the size of the rom
    char keyboard[16]; //store the state of the keyboard, 1 if pressed, 0 if not pressed

} CHIP8;

void Initialize_screen(CHIP8 *self);

void Display_screen(CHIP8 *self);

bool Read_ch8_file(CHIP8 *self);

void load_font(CHIP8 *self);

bool check_keypress(CHIP8 *self);

void execute_command(uint8_t cmd[2], CHIP8 *self);

void printMem(CHIP8 *self);