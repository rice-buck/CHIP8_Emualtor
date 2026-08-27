#include "emualtor.h"



int main() {
    // Initialize SDL
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 320, 0);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    CHIP8 myChip8;

    printf("Welcome to the CHIP-8 Emulator!\n");
    Initialize_CHIP8(&myChip8);

    print_menu_selection();
    bool running = (file_select_menu(&myChip8) != 'q'); // false if the user quit from the selection screen
    if (running) {
        Read_ch8_file(&myChip8);
    }

    // The main loop
    while (running) {
        // Read the keypad array
        char key = check_keypress_main(&myChip8);
        if (key == 'q') {
            running = false; // Exit the main loop if 'q' is returned
        } else if (key == 'a') {
            Initialize_CHIP8(&myChip8);
            Draw_screen_SDL(&myChip8, renderer); // Clear the display
            if (file_select_menu(&myChip8) == 'q') {
                running = false; // Exit the main loop if the user quit from the selection screen
            } else {
                Read_ch8_file(&myChip8);
            }
        }
        else {running = true;}

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