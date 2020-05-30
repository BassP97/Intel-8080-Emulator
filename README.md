# Intel 8080 Emulator
This is an emulator of the intel 8080, one of the first 8 bit microprocessors. At a high level, my emulator reads a sequence of instructions (a program) from a file, keeping track of its location in the file using a program counter, and executes the program's instructions sequentially. It simulates a full CPU, from the registers to the I/O to the memory. 

## Legal Disclaimer
You shouldn't use this program with illegally obtained roms, for what I assume are obvious reasons. 

## Prerequisites
Since the emulator is written in raw C, the user only needs a C compiler to get started (note - though the program is compiler agnostic, the provided makefile only works with gcc). Further, while the processor is only 8 bits, there are instructions that rely on bit arithmetic using 16 bit values that consist of two concatentated 8 bit values. This program has only been tested on little endian CPUs. Therefore 16 bit instructions may either malfunction or fail to execute on big endian CPUs due to the most and least significant bits getting swapped. 

## Getting Started
After cloning the repo using git, open up a terminal and type "make" within your cloned "src" directory. After making, type "./disas $filename$" to see the disassembly for any 8080 program contained within the "data" folder, or type "./emu def" to run space invaders. Note: this emulator does NOT have a gui - it only runs 8080 code and displays the current processor state. Space invaders is not playable and is only meant as a way of demonstrating the emulator's capabilities. 

The user can run arbitrary 8080 code by splitting the 8080 code into four different files (each of which should contain 2 kilobytes of code) and typing "./emu $file one$ $file two$ $file three$ $file four$"

## Example Output
Every cycle the emulator reads the instruction at the current program counter, runs that instruction, and then outputs that instruction's effects in a message that resembles the following: 

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Number of steps: 64800\
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 0x360023 at current PC 0x1a5f\
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Current Processor state:\
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; C=1    P=1    S=1    Z=0\
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; A:0x29 B:0x00 C:0x00 D:0x1c E:0x00 H:0x29 L:0xbd SP:0x23fc\
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; MVI    M, 00\

In order, this reflects the current instruction's encoding (in hex) and the current program counter, the current flag state and the current register state (including the stack pointer). Finally, the message includes the disassembly of the current instruction. 

## License
This emulator is licensed under a standard open <a href = "https://opensource.org/licenses/MIT"> MIT license </a>
