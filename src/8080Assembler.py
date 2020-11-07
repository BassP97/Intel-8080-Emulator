import sys
import os

instructionDict = {
    "NOP": 0x0,
    "LXI B": 0x1,
    "STAX B": 0x2,
    "INX B": 0x3,
    "INR B": 0x4,
    "DCR B": 0x5,
    "MVI B": 0x6,
    "RLC": 0x7,
    "DAD B": 0x09,
    "LDAX B": 0x0A,
    "DCX B": 0x0B,
    "INR C": 0x0C,
    "DCR C": 0x0D,
    "MVI C": 0x0E,
    "RRC": 0x0F,
    "LXI D": 0x11,
    "STAX D": 0x12,
    "INX D": 0x13,
    "INR D": 0x14,
    "DCR D": 0x15,
    "MVI D": 0x16,
    "RAL": 0x17,
    "DAD D": 0x19,
    "LDAX D": 0x1A,
    "DCX D": 0x1B,
    "INR E": 0x1C,
    "DCR E": 0x1D,
    "MVI E": 0x1E,
    "RAR": 0x1F,
    "LXI H": 0x21,
    "SHLD": 0x22,
    "INX H": 0x23,
    "INR H": 0x24,
    "DCR H": 0x25,
    "MVI H": 0x26,
    "DAA": 0x27,
    "DAD H": 0x29,
    "LHLD": 0x2A,
    "DCX H": 0x2B,
    "INR L": 0x2C,
    "DCR L": 0x2D,
    "MVI L": 0x2E,
    "CMA": 0x2F,
    "LXI SP": 0x31,
    "STA": 0x32,
    "INX SP": 0x33,
    "INR M": 0x34,
    "DCR M": 0x35,
    "MVI M": 0x36,
    "STC": 0x37,
    "DAD SP": 0x39,
    "LDA": 0x3A,
    "DCX SP": 0x3B,
    "INR A": 0x3C,
    "DCR A": 0x3D,
    "MVI A": 0x3E,
    "CMC": 0x3F,
    "MOV B,B": 0x40,
    "MOV B,C": 0x41,
    "MOV B,D": 0x42,
    "MOV B,E": 0x43,
    "MOV B,H": 0x44,
    "MOV B,L": 0x45,
    "MOV B,M": 0x46,
    "MOV B,A": 0x47,
    "MOV C,B": 0x48,
    "MOV C,C": 0x49,
    "MOV C,D": 0x4A,
    "MOV C,E": 0x4B,
    "MOV C,H": 0x4C,
    "MOV C,L": 0x4D,
    "MOV C,M": 0x4E,
    "MOV C,A": 0x4F,
    "MOV D,B": 0x50,
    "MOV D,C": 0x51,
    "MOV D,D": 0x52,
    "MOV D,E": 0x53,
    "MOV D,H": 0x54,
    "MOV D,L": 0x55,
    "MOV D,M": 0x56,
    "MOV D,A": 0x57,
    "MOV E,B": 0x58,
    "MOV E,C": 0x59,
    "MOV E,D": 0x5A,
    "MOV E,E": 0x5B,
    "MOV E,H": 0x5C,
    "MOV E,L": 0x5D,
    "MOV E,M": 0x5E,
    "MOV E,A": 0x5F,
    "MOV H,B": 0x60,
    "MOV H,C": 0x61,
    "MOV H,D": 0x62,
    "MOV H,E": 0x63,
    "MOV H,H": 0x64,
    "MOV H,L": 0x65,
    "MOV H,M": 0x66,
    "MOV H,A": 0x67,
    "MOV L,B": 0x68,
    "MOV L,C": 0x69,
    "MOV L,D": 0x6A,
    "MOV L,E": 0x6B,
    "MOV L,H": 0x6C,
    "MOV L,L": 0x6D,
    "MOV L,M": 0x6E,
    "MOV L,A": 0x6F,
    "MOV M,B": 0X70,
    "MOV M,C": 0X71,
    "MOV M,D": 0X72,
    "MOV M,E": 0X73,
    "MOV M,H": 0X74,
    "MOV M,L": 0X75,
    "HLT": 0X76,
    "MOV M,A": 0X77,
    "MOV A,B": 0X78,
    "MOV A,C": 0X79,
    "MOV A,D": 0X7A,
    "MOV A,E": 0X7B,
    "MOV A,H": 0X7C,
    "MOV A,L": 0X7D,
    "MOV A,M": 0X7E,
    "MOV A,A": 0X7F,
    "ADD B": 0X80,
    "ADD C": 0X81,
    "ADD D": 0X82,
    "ADD E": 0X83,
    "ADD H": 0X84,
    "ADD L": 0X85,
    "ADD M": 0X86,
    "ADD A": 0X87,
    "ADC B": 0X88,
    "ADC C": 0X89,
    "ADC D": 0X8A,
    "ADC E": 0X8B,
    "ADC H": 0X8C,
    "ADC L": 0X8D,
    "ADC M": 0X8E,
    "ADC A": 0X8F,
    "SUB B": 0X90,
    "SUB C": 0X91,
    "SUB D": 0X92,
    "SUB E": 0X93,
    "SUB H": 0X94,
    "SUB L": 0X95,
    "SUB M": 0X96,
    "SUB A": 0X97,
    "SBB B": 0X98,
    "SBB C": 0X99,
    "SBB D": 0X9A,
    "SBB E": 0X9B,
    "SBB H": 0X9C,
    "SBB L": 0X9D,
    "SBB M": 0X9E,
    "SBB A": 0X9F,
    "ANA B": 0XA0,
    "ANA C": 0XA1,
    "ANA D": 0XA2,
    "ANA E": 0XA3,
    "ANA H": 0XA4,
    "ANA L": 0XA5,
    "ANA M": 0XA6,
    "ANA A": 0XA7,
    "XRA B": 0XA8,
    "XRA C": 0XA9,
    "XRA D": 0XAA,
    "XRA E": 0XAB,
    "XRA H": 0XAC,
    "XRA L": 0XAD,
    "XRA M": 0XAE,
    "XRA A": 0XAF,
    "ORA B": 0XB0,
    "ORA C": 0XB1,
    "ORA D": 0XB2,
    "ORA E": 0XB3,
    "ORA H": 0XB4,
    "ORA L": 0XB5,
    "ORA M": 0XB6,
    "ORA A": 0XB7,
    "CMP B": 0XB8,
    "CMP C": 0XB9,
    "CMP D": 0XBA,
    "CMP E": 0XBB,
    "CMP H": 0XBC,
    "CMP L": 0XBD,
    "CMP M": 0XBE,
    "CMP A": 0XBF,
    "RNZ": 0XC0,
    "POP B": 0XC1,
    "JNZ": 0XC2,
    "JMP": 0XC3,
    "CNZ": 0XC4,
    "PUSH B": 0XC5,
    "ADI": 0XC6,
    "RST 0": 0XC7,
    "RZ": 0XC8,
    "RET": 0XC9,
    "JZ": 0XCA,
    "CZ": 0XCC,
    "CALL": 0XCD,
    "ACI": 0XCE,
    "RST 1": 0XCF,
    "RNC": 0XD0,
    "POP D": 0XD1,
    "JNC": 0XD2,
    "OUT": 0XD3,
    "CNC": 0XD4,
    "PUSH D": 0XD5,
    "SUI": 0XD6,
    "RST 2": 0XD7,
    "RC": 0XD8,
    "JC": 0XDA,
    "IN": 0XDB,
    "CC": 0XDC,
    "SBI": 0XDE,
    "RST 3": 0XDF,
    "RPO": 0XE0,
    "POP H": 0XE1,
    "JPO": 0XE2,
    "XTHL": 0XE3,
    "CPO": 0XE4,
    "PUSH H": 0XE5,
    "ANI": 0XE6,
    "RST 4": 0XE7,
    "RPE": 0XE8,
    "PCHL": 0XE9,
    "JPE": 0XEA,
    "XCHG": 0XEB,
    "CPE": 0XEC,
    "XRI": 0XEE,
    "RST 5": 0XEF,
    "RP": 0XF0,
    "POP PSW": 0XF1,
    "JP": 0XF2,
    "DI": 0XF3,
    "CP": 0XF4,
    "PUSH PSW": 0XF5,
    "ORI": 0XF6,
    "RST 6": 0XF7,
    "RM": 0XF8,
    "SPHL": 0XF9,
    "JM": 0XFA,
    "EI": 0XFB,
    "CM": 0XFC,
    "CPI": 0XFE,
    "RST 7": 0XFF
}


