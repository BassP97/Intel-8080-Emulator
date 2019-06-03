#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef struct condCodes {
    uint8_t    z:1;
    uint8_t    s:1;
    uint8_t    p:1;
    uint8_t    cy:1;
    uint8_t    ac:1;
    uint8_t    pad:3;
} condCodes;

typedef struct State8080 {
    uint8_t    a;       //a through l (as well as sp and pc) are 8 bit registers
    uint8_t    b;
    uint8_t    c;
    uint8_t    d;
    uint8_t    e;
    uint8_t    h;
    uint8_t    l;
    uint16_t   sp;      //"Special" registers that store the stack pointer
    uint16_t   pc;      //And program counter
    uint8_t    *memory; //Stores the RAM for our 8080
    struct     condCodes      cc;
    uint8_t    int_enable;
} State8080;

void unimplementedInstruction(struct State8080* state){
  printf ("\nError: Unimplemented instruction\n");
  exit(1);
}

//Checks the parity of a given value
//Hard coded to ONLY work with 8 bit values
int checkParity(uint8_t x){
  short y = x ^ (x >> 1);
  y = y ^ (y >> 2);
  y = y ^ (y >> 4);
  y = y ^ (y >> 8);
  if (y & 1)
    return 1;
  return 0;
}


int emuOp(struct State8080* currState){
  if (currState->pc == 0x0add){
    exit(1);
  }
  unsigned char* currOp = &currState->memory[currState->pc];
  printf("0x%02x%02x%02x at current PC 0x%04x\nCurrent Processor State:\n", currOp[0],currOp[1],currOp[2], currState->pc);
  printf(" C=%d    P=%d    S=%d    Z=%d\n", currState->cc.cy, currState->cc.p,
        currState->cc.s, currState->cc.z);
  printf(" A:0x%02x B:0x%02x C:0x%02x D:0x%02x E:0x%02x H:0x%02x L:0x%02x SP:0x%04x\n",
        currState->a, currState->b, currState->c, currState->d,
        currState->e, currState->h, currState->l, currState->sp);

  uint16_t nextPC;
  switch (*currOp){
    case 0x00:{
      printf("NOP");
      nextPC = (currState->pc) + 1;
      break;
    }

    case 0x01:{
      printf("LXI    B, 0x%02x %02x", currOp[2], currOp[1]);
      currState->b = currOp[2];
      currState->c = currOp[1];
			nextPC = currState->pc+3;
    }
    case 0x02: printf("STAX   B"); unimplementedInstruction(currState); break;
    case 0x03: printf("INX    B"); unimplementedInstruction(currState); break;
    case 0x04: printf("INR    B"); unimplementedInstruction(currState); break;
    case 0x05:{
      printf("DCR    B");
			currState->b = currState->b - 1;
			currState->cc.z = (currState->b == 0);
			currState->cc.s = (0x80 == (currState->b & 0x80));
      //TODO: double check that checkParity works correctly
			currState->cc.p = checkParity(currState->b);
      nextPC = currState->pc+1;
      break;
		}
    case 0x06:{
      printf("MVI    B, %02x", currOp[1]);
      currState->b = currOp[1];
      nextPC = (currState->pc) + 2;
      break;
    }
    case 0x07: printf("RLC"); unimplementedInstruction(currState); break;
    case 0x08: printf("NOP"); unimplementedInstruction(currState); break;
    case 0x09:{
      printf("DAD    B");
      uint32_t hl = ((currState->h << 8) | currState->l);
      uint32_t bc = ((currState->b << 8) | currState->c);
      uint32_t res = hl + bc;
      currState->h = (res & 0xff00) >> 8;
      currState->l = res & 0xff;
      currState->cc.cy = ((res & 0xffff0000) > 0);
      break;
    }
    case 0x0a: printf("LDAX   B"); unimplementedInstruction(currState); break;
    case 0x0b: printf("DCX    B"); unimplementedInstruction(currState); break;
    case 0x0c: printf("INR    C"); unimplementedInstruction(currState); break;
    case 0x0d: printf("DCR    C"); unimplementedInstruction(currState); break;
    case 0x0e: {
      printf("MVI    C, %02x", currOp[1]);
    }
    case 0x0f: printf("RRC"); unimplementedInstruction(currState); break;

    case 0x10: printf("NOP"); unimplementedInstruction(currState); break;
    case 0x11:{
      printf("LXI    D, 0x%02x%02x", currOp[2], currOp[1]);
      currState->d = currOp[2];
      currState->e = currOp[1];
      nextPC = (currState->pc) + 3;
      break;
    }
    case 0x12: printf("STAX   D"); unimplementedInstruction(currState); break;
    case 0x13: {
      printf("INX    D");
      currState->e++;
      //if e has overflowed, increment d
			if (currState->e == 0)
				currState->d++;
      nextPC = (currState->pc) + 1;
      break;
    }
    case 0x14: printf("INR    D"); unimplementedInstruction(currState); break;
    case 0x15: printf("DCR    D"); unimplementedInstruction(currState); break;
    case 0x16: printf("MVI    D, %02x", currOp[1]); unimplementedInstruction(currState); break;
    case 0x17: printf("RAL"); unimplementedInstruction(currState); break;
    case 0x18: printf("NOP"); unimplementedInstruction(currState); break;
    case 0x19: printf("DAD    D"); unimplementedInstruction(currState); break;
    case 0x1a:{
      printf("LDAX   D");
      currState->a = currState->memory[((short)currState->d) << 8 | currState->e];
      nextPC = (currState->pc) + 1;
      break;
    }
    case 0x1b: printf("DCX    D"); unimplementedInstruction(currState); break;
    case 0x1c: printf("INR    E"); unimplementedInstruction(currState); break;
    case 0x1d: printf("DCR    E"); unimplementedInstruction(currState); break;
    case 0x1e: printf("MVI    E, %02x", currOp[1]); unimplementedInstruction(currState); break;
    case 0x1f: printf("RAR"); unimplementedInstruction(currState); break;

    case 0x20: printf("NOP"); unimplementedInstruction(currState); break;
    case 0x21:{
      printf("LXI    H, 0x%02x%02x ", currOp[2], currOp[1]);
      currState->h = currOp[2];
      currState->l = currOp[1];
      nextPC = (currState->pc) + 3;
      break;
    }
    case 0x22: printf("SHLD   0x%02x%02x ", currOp[2], currOp[1]); unimplementedInstruction(currState); break;
    case 0x23: {
      printf("INX    H");
      currState->l++;
			if (currState->l == 0)
				currState->h++;
      nextPC = (currState->pc) + 1;
      break;
    }
    case 0x24: printf("INR    H"); unimplementedInstruction(currState); break;
    case 0x25: printf("DCR    H"); unimplementedInstruction(currState); break;
    case 0x26: printf("MVI    H, %02x", currOp[1]); unimplementedInstruction(currState); break;
    case 0x27: printf("DAA    "); unimplementedInstruction(currState); break;
    case 0x28: printf("NOP"); unimplementedInstruction(currState); break;
    case 0x29: printf("DAD    H"); unimplementedInstruction(currState); break;
    case 0x2a: printf("LHLD   %02x, %02x", currOp[2], currOp[1]); unimplementedInstruction(currState); break;
    case 0x2b: printf("DCX    H"); unimplementedInstruction(currState); break;
    case 0x2c: printf("INR    L"); unimplementedInstruction(currState); break;
    case 0x2d: printf("DCR    L"); unimplementedInstruction(currState); break;
    case 0x2e: printf("MVI    L, %02x", currOp[1]); unimplementedInstruction(currState); break;
    case 0x2f: printf("CMA"); unimplementedInstruction(currState); break;

    case 0x30: printf("NOP"); unimplementedInstruction(currState); break;
    case 0x31:{
      printf("LXI    SP, 0x%02x %02x", currState->memory[currState->pc+2], currState->memory[currState->pc+1]);
      currState->sp =((short)currOp[2] << 8 | currOp[1]);
      nextPC = (currState->pc) + 3;
      break;
    }
    case 0x32: printf("STA    0x%02x%02x", currOp[2], currOp[1]); unimplementedInstruction(currState); break;
    case 0x33: printf("INX    SP"); unimplementedInstruction(currState); break;
    case 0x34: printf("INR    M"); unimplementedInstruction(currState); break;
    case 0x35: printf("DCR    M"); unimplementedInstruction(currState); break;
    case 0x36:{
      printf("MVI    M, %02x", currOp[1]);
      currState->memory[((short)currState->h<< 8 | currState->l)] = currOp[1];;
      nextPC = (currState->pc) + 2;
      break;
    }
    case 0x37: printf("STC"); unimplementedInstruction(currState); break;
    case 0x38: printf("NOP"); unimplementedInstruction(currState); break;
    case 0x39: printf("DAD    SP"); unimplementedInstruction(currState); break;
    case 0x3a: printf("LDA    0x%02x%02x", currOp[2], currOp[1]); unimplementedInstruction(currState); break;
    case 0x3b: printf("DCX    SP"); unimplementedInstruction(currState); break;
    case 0x3c: printf("INR    A"); unimplementedInstruction(currState); break;
    case 0x3d: printf("DCR    A"); unimplementedInstruction(currState); break;
    case 0x3e: printf("MVI    A, %02x", currOp[1]); unimplementedInstruction(currState); break;
    case 0x3f: printf("CMC"); unimplementedInstruction(currState); break;

    case 0x40: printf("MOV    B,B"); unimplementedInstruction(currState); break;
    case 0x41: printf("MOV    B,C"); unimplementedInstruction(currState); break;
    case 0x42: printf("MOV    B,D"); unimplementedInstruction(currState); break;
    case 0x43: printf("MOV    B,E"); unimplementedInstruction(currState); break;
    case 0x44: printf("MOV    B,H"); unimplementedInstruction(currState); break;
    case 0x45: printf("MOV    B,L"); unimplementedInstruction(currState); break;
    case 0x46: printf("MOV    B,M"); unimplementedInstruction(currState); break;
    case 0x47: printf("MOV    B,A"); unimplementedInstruction(currState); break;
    case 0x48: printf("MOV    C,B"); unimplementedInstruction(currState); break;
    case 0x49: printf("MOV    C,C"); unimplementedInstruction(currState); break;
    case 0x4a: printf("MOV    C,D"); unimplementedInstruction(currState); break;
    case 0x4b: printf("MOV    C,E"); unimplementedInstruction(currState); break;
    case 0x4c: printf("MOV    C,H"); unimplementedInstruction(currState); break;
    case 0x4d: printf("MOV    C,L"); unimplementedInstruction(currState); break;
    case 0x4e: printf("MOV    C,M"); unimplementedInstruction(currState); break;
    case 0x4f: printf("MOV    C,A"); unimplementedInstruction(currState); break;

    case 0x50: printf("MOV    D,B"); unimplementedInstruction(currState); break;
    case 0x51: printf("MOV    D,C"); unimplementedInstruction(currState); break;
    case 0x52: printf("MOV    D,D"); unimplementedInstruction(currState); break;
    case 0x53: printf("MOV    D,E"); unimplementedInstruction(currState); break;
    case 0x54: printf("MOV    D,H"); unimplementedInstruction(currState); break;
    case 0x55: printf("MOV    D,L"); unimplementedInstruction(currState); break;
    case 0x56: printf("MOV    D,M"); unimplementedInstruction(currState); break;
    case 0x57: printf("MOV    D,A"); unimplementedInstruction(currState); break;
    case 0x58: printf("MOV    E,B"); unimplementedInstruction(currState); break;
    case 0x59: printf("MOV    E,C"); unimplementedInstruction(currState); break;
    case 0x5a: printf("MOV    E,D"); unimplementedInstruction(currState); break;
    case 0x5b: printf("MOV    E,E"); unimplementedInstruction(currState); break;
    case 0x5c: printf("MOV    E,H"); unimplementedInstruction(currState); break;
    case 0x5d: printf("MOV    E,L"); unimplementedInstruction(currState); break;
    case 0x5e: printf("MOV    E,M"); unimplementedInstruction(currState); break;
    case 0x5f: printf("MOV    E,A"); unimplementedInstruction(currState); break;

    case 0x60: printf("MOV    H,d"); unimplementedInstruction(currState); break;
    case 0x61: printf("MOV    H,C"); unimplementedInstruction(currState); break;
    case 0x62: printf("MOV    H,D"); unimplementedInstruction(currState); break;
    case 0x63: printf("MOV    H,E"); unimplementedInstruction(currState); break;
    case 0x64: printf("MOV    H,H"); unimplementedInstruction(currState); break;
    case 0x65: printf("MOV    H,L"); unimplementedInstruction(currState); break;
    case 0x66: printf("MOV    H,M"); unimplementedInstruction(currState); break;
    case 0x67: printf("MOV    H,A"); unimplementedInstruction(currState); break;
    case 0x68: printf("MOV    L,d"); unimplementedInstruction(currState); break;
    case 0x69: printf("MOV    L,C"); unimplementedInstruction(currState); break;
    case 0x6a: printf("MOV    L,D"); unimplementedInstruction(currState); break;
    case 0x6b: printf("MOV    L,E"); unimplementedInstruction(currState); break;
    case 0x6c: printf("MOV    L,H"); unimplementedInstruction(currState); break;
    case 0x6d: printf("MOV    L,L"); unimplementedInstruction(currState); break;
    case 0x6e: printf("MOV    L,M"); unimplementedInstruction(currState); break;
    case 0x6f: printf("MOV    L,A"); unimplementedInstruction(currState); break;

    case 0x70: printf("MOV    M,B"); unimplementedInstruction(currState); break;
    case 0x71: printf("MOV    M,C"); unimplementedInstruction(currState); break;
    case 0x72: printf("MOV    M,D"); unimplementedInstruction(currState); break;
    case 0x73: printf("MOV    M,E"); unimplementedInstruction(currState); break;
    case 0x74: printf("MOV    M,H"); unimplementedInstruction(currState); break;
    case 0x75: printf("MOV    M,L"); unimplementedInstruction(currState); break;
    case 0x76:{
      printf("HLT");
      unimplementedInstruction(currState);
      break;
    }
    case 0x77:{
      printf("MOV    M,A");
      //move value in register A into the memory address stored in HL
      currState->memory[(short)currState->h << 8 | currState->l] = currState->a;
      nextPC = currState->pc+1;
      break;
    }
    case 0x78: printf("MOV    A,B"); unimplementedInstruction(currState); break;
    case 0x79: printf("MOV    A,C"); unimplementedInstruction(currState); break;
    case 0x7a: printf("MOV    A,D"); unimplementedInstruction(currState); break;
    case 0x7b: printf("MOV    A,E"); unimplementedInstruction(currState); break;
    case 0x7c: {
      printf("MOV    A,H");
      currState->a = currState->h;
      nextPC = currState->pc+1;
      break;
    }
    case 0x7d: printf("MOV    A,L"); unimplementedInstruction(currState); break;
    case 0x7e: printf("MOV    A,M"); unimplementedInstruction(currState); break;
    case 0x7f: printf("MOV    A,A"); unimplementedInstruction(currState); break;

    case 0x80: printf("ADD    B"); unimplementedInstruction(currState); break;
    case 0x81: printf("ADD    C"); unimplementedInstruction(currState); break;
    case 0x82: printf("ADD    D"); unimplementedInstruction(currState); break;
    case 0x83: printf("ADD    E"); unimplementedInstruction(currState); break;
    case 0x84: printf("ADD    H"); unimplementedInstruction(currState); break;
    case 0x85: printf("ADD    L"); unimplementedInstruction(currState); break;
    case 0x86: printf("ADD    E"); unimplementedInstruction(currState); break;
    case 0x87: printf("ADD    A"); unimplementedInstruction(currState); break;
    case 0x88: printf("ADC    B"); unimplementedInstruction(currState); break;
    case 0x89: printf("ADC    C"); unimplementedInstruction(currState); break;
    case 0x8a: printf("ADC    D"); unimplementedInstruction(currState); break;
    case 0x8b: printf("ADC    E"); unimplementedInstruction(currState); break;
    case 0x8c: printf("ADC    A"); unimplementedInstruction(currState); break;
    case 0x8d: printf("ADC    L"); unimplementedInstruction(currState); break;
    case 0x8e: printf("ADC    M"); unimplementedInstruction(currState); break;
    case 0x8f: printf("ADC    A"); unimplementedInstruction(currState); break;

    case 0x90: printf("SUB    B"); unimplementedInstruction(currState); break;
    case 0x91: printf("SUB    C"); unimplementedInstruction(currState); break;
    case 0x92: printf("SUB    D"); unimplementedInstruction(currState); break;
    case 0x93: printf("SUB    E"); unimplementedInstruction(currState); break;
    case 0x94: printf("SUB    H"); unimplementedInstruction(currState); break;
    case 0x95: printf("SUB    L"); unimplementedInstruction(currState); break;
    case 0x96: printf("SUB    E"); unimplementedInstruction(currState); break;
    case 0x97: printf("SUB    A"); unimplementedInstruction(currState); break;
    case 0x98: printf("SBB    B"); unimplementedInstruction(currState); break;
    case 0x99: printf("SBB    C"); unimplementedInstruction(currState); break;
    case 0x9a: printf("SBB    D"); unimplementedInstruction(currState); break;
    case 0x9b: printf("SBB    E"); unimplementedInstruction(currState); break;
    case 0x9c: printf("SBB    A"); unimplementedInstruction(currState); break;
    case 0x9d: printf("SBB    L"); unimplementedInstruction(currState); break;
    case 0x9e: printf("SBB    M"); unimplementedInstruction(currState); break;
    case 0x9f: printf("SBB    A"); unimplementedInstruction(currState); break;

    case 0xa0: printf("ANA    B"); unimplementedInstruction(currState); break;
    case 0xa1: printf("ANA    C"); unimplementedInstruction(currState); break;
    case 0xa2: printf("ANA    D"); unimplementedInstruction(currState); break;
    case 0xa3: printf("ANA    E"); unimplementedInstruction(currState); break;
    case 0xa4: printf("ANA    H"); unimplementedInstruction(currState); break;
    case 0xa5: printf("ANA    L"); unimplementedInstruction(currState); break;
    case 0xa6: printf("ANA    E"); unimplementedInstruction(currState); break;
    case 0xa7: printf("ANA    A"); unimplementedInstruction(currState); break;
    case 0xa8: printf("XRA    B"); unimplementedInstruction(currState); break;
    case 0xa9: printf("XRA    C"); unimplementedInstruction(currState); break;
    case 0xaa: printf("XRA    D"); unimplementedInstruction(currState); break;
    case 0xab: printf("XRA    E"); unimplementedInstruction(currState); break;
    case 0xac: printf("XRA    A"); unimplementedInstruction(currState); break;
    case 0xad: printf("XRA    L"); unimplementedInstruction(currState); break;
    case 0xae: printf("XRA    M"); unimplementedInstruction(currState); break;
    case 0xaf: printf("XRA    A"); unimplementedInstruction(currState); break;

    case 0xb0: printf("ORA    B"); unimplementedInstruction(currState); break;
    case 0xb1: printf("ORA    C"); unimplementedInstruction(currState); break;
    case 0xb2: printf("ORA    D"); unimplementedInstruction(currState); break;
    case 0xb3: printf("ORA    E"); unimplementedInstruction(currState); break;
    case 0xb4: printf("ORA    H"); unimplementedInstruction(currState); break;
    case 0xb5: printf("ORA    L"); unimplementedInstruction(currState); break;
    case 0xb6: printf("ORA    E"); unimplementedInstruction(currState); break;
    case 0xb7: printf("ORA    A"); unimplementedInstruction(currState); break;
    case 0xb8: printf("CMP    B"); unimplementedInstruction(currState); break;
    case 0xb9: printf("CMP    C"); unimplementedInstruction(currState); break;
    case 0xba: printf("CMP    D"); unimplementedInstruction(currState); break;
    case 0xbb: printf("CMP    E"); unimplementedInstruction(currState); break;
    case 0xbc: printf("CMP    A"); unimplementedInstruction(currState); break;
    case 0xbd: printf("CMP    L"); unimplementedInstruction(currState); break;
    case 0xbe: printf("CMP    M"); unimplementedInstruction(currState); break;
    case 0xbf: printf("CMP    A"); unimplementedInstruction(currState); break;

    case 0xc0: printf("RNZ"); unimplementedInstruction(currState); break;
    case 0xc1: printf("POP    B"); unimplementedInstruction(currState); break;
    case 0xc2: {
      printf("JNZ    0x%02x%02x", currOp[2], currOp[1]);
      if(currState->cc.z == 0){
        nextPC = ((short)currOp[2]<<8|currOp[1]);
        break;
      }
      nextPC = currState->pc+2;
      break;
    }
    case 0xc3:{
      printf("JMP    0x%02x%02x", currState->memory[currState->pc+2], currState->memory[currState->pc+1]);
      nextPC = ((short)currOp[2] << 8 | currOp[1]);
      break;
    }
    case 0xc4: printf("CNZ    0x%02x%02x", currOp[2], currOp[1]); unimplementedInstruction(currState); break;
    case 0xc5: printf("PUSH   B"); unimplementedInstruction(currState); break;
    case 0xc6: printf("ADI    %02x", currOp[1]); unimplementedInstruction(currState); break;
    case 0xc7: printf("RST    0"); unimplementedInstruction(currState); break;
    case 0xc8: printf("RZ"); unimplementedInstruction(currState); break;
    case 0xc9: {
      printf("RET");
      nextPC = ((short)currState->memory[currState->sp]<<8|currState->memory[currState->sp+1]);
      nextPC = nextPC+3;
      currState->sp = currState->sp+2;
      break;
    }
    case 0xca: printf("JZ     0x%02x%02x", currOp[2], currOp[1]); unimplementedInstruction(currState); break;
    case 0xcb: printf("NOP"); unimplementedInstruction(currState); break;
    case 0xcc: printf("CZ     0x%02x%02x", currOp[2], currOp[1]); unimplementedInstruction(currState); break;
    case 0xcd: {
      printf("CALL   0x%02x%02x", currOp[2], currOp[1]);
      //add program counter to the stack
      currState->memory[currState->sp-1]=((char*)&currState->pc)[0];
      currState->memory[currState->sp-2]=((char*)&currState->pc)[1];
      currState->sp = currState->sp-2;

      //The above code (specifically  "((char*)&currState->pc)[0]") is somewhat unintuitive
      //As an explanation: to seperate the 16 bit program counter value into two bits, I cast it to a
      //character pointer (and thus a character array) and index into it to get the lower and upper bits
      //that are then written to the stack.

      //move to address being called
      nextPC = (((short)currState->memory[(currState->pc)+2])) << 8 | currState->memory[(currState->pc)+1];

      break;
    }
    case 0xce: printf("ACI    %02x", currOp[1]); unimplementedInstruction(currState); break;
    case 0xcf: printf("RST    1"); unimplementedInstruction(currState); break;

    case 0xd0: printf("RNC"); unimplementedInstruction(currState); break;
    case 0xd1: printf("POP    C"); unimplementedInstruction(currState); break;
    case 0xd2: printf("JNC    0x%02x%02x", currOp[2], currOp[1]); unimplementedInstruction(currState); break;
    case 0xd3: printf("OUT    %02x", currOp[1]); unimplementedInstruction(currState); break;
    case 0xd4: printf("CNC    0x%02x%02x", currOp[2], currOp[1]); unimplementedInstruction(currState); break;
    case 0xd5: printf("PUSH   D"); unimplementedInstruction(currState); break;
    case 0xd6: printf("SUI    %02x", currOp[1]); unimplementedInstruction(currState); break;
    case 0xd7: printf("RST    2"); unimplementedInstruction(currState); break;
    case 0xd8: printf("RC"); unimplementedInstruction(currState); break;
    case 0xd9: printf("NOP"); unimplementedInstruction(currState); break;
    case 0xda: printf("JC     0x%02x%02x", currOp[2], currOp[1]); unimplementedInstruction(currState); break;
    case 0xdb: printf("IN     %02x", currOp[1]); unimplementedInstruction(currState); break;
    case 0xdc: printf("CC     0x%02x%02x", currOp[2], currOp[1]); unimplementedInstruction(currState); break;
    case 0xdd: printf("NOP"); unimplementedInstruction(currState); break;
    case 0xde: printf("SBI    %02x", currOp[1]); unimplementedInstruction(currState); break;
    case 0xdf: printf("RST    3"); unimplementedInstruction(currState); break;

    case 0xe0: printf("RPO"); unimplementedInstruction(currState); break;
    case 0xe1: printf("POP    H"); unimplementedInstruction(currState); break;
    case 0xe2: printf("JPO    0x%02x%02x", currOp[2], currOp[1]); unimplementedInstruction(currState); break;
    case 0xe3: printf("XTHL    "); unimplementedInstruction(currState); break;
    case 0xe4: printf("CPO    0x%02x%02x", currOp[2], currOp[1]); unimplementedInstruction(currState); break;
    case 0xe5: printf("PUSH   H"); unimplementedInstruction(currState); break;
    case 0xe6: printf("ANI    %02x", currOp[1]); unimplementedInstruction(currState); break;
    case 0xe7: printf("RST    4"); unimplementedInstruction(currState); break;
    case 0xe8: printf("RPE     "); unimplementedInstruction(currState); break;
    case 0xe9: printf("PCHL    "); unimplementedInstruction(currState); break;
    case 0xea: printf("JPE    0x%02x%02x", currOp[2], currOp[1]); unimplementedInstruction(currState); break;
    case 0xeb: printf("XCHG    "); unimplementedInstruction(currState); break;
    case 0xec: printf("CPE    0x%02x%02x", currOp[2], currOp[1]); unimplementedInstruction(currState); break;
    case 0xed: printf("NOP"); unimplementedInstruction(currState); break;
    case 0xee: printf("XRI    %02x", currOp[1]); unimplementedInstruction(currState); break;
    case 0xef: printf("RST    5"); unimplementedInstruction(currState); break;

    case 0xf0: printf("RP"); unimplementedInstruction(currState); break;
    case 0xf1: printf("POP PSW"); unimplementedInstruction(currState); break;
    case 0xf2: printf("JP     0x%02x%02x", currOp[2], currOp[1]); unimplementedInstruction(currState); break;
    case 0xf3: printf("DI"); unimplementedInstruction(currState); break;
    case 0xf4: printf("CP     0x%02x%02x", currOp[2], currOp[1]); unimplementedInstruction(currState); break;
    case 0xf5: printf("PUSHPSW"); unimplementedInstruction(currState); break;
    case 0xf6: printf("ORI    %02x", currOp[1]); unimplementedInstruction(currState); break;
    case 0xf7: printf("RST    6"); unimplementedInstruction(currState); break;
    case 0xf8: printf("RM"); unimplementedInstruction(currState); break;
    case 0xf9: printf("SPHL"); unimplementedInstruction(currState); break;
    case 0xfa: printf("JM     0x%02x%02x", currOp[2], currOp[1]); unimplementedInstruction(currState); break;
    case 0xfb: printf("EI"); unimplementedInstruction(currState); break;
    case 0xfc: printf("CM     0x%02x%02x", currOp[2], currOp[1]); unimplementedInstruction(currState); break;
    case 0xfd: printf("NOP"); unimplementedInstruction(currState); break;
    case 0xfe: {
      printf("CPI    %02x", currOp[1]);
      uint8_t temp = currState->a-currOp[1];
      currState->cc.z = (temp == 0);
			currState->cc.s = (0x80 == (temp & 0x80));
			currState->cc.p = checkParity(temp);
			currState->cc.cy = (currState->a < currOp[1]);
      nextPC = currState->pc + 2;
      break;
    }
    case 0xff: printf("RST    7"); unimplementedInstruction(currState); break;
  }

  printf("\n\n");
  return nextPC;
}



