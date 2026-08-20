#include "emualtor.h"



int main() {
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 320, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    CHIP8 myChip8;



    Initialize_CHIP8(&myChip8);
    load_font(&myChip8);

    bool runningStart = true;
    while(runningStart) {
        if(check_keypress_start(&myChip8));
        else {runningStart = false;}
    }

    Read_ch8_file(&myChip8);
    bool runningMain = true;

    // The main loop
    while (runningMain) {
        
        // Read the keypad array
        if(check_keypress_main(&myChip8));
        else {runningMain = false;}

        // execute one CHIP-8 instruction
        execute_command(&myChip8.mem[myChip8.pc], &myChip8);

        // Update display
        Draw_screen_SDL(&myChip8, renderer);

        SDL_Delay(2);  
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}