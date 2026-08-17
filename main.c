#include "emualtor.h"


int main() {
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 320, 0);

    CHIP8 myChip8;
    bool running = true;
    strcpy(myChip8.filename, "5-quirks.ch8"); //load ch8 filename

    Initialize_screen(&myChip8);
    Read_ch8_file(&myChip8);
    load_font(&myChip8);
    myChip8.pc = 0x200;
    printMem(&myChip8);

    // The main loop
    while (running) {
        
        // Read the keypad array
        if(check_keypress(&myChip8));
        else {running = false;}

        // execute one CHIP-8 instruction
        execute_command(&myChip8.mem[myChip8.pc], &myChip8);

        // Update display
        Display_screen(&myChip8);

        SDL_Delay(2); 
    }

    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}