emu: 8080Emu.c
	gcc 8080Emu.c -o emu

disas: 8080Disassembler.c
	gcc 8080Disassembler.c -o disas