def processInstruction(instruction):
    ins = ""
    oper = ""
    if "0x" in instruction:
        # assumption: addresses and operands are *all* in hex
        instructionAndOperand = instruction.split("0x")
        ins = instructionAndOperand[0].strip("\n, ")
        oper = instructionAndOperand[1].strip()
    else:
        ins = instruction.strip('\n, ')
    return ins, oper


def getInstruction(instruction):
    res = bytearray()
    ins, oper = processInstruction(instruction)
    insBytes = instructionDict[ins]
    res.append(insBytes)
    if oper:
        operBytes = int(oper, 16)
        if operBytes > 0xFF:
            # get the upper 8 bits
            operBytes1 = operBytes >> 8

            # mask off the upper 8 bits to get lower 8 bits
            mask = operBytes1 << 8
            operBytes2 = operBytes ^ mask
            res.append(operBytes1)
            res.append(operBytes2)
        else:
            res.append(operBytes)
    return res


def main():
    toWrite = bytearray()
    singleInstruction = bytearray
    with open(sys.argv[1], 'r') as f:
        assemblerLines = f.readlines()
        for line in assemblerLines:
            singleInstruction = getInstruction(line)
            toWrite = toWrite+singleInstruction
    fileName = os.path.basename(sys.argv[1])
    path = sys.argv[1].replace(fileName, '')
    fileName = fileName.replace("8080", '')
    outName = fileName+"rom"
    with open(path+outName, 'wb') as f:
        f.write(toWrite)


if __name__ == "__main__":
    main()
