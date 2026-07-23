//chip 8 emulator
#include "emualtor.h"
#include <stdint.h>
#include <stdio.h>



void Initialize_screen(CHIP8 *self){
    for(int i = 0; i < (WIDTH * HEIGHT); ++i){
        self->display[i] = ' ';
    }
}

void Display_screen(CHIP8 *self){
    int w;
    int h;
    for(int y = 0; y < HEIGHT; ++y){
        printf("\n"); //new line for each row completed
        h += 1;
        w %= 32;

        for(int x = 0; x < WIDTH; ++x){
            printf("%d", self->display[x + (y * WIDTH)]);
            w += 1;
        }
    }
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
void execute_command(uint8_t cmd[2]){
    switch(cmd[0]){
        case 0x00:
            printf("First byte is 00\n");

            switch(cmd[1]){
                case 0xE0:
                    printf("Second byte is E0\n");
                    break;
            }
            break; 
        //case
    }
}



int main(){
    CHIP8 chip8_1;

    strncpy(chip8_1.filename, "2-ibm-logo.ch8", sizeof(chip8_1.filename) - 1); 
    chip8_1.filename[sizeof(chip8_1.filename) - 1] = '\0'; // Ensure null termination

    if(Read_ch8_file(&chip8_1)){
        printf("File read successfully\n");
    }
    else{ printf("ERROR: file no worky");}

    printMem(&chip8_1);

    execute_command(&chip8_1.mem[0x200]); //each 16 bytes will go up by 10 in HEX, so 0x200 for 00 E0, then 0x210 for A2 2A
}