struct State8080* Init8080(){
	struct State8080* state = calloc(1,sizeof(struct State8080));
	state->memory = malloc(0x10000);  //16K
	return state;
}

void fileToMem(struct State8080* state, char* filename, uint32_t offset){
  printf("Loading %s into memory\n", filename);
  FILE *f= fopen(filename, "rb");
	if (f==NULL){
		printf("error: Couldn't open %s\n", filename);
		exit(1);
	}

	fseek(f, 0L, SEEK_END);
	int fsize = ftell(f);
	fseek(f, 0L, SEEK_SET);

  //Read the file into program memory
	uint8_t *buffer = &state->memory[offset];
	fread(buffer, fsize, 1, f);
	fclose(f);
}


int main(int argc, char *argv[]){
  //Initialize our 8080 cpu with 16k of memory
  struct State8080* currState = Init8080();
  currState->pc = 0;
  char def[] = "default";

  //Open all four files and write them to the appropriate locations in memory
  //Arguments for space invaders should be in order: .h, .g, .f, .e

  //Each file should be 2 kilobytes
  if (strcmp(def, argv[1]) == 0){
    fileToMem(currState, "invaders.h", 0);
  	fileToMem(currState, "invaders.g", 0x800);
  	fileToMem(currState, "invaders.f", 0x1000);
    fileToMem(currState, "invaders.e", 0x1800);
  }else{
    fileToMem(currState, argv[1], 0);
  	fileToMem(currState, argv[2], 0x800);
  	fileToMem(currState, argv[3], 0x1000);
    fileToMem(currState, argv[4], 0x1800);
  }


  //Execute the program by reading and executing instruction at a time
  int pc = 0;
  int numSteps = 0;
  printf("Loaded everything into memory!\n");
  while(1){
    printf("Number of steps: %i\n",numSteps);
    currState->pc = emuOp(currState);
    numSteps++;
    //if (numSteps%100 == 0)
      //getchar();
  }

  return 0;
}
