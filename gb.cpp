/*

This is a gameboy binary disassembler... technically for the Custom(-ized from the Z80) 8-bit Sharp LR35902

*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string.h>
#include <sstream>

using namespace std;

int counter=0; //Keeps track of what address we're disassembling

void printByteAsHex(unsigned char byte)
{
    cout << " " << setw(2) << setfill('0') << hex << (int)byte;
}

unsigned char getNext8Bits(ifstream &file)
{
    char *buffer = new char [1]; //We're going to read this in 1 byte chunks
    file.read(buffer,1); //Read the next byte
    return *buffer;
}

void print8BitLine(string instruction, string description, string impactedRegister, unsigned char byte1)
{
    printByteAsHex(byte1);

    //Print the value of these bytes into our function
    cout << setfill(' ') << setw(15) << instruction;
    printByteAsHex(byte1); //Print the value
    cout << setfill(' ') << setw(30);

    //Print the value of these bytes into the description of this function
    printByteAsHex(byte1);
    cout << impactedRegister;
    cout << setfill(' ') << setw(45) << description;

    //Advance our addresses appropriately
    counter=counter+1;
}

void print16BitLine(string instruction, string description, string impactedRegister, unsigned char byte1, unsigned char byte2)
{
    cout << " ";
    printByteAsHex(byte1);
    printByteAsHex(byte2);
                
    //Print the value of these bytes into our function
    cout << "\t  " << instruction;
    printByteAsHex(byte1);
    printByteAsHex(byte2);
    cout << "\t\t\t\t";

    //Print the value of these bytes into the description of this function
    printByteAsHex(byte1);
    printByteAsHex(byte2);
    cout << impactedRegister << "\t" << description;

    //Advance our addresses appropriately
    counter=counter+2;
}

int main(int argc, char * argv[])
{
    if(argc<1)
    {
        cout << "Usage: gbd filename.gb" << endl;
        //exit();
    }
    cout << "Disassembling " << argv[1] << "...\n" << endl;

    //Grab our first argument (our filename to open)
	ifstream inputfile (argv[1], ifstream::binary);
	
	unsigned char *buffer = new unsigned char [1]; //Set up our buffer

	
	//Let's get the length of the file
	inputfile.seekg(0, inputfile.end);
	int length = inputfile.tellg();
	inputfile.seekg(0, inputfile.beg);
	
    //And we'll declare some variables to handle the CB prefix opcodes
    unsigned char tempCB;
    unsigned char thisByteCB;
    unsigned char byte1,byte2;
	
	//Scan until the file is done
	while(!inputfile.eof())
	{
        cout << setw(4) << setfill('0') << hex << counter << ":" << setw(1); //Print our current address in the left margin

        *buffer = getNext8Bits(inputfile); //Read a byte from the file
        printByteAsHex(*buffer);//Print our opcode
        
        unsigned char thisByte = *buffer; //Get the current opcode by itself

        //We only want to print opcodes as if they're ASCII if they actually fall in the appropriate range
        //We don't want to try to print the unprintable
        if(*buffer >= 32 && *buffer<=126) cout << "   " << (char)*buffer;
        

        /*
            Now, as we parse our opcodes, print what they are

        */

        switch(*buffer)
        {
            case 0x00:
                cout << "\t  NOP";
                break;
            case 0x01: //LD BC,d16
               
                //Since this is a 16-bit function, pull both arguments
               byte1 = getNext8Bits(inputfile);
               byte2 = getNext8Bits(inputfile);
               
                print16BitLine("LD BC", "Load a 16-bit immediate value into registers BC", "->BC", byte1, byte2);



                break;
            case 0x02:
                cout << "\t  LD (BC),A";
                break;
            case 0x03:
                cout << "\t  INC BC\t\t\t\t\tIncrement Register";
                break;
            case 0x04:
                cout << "\t  INC B\t\t\t\t\tIncrement Register";
                break;
            case 0x05:
                cout << "\t  DEC B\t\t\t\t\tDecrement Register";
                break;
            case 0x06: //LD B, d8

                byte1 = getNext8Bits(inputfile);
               
                print8BitLine("LD B", "\tLoad a 8-bit immediate value into register B", "->B", byte1);

                break;
            case 0x07: //RCLA, check here for more info: https://hax.iimarckus.org/topic/1617/
                cout << "\t  RLCA\t\t\t\t\tRotate Left, carry A";
                break;
            case 0x08:
                cout << "\t  LD (a16),SP";
                break;
            case 0x09:
                cout << "\t  ADD HL,BC";
                break;
            case 0x0A:
                cout << "\t  LD A,(BC)";
                break;
            case 0x0B:
                cout << "\t  DEC BC\t\t\t\t\tDecrement Register";
                break;
            case 0x0C:
                cout << "\t  INC C\t\t\t\t\tIncrement Register";
                break;
            case 0x0D:
                cout << "\t  DEC C\t\t\t\t\tDecrement Register";
                break;
            case 0x0E:
                cout << "\t  LD C,d8";
                break;
            case 0x0F:
                cout << "\t  RRCA";
                break;

            case 0x10:
                cout << "\t  STOP 0";
                break;
            case 0x11:
                cout << "\t  LD DE,d16";
                break;
            case 0x12:
                cout << "\t  LD (DE),A";
                break;
            case 0x13:
                cout << "\t  INC DE\t\t\t\t\tIncrement Register";
                break;
            case 0x14:
                cout << "\t  INC D\t\t\t\t\tIncrement Register";
                break;
            case 0x15:
                cout << "\t  DEC D\t\t\t\t\tDecrement Register";
                break;
            case 0x16:
                cout << "\t  LD D,d8";
                break;
            case 0x17:
                cout << "\t  RLA";
                break;
            case 0x18:
                cout << "\t  JR r8";
                break;
            case 0x19:
                cout << "\t  ADD HL,DE";
                break;
            case 0x1A:
                cout << "\t  LD A,(DE)";
                break;
            case 0x1B:
                cout << "\t  DEC DE\t\t\t\t\tDecrement Register";
                break;
            case 0x1C:
                cout << "\t  INC E\t\t\t\t\tIncrement Register";
                break;
            case 0x1D:
                cout << "\t  DEC E\t\t\t\t\tDecrement Register";
                break;
            case 0x1E:
                cout << "\t  LD E,d8";
                break;
            case 0x1F:
                cout << "\t  RRA";
                break;

            case 0x20:
                cout << "\t  JR NZ,r8";
                break;
            case 0x21:
                cout << "\t  LD HL,d16";
                break;
            case 0x22:
                cout << "\t  LD(HL+),A";
                break;
            case 0x23:
                cout << "\t  INC HL\t\t\t\t\tIncrement Register";
                break;
            case 0x24:
                cout << "\t  INC H\t\t\t\t\tIncrement Register";
                break;
            case 0x25:
                cout << "\t  DEC H\t\t\t\t\tDecrement Register";
                break;
            case 0x26:
                cout << "\t  LD H,d8";
                break;
            case 0x27:
                cout << "\t  DAA";
                break;
            case 0x28:
                cout << "\t  JR Z,r8";
                break;
            case 0x29:
                cout << "\t  ADD HL,HL";
                break;
            case 0x2A:
                cout << "\t  LD A,(HL+)";
                break;
            case 0x2B:
                cout << "\t  DEC HL\t\t\t\t\tDecrement Register";
                break;
            case 0x2C:
                cout << "\t  INC L\t\t\t\t\tIncrement Register";
                break;
            case 0x2D:
                cout << "\t  DEC L\t\t\t\t\tDecrement Register";
                break;
            case 0x2E:
                cout << "\t  LD L,d8";
                break;
            case 0x2F:
                cout << "\t  CPL";
                break;

            case 0x30:
                cout << "\t  JR NC,r8";
                break;
            case 0x31:
                cout << "\t  LD SP,d16";
                break;
            case 0x32:
                cout << "\t  LD (HL-),A";
                break;
            case 0x33:
                cout << "\t  INC SP\t\t\t\t\tIncrement Register";
                break;
            case 0x34:
                cout << "\t  INC (HL)\t\t\t\t\tIncrement Register";
                break;
            case 0x35:
                cout << "\t  DEC (HL)\t\t\t\t\t\tDecrement Register";
                break;
            case 0x36:
                cout << "\t  LD (HL),d8";
                break;
            case 0x37:
                cout << "\t  SCF";
                break;
            case 0x38:
                cout << "\t  JR C,r8";
                break;
            case 0x39:
                cout << "\t  ADD HL,SP";
                break;
            case 0x3A:
                cout << "\t  LD A,(HL-)";
                break;
            case 0x3B:
                cout << "\t  DEC SP\t\t\t\t\tDecrement Register";
                break;
            case 0x3C:
                cout << "\t  INC A";
                break;
            case 0x3D:
                cout << "\t  DEC A\t\t\t\t\tDecrement Register";
                break;
            case 0x3E:
                cout << "\t  LD A,d8";
                break;
            case 0x3F:
                cout << "\t  CCF";
                break;

            case 0x40:
                cout << "\t  LD B,B\t\t\t\t\tB->B\tCopy register B into itself, usually used as a flag reset";
                break;
            case 0x41:
                cout << "\t  LD B,C\t\t\t\t\tC->B\tCopy the value in register C into register B";
                break;
            case 0x42:
                cout << "\t  LD B,D\t\t\t\t\tD->B\tCopy the value in register D into register B";
                break;
            case 0x43:
                cout << "\t  LD B,E\t\t\t\t\tE->B\tCopy the value in register E into register B";
                break;
            case 0x44:
                cout << "\t  LD B,H\t\t\t\t\tH->B\tCopy the value in register H into register B";
                break;
            case 0x45:
                cout << "\t  LD B,L\t\t\t\t\tL->B\tCopy the value in register L into register B";
                break;
            case 0x46:
                cout << "\t  LD B,(HL)\t\t\t\t\tHL->B\tCopy the value in register HL into register B";
                break;
            case 0x47:
                cout << "\t  LD B,A\t\t\t\t\tA->B\tCopy the value in register A into register B";
                break;
            case 0x48:
                cout << "\t  LD C,B\t\t\t\t\tB->C\tCopy the value in register B into register C";
                break;
            case 0x49:
                cout << "\t  LD C,C\t\t\t\t\tC->C\tCopy register C into itself, usually used as a flag reset";
                break;
            case 0x4A:
                cout << "\t  LD C,D\t\t\t\t\tD->C\tCopy the value in register D into register C";
                break;
            case 0x4B:
                cout << "\t  LD C,E\t\t\t\t\tE->C\tCopy the value in register E into register C";
                break;
            case 0x4C:
                cout << "\t  LD C,H\t\t\t\t\tH->C\tCopy the value in register H into register C";
                break;
            case 0x4D:
                cout << "\t  LD C,L\t\t\t\t\tL->C\tCopy the value in register L into register C";
                break;
            case 0x4E:
                cout << "\t  LD C,(HL)\t\t\t\t\tHL->C\tCopy the value in register HL into register C";
                break;
            case 0x4F:
                cout << "\t  LD C,A\t\t\t\t\tA->C\tCopy the value in register A into register C";
                break;

            case 0x50:
                cout << "\t  LD D,B\t\t\t\t\tB->D\tCopy the value in register B into register D";
                break;
            case 0x51:
                cout << "\t  LD D,C\t\t\t\t\tC->D\tCopy the value in register C into register D";
                break;
            case 0x52:
                cout << "\t  LD D,D\t\t\t\t\tD->D\tCopy register D into itself, usually used as a flag reset";
                break;
            case 0x53:
                cout << "\t  LD D,E\t\t\t\t\tE->D\tCopy the value in register E into register D";
                break;
            case 0x54:
                cout << "\t  LD D,H\t\t\t\t\tH->D\tCopy the value in register H into register D";
                break;
            case 0x55:
                cout << "\t  LD D,L\t\t\t\t\tL->D\tCopy the value in register L into register D";
                break;
            case 0x56:
                cout << "\t  LD D,(HL)\t\t\t\t\tHL->D\tCopy the value in register HL into register D";
                break;
            case 0x57:
                cout << "\t  LD D,A\t\t\t\t\tA->D\tCopy the value in register A into register D";
                break;
            case 0x58:
                cout << "\t  LD E,B\t\t\t\t\tB->E\tCopy the value in register B into register E";
                break;
            case 0x59:
                cout << "\t  LD E,C\t\t\t\t\tC->E\tCopy the value in register C into register E";
                break;
            case 0x5A:
                cout << "\t  LD E,D\t\t\t\t\tE->E\tCopy the value in register E into register E";
                break;
            case 0x5B:
                cout << "\t  LD E,E\t\t\t\t\tE->E\tCopy register E into itself, usually used as a flag reset";
                break;
            case 0x5C:
                cout << "\t  LD E,H\t\t\t\t\tH->E\tCopy the value in register H into register E";
                break;
            case 0x5D:
                cout << "\t  LD E,L\t\t\t\t\tL->E\tCopy the value in register L into register E";
                break;
            case 0x5E:
                cout << "\t  LD E,(HL)\t\t\t\t\tHL->E\tCopy the value in register HL into register E";
                break;
            case 0x5F:
                cout << "\t  LD E,A\t\t\t\t\tA->E\tCopy the value in register A into register E";
                break;

            case 0x60:
                cout << "\t  LD H,B\t\t\t\t\tB->H\tCopy the value in register B into register H";
                break;
            case 0x61:
                cout << "\t  LD H,C\t\t\t\t\tC->H\tCopy the value in register C into register H";
                break;
            case 0x62:
                cout << "\t  LD H,D";
                break;
            case 0x63:
                cout << "\t  LD H,B";
                break;
            case 0x64:
                cout << "\t  LD H,H";
                break;
            case 0x65:
                cout << "\t  LD H,L";
                break;
            case 0x66:
                cout << "\t  LD H,(HL)";
                break;
            case 0x67:
                cout << "\t  LD H,A";
                break;
            case 0x68:
                cout << "\t  LD H,B";
                break;
            case 0x69:
                cout << "\t  LD L,C";
                break;
            case 0x6A:
                cout << "\t  LD H,B";
                break;
            case 0x6B:
                cout << "\t  LD L,E";
                break;
            case 0x6C:
                cout << "\t  LD L,H";
                break;
            case 0x6D:
                cout << "\t  LD L,L";
                break;
            case 0x6E:
                cout << "\t  LD L,(HL)";
                break;
            case 0x6F:
                cout << "\t  LD L,A";
                break;
            case 0x70:
                cout << "\t  LD (HL),B";
                break;
            case 0x71:
                cout << "\t  LD (HL),C";
                break;
            case 0x72:
                cout << "\t  LD (HL),D";
                break;
            case 0x73:
                cout << "\t  LD (HL),E";
                break;
            case 0x74:
                cout << "\t  LD (HL),H";
                break;
            case 0x75:
                cout << "\t  LD (HL),L";
                break;
            case 0x76:
                cout << "\t  HALT";
                break;
            case 0x77:
                cout << "\t  LD (HL),A";
                break;
            case 0x78:
                cout << "\t  LD A,B";
                break;
            case 0x79:
                cout << "\t  LD A,C";
                break;
            case 0x7A:
                cout << "\t  LD A,D";
                break;
            case 0x7B:
                cout << "\t  LD A,E";
                break;
            case 0x7C:
                cout << "\t  LD A,H";
                break;
            case 0x7D:
                cout << "\t  LD A,L";
                break;
            case 0x7E:
                cout << "\t  LD A,(HL)";
                break;
            case 0x7F:
                cout << "\t  LD A,A";
                break;
            case 0x80:
                cout << "\t  ADD A,B\t\t\t\t\tA+B\tAdd the values in registers A and B, with the result stored in A";
                break;
            case 0x81:
                cout << "\t  ADD A,C\t\t\t\t\tA+C\tAdd the values in registers A and C, with the result stored in A";
                break;
            case 0x82:
                cout << "\t  ADD A,D\t\t\t\t\tA+D\tAdd the values in registers A and D, with the result stored in A";
                break;
            case 0x83:
                cout << "\t  ADD A,E\t\t\t\t\tA+E\tAdd the values in registers A and E, with the result stored in A";
                break;
            case 0x84:
                cout << "\t  ADD A,H\t\t\t\t\tA+H\tAdd the values in registers A and H, with the result stored in A";
                break;
            case 0x85:
                cout << "\t  ADD A,L\t\t\t\t\tA+L\tAdd the values in registers A and L, with the result stored in A";
                break;
            case 0x86:
                cout << "\t  ADD A,(HL)\t\t\t\t\tA+HL\tAdd the values in registers A and HL, with the result stored in A";
                break;
            case 0x87:
                cout << "\t  ADD A,A\t\t\t\t\tA+A\tAdd the values in registers A and A, with the result stored in A";
                break;
            case 0x88:
                cout << "\t  ADC A,B";
                break;
            case 0x89:
                cout << "\t  ADC A,C";
                break;
            case 0x8A:
                cout << "\t  ADC A,D";
                break;
            case 0x8B:
                cout << "\t  ADC A,E";
                break;
            case 0x8C:
                cout << "\t  ADC A,H";
                break;
            case 0x8D:
                cout << "\t  ADC A,L";
                break;
            case 0x8E:
                cout << "\t  ADC A,(HL)";
                break;
            case 0x8F:
                cout << "\t  ADC A,A";
                break;

            case 0x90:
                cout << "\t  SUB B";
                break;
            case 0x91:
                cout << "\t  SUB C";
                break;
            case 0x92:
                cout << "\t  SUB D";
                break;
            case 0x93:
                cout << "\t  SUB E";
                break;
            case 0x94:
                cout << "\t  SUB H";
                break;
            case 0x95:
                cout << "\t  SUB L";
                break;
            case 0x96:
                cout << "\t  SUB (HL)";
                break;
            case 0x97:
                cout << "\t  SUB A";
                break;
            case 0x98:
                cout << "\t  SBC A,B";
                break;
            case 0x99:
                cout << "\t  SBC A,C";
                break;
            case 0x9A:
                cout << "\t  SBC A,D";
                break;
            case 0x9B:
                cout << "\t  SBC A,E";
                break;
            case 0x9C:
                cout << "\t  SBC A,H";
                break;
            case 0x9D:
                cout << "\t  SBC A,L";
                break;
            case 0x9E:
                cout << "\t  SBC A,(HL)";
                break;
            case 0x9F:
                cout << "\t  SBC A,A";
                break;

            case 0xA0:
                cout << "\t  AND B\t\t\t\t\tB && A\tDo the binary AND operation against B and A";
                break;
            case 0xA1:
                cout << "\t  AND C\t\t\t\t\tC && A\tDo the binary AND operation against C and A";
                break;
            case 0xA2:
                cout << "\t  AND D\t\t\t\t\tD && A\tDo the binary AND operation against D and A";
                break;
            case 0xA3:
                cout << "\t  AND E\t\t\t\t\tE && A\tDo the binary AND operation against E and A";
                break;
            case 0xA4:
                cout << "\t  AND H\t\t\t\t\tH && A\tDo the binary AND operation against H and A";
                break;
            case 0xA5:
                cout << "\t  AND L\t\t\t\t\tL && A\tDo the binary AND operation against L and A";
                break;
            case 0xA6:
                cout << "\t  AND (HL)\t\t\t\tHL && A\tDo the binary AND operation against HL and A";
                break;
            case 0xA7: //AND A               

                cout << "\t  AND A\t\t\t\t\tA && A\tDo the binary AND operation against A and A";
                break;
            case 0xA8:
                cout << "\t  XOR B\t\t\t\t\tA XOR B\tXor the values in registers A and B, with the result stored in A";
                break;
            case 0xA9:
                cout << "\t  XOR C\t\t\t\t\tA XOR C\tXor the values in registers A and C, with the result stored in A";
                break;
            case 0xAA:
                cout << "\t  XOR D\t\t\t\t\tA XOR D\tXor the values in registers A and D, with the result stored in A";
                break;
            case 0xAB:
                cout << "\t  XOR E\t\t\t\t\tA XOR E\tXor the values in registers A and E, with the result stored in A";
                break;
            case 0xAC:
                cout << "\t  XOR H\t\t\t\t\tA XOR H\tXor the values in registers A and H, with the result stored in A";
                break;
            case 0xAD:
                cout << "\t  XOR L\t\t\t\t\tA XOR L\tXor the values in registers A and L, with the result stored in A";
                break;
            case 0xAE:
                cout << "\t  XOR (HL)\t\t\t\t\tA XOR HL\tXor the values in registers A and HL, with the result stored in A";
                break;
            case 0xAF:
                cout << "\t  XOR A\t\t\t\t\tA XOR A\tXor the values in register A against itself. A now equals zero.";
                break;

            case 0xB0:
                cout << "\t  OR B\t\t\t\t\tA || B\tOr the values in registers A and B, with the result stored in A";
                break;
            case 0xB1:
                cout << "\t  OR C";
                break;
            case 0xB2:
                cout << "\t  OR D";
                break;
            case 0xB3:
                cout << "\t  OR E";
                break;
            case 0xB4:
                cout << "\t  OR H";
                break;
            case 0xB5:
                cout << "\t  OR L";
                break;
            case 0xB6:
                cout << "\t  OR (HL)";
                break;
            case 0xB7:
                cout << "\t  OR A";
                break;
            case 0xB8:
                cout << "\t  CP B";
                break;
            case 0xB9:
                cout << "\t  CP C";
                break;
            case 0xBA:
                cout << "\t  CP D";
                break;
            case 0xBB:
                cout << "\t  CP E";
                break;
            case 0xBC:
                cout << "\t  CP H";
                break;
            case 0xBD:
                cout << "\t  CP L";
                break;
            case 0xBE:
                cout << "\t  CP (HL)";
                break;
            case 0xBF:
                cout << "\t  CP A";
                break;

            case 0xC0:
                cout << "\t  RET NZ";
                break;
            case 0xC1:
                cout << "\t  POP BC";
                break;
            case 0xC2:
                cout << "\t  JP NZ,a16";
                break;
            case 0xC3: //JP a16
                byte1 = getNext8Bits(inputfile);
                byte2 = getNext8Bits(inputfile);
               
                print16BitLine("JP ", "Jump to the specified 16-bit address", "->PC", byte1, byte2);

                //cout << "\t  JP a16";
                break;
            case 0xC4:
                cout << "\t  CALL NZ,a16";
                break;
            case 0xC5:
                cout << "\t  PUSH BC";
                break;
            case 0xC6:
                cout << "\t  ADD A,d8";
                break;
            case 0xC7:
                cout << "\t  RST 00H\t\t\t\t\tCall the code at 00"; //According to https://www.cemetech.net/forum/viewtopic.php?t=6744
                break;
            case 0xC8:
                cout << "\t  RET Z";
                break;
            case 0xC9:
                cout << "\t  RET";
                break;
            case 0xCA:
                cout << "\t  JP Z,a16";
                break;
            case 0xCB:
                /*
                    0xCB is a PREFIX opcode. This means that it's meant to be used like CB xx, and is used to hold another series of opcodes.
                    We need to pair it with the byte immediately following to perform the relevant function
                */

                //We need to get the next byte
                *buffer = getNext8Bits(inputfile);
                
                tempCB = *buffer;
                printByteAsHex(tempCB);
                
                thisByteCB = *buffer; //Get the current opcode by itself

                //And now we get to do the second huge switch/case to handle CB prefixed instructions
                switch(thisByteCB)
                {
                    case 0x00:
                        cout << "\t  RLC B";
                        break;
                    case 0x01:
                        cout << "\t  RLC C";
                        break;
                    case 0x02:
                        cout << "\t  RLC D";
                        break;
                    case 0x03:
                        cout << "\t  RLC E";
                        break;
                    case 0x04:
                        cout << "\t  RLC H";
                        break;
                    case 0x05:
                        cout << "\t  RLC L";
                        break;
                    case 0x06:
                        cout << "\t  RLC (HL)";
                        break;
                    case 0x07:
                        cout << "\t  RLC A";
                        break;
                    case 0x08:
                        cout << "\t  RRC B";
                        break;
                    case 0x09:
                        cout << "\t  RRC C";
                        break;
                    case 0x0A:
                        cout << "\t  RRC D";
                        break;
                    case 0x0B:
                        cout << "\t  RRC E";
                        break;
                    case 0x0C:
                        cout << "\t  RRC H";
                        break;
                    case 0x0D:
                        cout << "\t  RRC L";
                        break;
                    case 0x0E:
                        cout << "\t  RRC (HL)";
                        break;
                    case 0x0F:
                        cout << "\t  RRC A";
                        break;

                    case 0x10:
                        cout << "\t  RL B";
                        break;
                    case 0x11:
                        cout << "\t  RL C";
                        break;
                    case 0x12:
                        cout << "\t  RL D";
                        break;
                    case 0x13:
                        cout << "\t  RL E";
                        break;
                    case 0x14:
                        cout << "\t  RL H";
                        break;
                    case 0x15:
                        cout << "\t  RLC L";
                        break;
                    case 0x16:
                        cout << "\t  RLC (HL)";
                        break;
                    case 0x17:
                        cout << "\t  RL A";
                        break;
                    case 0x18:
                        cout << "\t  RR B";
                        break;
                    case 0x19:
                        cout << "\t  RR C";
                        break;
                    case 0x1A:
                        cout << "\t  RR D";
                        break;
                    case 0x1B:
                        cout << "\t  RR E";
                        break;
                    case 0x1C:
                        cout << "\t  RR H";
                        break;
                    case 0x1D:
                        cout << "\t  RR L";
                        break;
                    case 0x1E:
                        cout << "\t  RR (HL)";
                        break;
                    case 0x1F:
                        cout << "\t  RR A";
                        break;

                    case 0x20:
                        cout << "\t  SLA B";
                        break;
                    case 0x21:
                        cout << "\t  SLA C";
                        break;
                    case 0x22:
                        cout << "\t  SLA D";
                        break;
                    case 0x23:
                        cout << "\t  SLA E";
                        break;
                    case 0x24:
                        cout << "\t  SLA H";
                        break;
                    case 0x25:
                        cout << "\t  SLA L";
                        break;
                    case 0x26:
                        cout << "\t  SLA (HL)";
                        break;
                    case 0x27:
                        cout << "\t  SLA A";
                        break;
                    case 0x28:
                        cout << "\t  SRA B";
                        break;
                    case 0x29:
                        cout << "\t  SRA C";
                        break;
                    case 0x2A:
                        cout << "\t  SRA D";
                        break;
                    case 0x2B:
                        cout << "\t  SRA E";
                        break;
                    case 0x2C:
                        cout << "\t  SRA H";
                        break;
                    case 0x2D:
                        cout << "\t  SRA L";
                        break;
                    case 0x2E:
                        cout << "\t  SRA (HL)";
                        break;
                    case 0x2F:
                        cout << "\t  SRA A";
                        break;

                    case 0x30:
                        cout << "\t  SWAP B";
                        break;
                    case 0x31:
                        cout << "\t  SWAP C";
                        break;
                    case 0x32:
                        cout << "\t  SWAP D";
                        break;
                    case 0x33:
                        cout << "\t  SWAP E";
                        break;
                    case 0x34:
                        cout << "\t  SWAP H";
                        break;
                    case 0x35:
                        cout << "\t  SWAP L";
                        break;
                    case 0x36:
                        cout << "\t  SWAP (HL)";
                        break;
                    case 0x37:
                        cout << "\t  SWAP A";
                        break;
                    case 0x38:
                        cout << "\t  SRL B";
                        break;
                    case 0x39:
                        cout << "\t  SRL C";
                        break;
                    case 0x3A:
                        cout << "\t  SRL D";
                        break;
                    case 0x3B:
                        cout << "\t  SRL E";
                        break;
                    case 0x3C:
                        cout << "\t  SRL H";
                        break;
                    case 0x3D:
                        cout << "\t  SRL L";
                        break;
                    case 0x3E:
                        cout << "\t  SRL (HL)";
                        break;
                    case 0x3F:
                        cout << "\t  SRL A";
                        break;

                    case 0x40:
                        cout << "\t  BIT 0,B";
                        break;
                    case 0x41:
                        cout << "\t  BIT 0,C";
                        break;
                    case 0x42:
                        cout << "\t  BIT 0,D";
                        break;
                    case 0x43:
                        cout << "\t  BIT 0,E";
                        break;
                    case 0x44:
                        cout << "\t  BIT 0,H";
                        break;
                    case 0x45:
                        cout << "\t  BIT 0,L";
                        break;
                    case 0x46:
                        cout << "\t  BIT 0,(HL)";
                        break;
                    case 0x47:
                        cout << "\t  BIT 0,A";
                        break;
                    case 0x48:
                        cout << "\t  BIT 1,B";
                        break;
                    case 0x49:
                        cout << "\t  BIT 1,C";
                        break;
                    case 0x4A:
                        cout << "\t  BIT 1,D";
                        break;
                    case 0x4B:
                        cout << "\t  BIT 1,E";
                        break;
                    case 0x4C:
                        cout << "\t  BIT 1,H";
                        break;
                    case 0x4D:
                        cout << "\t  BIT 1,L";
                        break;
                    case 0x4E:
                        cout << "\t  BIT 1,(HL)";
                        break;
                    case 0x4F:
                        cout << "\t  BIT 1,A";
                        break;

                    case 0x50:
                        cout << "\t  BIT 2,B";
                        break;
                    case 0x51:
                        cout << "\t  BIT 2,C";
                        break;
                    case 0x52:
                        cout << "\t  BIT 2,D";
                        break;
                    case 0x53:
                        cout << "\t  BIT 2,E";
                        break;
                    case 0x54:
                        cout << "\t  BIT 2,H";
                        break;
                    case 0x55:
                        cout << "\t  BIT 2,L";
                        break;
                    case 0x56:
                        cout << "\t  BIT 2,(HL)";
                        break;
                    case 0x57:
                        cout << "\t  BIT 2,A";
                        break;
                    case 0x58:
                        cout << "\t  BIT 3,B";
                        break;
                    case 0x59:
                        cout << "\t  BIT 3,C";
                        break;
                    case 0x5A:
                        cout << "\t  BIT 3,D";
                        break;
                    case 0x5B:
                        cout << "\t  BIT 3,E";
                        break;
                    case 0x5C:
                        cout << "\t  BIT 3,H";
                        break;
                    case 0x5D:
                        cout << "\t  BIT 3,L";
                        break;
                    case 0x5E:
                        cout << "\t  BIT 3,(HL)";
                        break;
                    case 0x5F:
                        cout << "\t  BIT 3,A";
                        break;

                    case 0x60:
                        cout << "\t  BIT 4,B";
                        break;
                    case 0x61:
                        cout << "\t  BIT 4,C";
                        break;
                    case 0x62:
                        cout << "\t  BIT 4,D";
                        break;
                    case 0x63:
                        cout << "\t  BIT 4,E";
                        break;
                    case 0x64:
                        cout << "\t  BIT 4,H";
                        break;
                    case 0x65:
                        cout << "\t  BIT 4,L";
                        break;
                    case 0x66:
                        cout << "\t  BIT 4,(HL)";
                        break;
                    case 0x67:
                        cout << "\t  BIT 4,A";
                        break;
                    case 0x68:
                        cout << "\t  BIT 5,B";
                        break;
                    case 0x69:
                        cout << "\t  BIT 5,C";
                        break;
                    case 0x6A:
                        cout << "\t  BIT 5,D";
                        break;
                    case 0x6B:
                        cout << "\t  BIT 5,E";
                        break;
                    case 0x6C:
                        cout << "\t  BIT 5,H";
                        break;
                    case 0x6D:
                        cout << "\t  BIT 5,L";
                        break;
                    case 0x6E:
                        cout << "\t  BIT 5,(HL)";
                        break;
                    case 0x6F:
                        cout << "\t  BIT 5,A";
                        break;

                    case 0x70:
                        cout << "\t  BIT 6,B";
                        break;
                    case 0x71:
                        cout << "\t  BIT 6,C";
                        break;
                    case 0x72:
                        cout << "\t  BIT 6,D";
                        break;
                    case 0x73:
                        cout << "\t  BIT 6,E";
                        break;
                    case 0x74:
                        cout << "\t  BIT 6,H";
                        break;
                    case 0x75:
                        cout << "\t  BIT 6,L";
                        break;
                    case 0x76:
                        cout << "\t  BIT 6,(HL)";
                        break;
                    case 0x77:
                        cout << "\t  BIT 6,A";
                        break;
                    case 0x78:
                        cout << "\t  BIT 7,B";
                        break;
                    case 0x79:
                        cout << "\t  BIT 7,C";
                        break;
                    case 0x7A:
                        cout << "\t  BIT 7,D";
                        break;
                    case 0x7B:
                        cout << "\t  BIT 7,E";
                        break;
                    case 0x7C:
                        cout << "\t  BIT 7,H";
                        break;
                    case 0x7D:
                        cout << "\t  BIT 7,L";
                        break;
                    case 0x7E:
                        cout << "\t  BIT 7,(HL)";
                        break;
                    case 0x7F:
                        cout << "\t  BIT 7,A";
                        break;

                    case 0x80:
                        cout << "\t  RES 0,B";
                        break;
                    case 0x81:
                        cout << "\t  RES 0,C";
                        break;
                    case 0x82:
                        cout << "\t  RES 0,D";
                        break;
                    case 0x83:
                        cout << "\t  RES 0,E";
                        break;
                    case 0x84:
                        cout << "\t  RES 0,H";
                        break;
                    case 0x85:
                        cout << "\t  RES 0,L";
                        break;
                    case 0x86:
                        cout << "\t  RES 0,(HL)";
                        break;
                    case 0x87:
                        cout << "\t  RES 0,A";
                        break;
                    case 0x88:
                        cout << "\t  RES 1,B";
                        break;
                    case 0x89:
                        cout << "\t  RES 1,C";
                        break;
                    case 0x8A:
                        cout << "\t  RES 1,D";
                        break;
                    case 0x8B:
                        cout << "\t  RES 1,E";
                        break;
                    case 0x8C:
                        cout << "\t  RES 1,H";
                        break;
                    case 0x8D:
                        cout << "\t  RES 1,L";
                        break;
                    case 0x8E:
                        cout << "\t  RES 1,(HL)";
                        break;
                    case 0x8F:
                        cout << "\t  RES 1,A";
                        break;

                    case 0x90:
                        cout << "\t  RES 2,B";
                        break;
                    case 0x91:
                        cout << "\t  RES 2,C";
                        break;
                    case 0x92:
                        cout << "\t  RES 2,D";
                        break;
                    case 0x93:
                        cout << "\t  RES 2,E";
                        break;
                    case 0x94:
                        cout << "\t  RES 2,H";
                        break;
                    case 0x95:
                        cout << "\t  RES 2,L";
                        break;
                    case 0x96:
                        cout << "\t  RES 2,(HL)";
                        break;
                    case 0x97:
                        cout << "\t  RES 2,A";
                        break;
                    case 0x98:
                        cout << "\t  RES 3,B";
                        break;
                    case 0x99:
                        cout << "\t  RES 3,C";
                        break;
                    case 0x9A:
                        cout << "\t  RES 3,D";
                        break;
                    case 0x9B:
                        cout << "\t  RES 3,E";
                        break;
                    case 0x9C:
                        cout << "\t  RES 3,H";
                        break;
                    case 0x9D:
                        cout << "\t  RES 3,L";
                        break;
                    case 0x9E:
                        cout << "\t  RES 3,(HL)";
                        break;
                    case 0x9F:
                        cout << "\t  RES 3,A";
                        break;

                    case 0xA0:
                        cout << "\t  RES 4,B";
                        break;
                    case 0xA1:
                        cout << "\t  RES 4,C";
                        break;
                    case 0xA2:
                        cout << "\t  RES 4,D";
                        break;
                    case 0xA3:
                        cout << "\t  RES 4,E";
                        break;
                    case 0xA4:
                        cout << "\t  RES 4,H";
                        break;
                    case 0xA5:
                        cout << "\t  RES 4,L";
                        break;
                    case 0xA6:
                        cout << "\t  RES 4,(HL)";
                        break;
                    case 0xA7:
                        cout << "\t  RES 4,A";
                        break;
                    case 0xA8:
                        cout << "\t  RES 5,B";
                        break;
                    case 0xA9:
                        cout << "\t  RES 5,C";
                        break;
                    case 0xAA:
                        cout << "\t  RES 5,D";
                        break;
                    case 0xAB:
                        cout << "\t  RES 5,E";
                        break;
                    case 0xAC:
                        cout << "\t  RES 5,H";
                        break;
                    case 0xAD:
                        cout << "\t  RES 5,L";
                        break;
                    case 0xAE:
                        cout << "\t  RES 5,(HL)";
                        break;
                    case 0xAF:
                        cout << "\t  RES 5,A";
                        break;

                    case 0xB0:
                        cout << "\t  RES 6,B";
                        break;
                    case 0xB1:
                        cout << "\t  RES 6,C";
                        break;
                    case 0xB2:
                        cout << "\t  RES 6,D";
                        break;
                    case 0xB3:
                        cout << "\t  RES 6,E";
                        break;
                    case 0xB4:
                        cout << "\t  RES 6,H";
                        break;
                    case 0xB5:
                        cout << "\t  RES 6,L";
                        break;
                    case 0xB6:
                        cout << "\t  RES 6,(HL)";
                        break;
                    case 0xB7:
                        cout << "\t  RES 6,A";
                        break;
                    case 0xB8:
                        cout << "\t  RES 7,B";
                        break;
                    case 0xB9:
                        cout << "\t  RES 7,C";
                        break;
                    case 0xBA:
                        cout << "\t  RES 7,D";
                        break;
                    case 0xBB:
                        cout << "\t  RES 7,E";
                        break;
                    case 0xBC:
                        cout << "\t  RES 7,H";
                        break;
                    case 0xBD:
                        cout << "\t  RES 7,L";
                        break;
                    case 0xBE:
                        cout << "\t  RES 7,(HL)";
                        break;
                    case 0xBF:
                        cout << "\t  RES 7,A";
                        break;

                    case 0xC0:
                        cout << "\t  SET 0,B";
                        break;
                    case 0xC1:
                        cout << "\t  SET 0,C";
                        break;
                    case 0xC2:
                        cout << "\t  SET 0,D";
                        break;
                    case 0xC3:
                        cout << "\t  SET 0,E";
                        break;
                    case 0xC4:
                        cout << "\t  SET 0,H";
                        break;
                    case 0xC5:
                        cout << "\t  SET 0,L";
                        break;
                    case 0xC6:
                        cout << "\t  SET 0,(HL)";
                        break;
                    case 0xC7:
                        cout << "\t  SET 0,A";
                        break;
                    case 0xC8:
                        cout << "\t  SET 1,B";
                        break;
                    case 0xC9:
                        cout << "\t  SET 1,C";
                        break;
                    case 0xCA:
                        cout << "\t  SET 1,D";
                        break;
                    case 0xCB:
                        cout << "\t  SET 1,E";
                        break;
                    case 0xCC:
                        cout << "\t  SET 1,H";
                        break;
                    case 0xCD:
                        cout << "\t  SET 1,L";
                        break;
                    case 0xCE:
                        cout << "\t  SET 1,(HL)";
                        break;
                    case 0xCF:
                        cout << "\t  SET 1,A";
                        break;


                    case 0xD0:
                        cout << "\t  SET 2,B";
                        break;
                    case 0xD1:
                        cout << "\t  SET 2,C";
                        break;
                    case 0xD2:
                        cout << "\t  SET 2,D";
                        break;
                    case 0xD3:
                        cout << "\t  SET 2,E";
                        break;
                    case 0xD4:
                        cout << "\t  SET 2,H";
                        break;
                    case 0xD5:
                        cout << "\t  SET 2,L";
                        break;
                    case 0xD6:
                        cout << "\t  SET 2,(HL)";
                        break;
                    case 0xD7:
                        cout << "\t  SET 2,A";
                        break;
                    case 0xD8:
                        cout << "\t  SET 3,B";
                        break;
                    case 0xD9:
                        cout << "\t  SET 3,C";
                        break;
                    case 0xDA:
                        cout << "\t  SET 3,D";
                        break;
                    case 0xDB:
                        cout << "\t  SET 3,E";
                        break;
                    case 0xDC:
                        cout << "\t  SET 3,H";
                        break;
                    case 0xDD:
                        cout << "\t  SET 3,L";
                        break;
                    case 0xDE:
                        cout << "\t  SET 3,(HL)";
                        break;
                    case 0xDF:
                        cout << "\t  SET 3,A";
                        break;

                    case 0xE0:
                        cout << "\t  SET 4,B";
                        break;
                    case 0xE1:
                        cout << "\t  SET 4,C";
                        break;
                    case 0xE2:
                        cout << "\t  SET 4,D";
                        break;
                    case 0xE3:
                        cout << "\t  SET 4,E";
                        break;
                    case 0xE4:
                        cout << "\t  SET 4,H";
                        break;
                    case 0xE5:
                        cout << "\t  SET 4,L";
                        break;
                    case 0xE6:
                        cout << "\t  SET 4,(HL)";
                        break;
                    case 0xE7:
                        cout << "\t  SET 4,A";
                        break;
                    case 0xE8:
                        cout << "\t  SET 5,B";
                        break;
                    case 0xE9:
                        cout << "\t  SET 5,C";
                        break;
                    case 0xEA:
                        cout << "\t  SET 5,D";
                        break;
                    case 0xEB:
                        cout << "\t  SET 5,E";
                        break;
                    case 0xEC:
                        cout << "\t  SET 5,H";
                        break;
                    case 0xED:
                        cout << "\t  SET 5,L";
                        break;
                    case 0xEE:
                        cout << "\t  SET 5,(HL)";
                        break;
                    case 0xEF:
                        cout << "\t  SET 5,A";
                        break;

                    case 0xF0:
                        cout << "\t  SET 6,B";
                        break;
                    case 0xF1:
                        cout << "\t  SET 6,C";
                        break;
                    case 0xF2:
                        cout << "\t  SET 6,D";
                        break;
                    case 0xF3:
                        cout << "\t  SET 6,E";
                        break;
                    case 0xF4:
                        cout << "\t  SET 6,H";
                        break;
                    case 0xF5:
                        cout << "\t  SET 6,L";
                        break;
                    case 0xF6:
                        cout << "\t  SET 6,(HL)";
                        break;
                    case 0xF7:
                        cout << "\t  SET 6,A";
                        break;
                    case 0xF8:
                        cout << "\t  SET 7,B";
                        break;
                    case 0xF9:
                        cout << "\t  SET 7,C";
                        break;
                    case 0xFA:
                        cout << "\t  SET 7,D";
                        break;
                    case 0xFB:
                        cout << "\t  SET 7,E";
                        break;
                    case 0xFC:
                        cout << "\t  SET 7,H";
                        break;
                    case 0xFD:
                        cout << "\t  SET 7,L";
                        break;
                    case 0xFE:
                        cout << "\t  SET 7,(HL)";
                        break;
                    case 0xFF:
                        cout << "\t  SET 7,A";
                        break;
                }
                
                counter++;//Increase our counter, as we've read another byte from our source file
                //This way, our addresses going forward will be correct...

                break;
            case 0xCC:
                cout << "\t  CALL Z,a16";
                break;
            case 0xCD:
                cout << "\t  CALL a16";
                break;
            case 0xCE:
                cout << "\t  ADC A,d8";
                break;
            case 0xCF:
                cout << "\t  RST 08H";
                break;

            case 0xD0:
                cout << "\t  RET NC";
                break;
            case 0xD1:
                cout << "\t  POP DE";
                break;
            case 0xD2:
                cout << "\t  JP NC,a16";
                break;
            case 0xD3:
                cout << "\t  Unused";
                break;
            case 0xD4:
                cout << "\t  CALL NC,16";
                break;
            case 0xD5:
                cout << "\t  PUSH DE";
                break;
            case 0xD6:
                cout << "\t  SUB d8";
                break;
            case 0xD7:
                cout << "\t  RST 10H";
                break;
            case 0xD8:
                cout << "\t  RET C";
                break;
            case 0xD9:
                cout << "\t  RETI";
                break;
            case 0xDA:
                cout << "\t  JP C,a16";
                break;
            case 0xDB:
                cout << "\t  Unused";
                break;
            case 0xDC:
                cout << "\t  CALL C,a16";
                break;
            case 0xDD:
                cout << "\t  Unused";
                break;
            case 0xDE:
                cout << "\t  SBC A,d8";
                break;
            case 0xDF:
                cout << "\t  RST 18H";
                break;



            case 0xE0:
                cout << "\t  LDH (a8),A";
                break;
            case 0xE1:
                cout << "\t  POP HL";
                break;
            case 0xE2:
                cout << "\t  LD (C),A";
                break;
            case 0xE3:
                cout << "\t  Unused";
                break;
            case 0xE4:
                cout << "\t  Unused";
                break;
            case 0xE5:
                cout << "\t  PUSH HL\t\t\t\t\tPush the contents of HL onto the stack";
                break;
            case 0xE6: //AND d8
                byte1 = getNext8Bits(inputfile);
               
                print8BitLine("AND A", "Do the binary AND operation of this immediate value and A", " && A", byte1);


                cout << "\t  AND d8";
                break;
            case 0xE7:
                cout << "\t  RST 20H";
                break;
            case 0xE8:
                cout << "\t  ADD SP,r8";
                break;
            case 0xE9:
                cout << "\t  JP (HL)";
                break;
            case 0xEA:
                cout << "\t  LD (a16),A";
                break;
            case 0xEB:
                cout << "\t  Unused";
                break;
            case 0xEC:
                cout << "\t  Unused";
                break;
            case 0xED:
                cout << "\t  Unused";
                break;
            case 0xEE:
                cout << "\t  XOR d8";
                break;
            case 0xEF:
                cout << "\t  RST 28H";
                break;

            case 0xF0:
                cout << "\t  LDH A,(a8)";
                break;
            case 0xF1:
                cout << "\t  POP AF";
                break;
            case 0xF2:
                cout << "\t  LD A,(C)";
                break;
            case 0xF3:
                cout << "\t  DI";
                break;
            case 0xF4:
                cout << "\t  Unused";
                break;
            case 0xF5:
                cout << "\t  PUSH AF\t\t\t\tPush the contents of AF onto the stack";
                break;
            case 0xF6:
                cout << "\t  OR d8";
                break;
            case 0xF7:
                cout << "\t  RST 30H";
                break;
            case 0xF8:
                cout << "\t  LD HL,SP+r8";
                break;
            case 0xF9:
                cout << "\t  LD SP,HL";
                break;
            case 0xFA:
                cout << "\t  LD A,(a16)";
                break;
            case 0xFB:
                cout << "\t  EI";
                break;
            case 0xFC:
                cout << "\t  Unused";
                break;
            case 0xFD:
                cout << "\t  Unused";
                break;
            case 0xFE:
                cout << "\t  CP d8";
                break;
            case 0xFF:
                cout << "\t  RST 38H";
                break;
        }
        
        cout << endl;
        
        counter = counter+1;

	}
}


