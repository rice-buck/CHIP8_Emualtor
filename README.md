# CHIP-8 Emulator

A CHIP-8 interpreter written in C. It loads a `.ch8` ROM into emulated
memory, executes the CHIP-8 instruction set, and renders the display
using SDL2.

## Features

- Full CHIP-8 opcode support
- SDL2 window rendering (scaled 64x32 display)
- In-app ROM selection menu
- Return to the ROM selection menu at any time without restarting the program

## How It Works

CHIP-8 programs are made of 2-byte instructions stored in memory
starting at address `0x200`. The emulator runs a simple fetch-decode-execute
loop: it reads the two bytes at the program counter, combines them into a
16-bit opcode, figures out which instruction that is, and carries it out.

For example, the opcode `0x6A02` breaks down like this:

- The first nibble `6` means "set register Vx to a byte value" (`LD Vx, byte`)
- The next nibble `A` is the register index, so `x = 0xA` (register `VA`)
- The last byte `02` is the value to load

So the emulator sets `V[0xA] = 0x02`, then moves the program counter forward
2 bytes to fetch the next instruction. Opcodes like this build up game logic;
others handle things like jumps, skipping instructions on comparisons, and
drawing sprites to the display buffer, which is what `Draw_screen_SDL`
renders to the SDL2 window each frame.

## Controls

- `1`-`8`: select a ROM from the menu
- `0`-`9`, `A`-`F`: CHIP-8 hex keypad input during emulation
- `Z`: return to the ROM selection menu
- `Esc` or closing the window: quit

## Adding or Changing ROMs

The ROM menu is hardcoded, not read from the folder automatically. To add
or change a ROM:

1. Place the `.ch8` file in the project root, next to the other ROMs.
2. In `emulator.c`, find the `switch(choice)` block inside
   `check_keypress_start` and add or edit a case with the filename, e.g.
   `strcpy(self->filename, "9-mygame.ch8");`.
3. Update the printed option text in `print_menu_selection` so it matches.
4. Rebuild with `make`.

Only `.ch8` files will work. `Read_ch8_file` does no format checking, it
just reads the file's raw bytes straight into CHIP-8 memory starting at
address `0x200` and treats every byte pair as an opcode. A `.ch8` file is
nothing more than raw CHIP-8 bytecode, so it loads correctly. Any other
file type (text, images, other binaries) will still "load" without error,
but its bytes won't form valid CHIP-8 instructions, so the emulator will
crash or behave unpredictably when it tries to execute them.

## Dependencies

- A C compiler (`gcc`)
- `make`
- SDL2

Install SDL2:

- Debian/Ubuntu: `sudo apt-get install libsdl2-dev`
- Fedora: `sudo dnf install SDL2-devel`
- Arch: `sudo pacman -S sdl2`
- macOS (Homebrew): `brew install sdl2`

## Build and Run

```
git clone <this-repo-url>
cd chip8_emulator
make
make run
```

`make` builds the `chip8` binary, and `make run` builds (if needed) and
launches it. On startup, select a ROM number from the menu printed in
the terminal.
