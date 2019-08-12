# Intel 8080 Emulator
This is an emulator of the intel 8080, one of the first 8 bit microprocessors. At a high level, my emulator reads a sequence of instructions (a program) from a file, keeping track of its location in the file using a program counter, and executes those instructions sequentially. It simulates a full CPU, from the registers to the I/O to the memory. 

## Prerequisites
Since the emulator is written in raw C, the user only needs a C compiler to get started. While the processor is only 8 bits, there are instructions that rely on bit arithmetic using 16 bit values, and this program has only been tested on little endian CPUs. Therefore those instructions may either malfunction or fail to execute on big endian CPUs.

## Getting Started
After cloning the repo using git, open up a terminal and type "make" within your cloned directory. After making, type "./disas $filename$" to see the disassembly for any 8080 program, and type "./emu def" to run space invaders. The user can run arbitrary 8080 code by splitting the 8080 code into four different files (each of which should contain 2 kilobytes of code) and typing "./emu $file one$ $file two$ $file three$ $file four$"

## Example Output
For each instruction, the emulator runs the instruction and then outputs instruction's effects in a message that resembles the following: 

Number of steps: 64800

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; 0x360023 at current PC 0x1a5f\
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; Current Processor currState:\
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; C=1    P=1    S=1    Z=0\
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; A:0x29 B:0x00 C:0x00 D:0x1c E:0x00 H:0x29 L:0xbd SP:0x23fc\
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; MVI    M, 00\

In order, this reflects the current instruction's bytes (in hex) and the current program counter, as well as the current flag state and current register state (including the stack pointer). Finally, the message includes the disassembly of the current instruction. 

## License
This emulator is licensed under a standard open <a href = "https://opensource.org/licenses/MIT"> MIT license </a>
