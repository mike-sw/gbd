/*

This is a gameboy binary disassembler... technically for the Custom(-ized from the Z80) 8-bit Sharp LR35902

*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string.h>
#include <sstream>

using namespace std;

int main(int argc, char * argv[])
{
    if(argc<1)
    {
        cout << "Usage: emu filename.tzx" << endl;
        //exit();
    }
    cout << "Disassembling " << argv[1] << "...\n" << endl;
    
    

/*	for(int x=0; x<argc;x++)
	{
		cout << "Argument " << x << ": " << argv[x] << endl;
	}*/

    //Grab our first argument (our filename to open)
	ifstream inputfile (argv[1], ifstream::binary);
	
	char *buffer = new char [2]; //We're going to read this in 4 byte chunks
	
	inputfile.read(buffer, 6);
    cout << "**" << buffer << endl;

	
	//Let's get the length of the file
	inputfile.seekg(0, inputfile.end);
	int length = inputfile.tellg();
	inputfile.seekg(0, inputfile.beg);
	
	
	//OK cool, now we have a valid file
	int counter=0;
	while(!inputfile.eof())
	{
		inputfile.read(buffer,1);
		//inputfile.unsetf(ios_base::skipws);
		
        printf("%04x:\t", counter); //Print the address
        unsigned char temp = *buffer;
        printf("%02x", temp); //Print the opcode
        
        unsigned char thisByte = *buffer; //Get the current opcode by itself

        cout << "   " << (char)temp;
        
        /*
            Now, as we parse our opcodes, print what they are

        */

        switch(thisByte)
        {
            case 0x00:
                cout << "\t  NOP";
                break;
            case 0x01:
                cout << "\t  LD BC,d16";
                break;
            case 0x02:
                cout << "\t  LD (BC),A";
                break;
            case 0x03:
                cout << "\t  INC BC";
                break;
            case 0x04:
                cout << "\t  INC B";
                break;
            case 0x05:
                cout << "\t  DEC B\t\t\t\t\tDecrement Register";
                break;
            case 0x06:
                cout << "\t LD B,d8";
                break;
            case 0x07:
                cout << "\t  RLCA";
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
                cout << "\t  DEC BC";
                break;
            case 0x0C:
                cout << "\t  INC C";
                break;
            case 0x0D:
                cout << "\t  DEC C";
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
                cout << "\t  INC DE";
                break;
            case 0x14:
                cout << "\t  INC D";
                break;
            case 0x15:
                cout << "\t  DEC D";
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
                cout << "\t  DEC DE";
                break;
            case 0x1C:
                cout << "\t  INC E";
                break;
            case 0x1D:
                cout << "\t  DEC E";
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
                cout << "\t  INC HL";
                break;
            case 0x24:
                cout << "\t  INC H";
                break;
            case 0x25:
                cout << "\t  DEC H";
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
                cout << "\t  DEC HL";
                break;
            case 0x2C:
                cout << "\t  INC L";
                break;
            case 0x2D:
                cout << "\t  DEC L";
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
                cout << "\t  INC SP";
                break;
            case 0x34:
                cout << "\t  INC (HL)";
                break;
            case 0x35:
                cout << "\t  DEC (HL)";
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
                cout << "\t  DEC SP";
                break;
            case 0x3C:
                cout << "\t  INC A";
                break;
            case 0x3D:
                cout << "\t  DEC A";
                break;
            case 0x3E:
                cout << "\t  LD A,d8";
                break;
            case 0x3F:
                cout << "\t  CCF";
                break;

            case 0x40:
                cout << "\t  LD B,B";
                break;
            case 0x41:
                cout << "\t  LD B,C";
                break;
            case 0x42:
                cout << "\t  LD B,D";
                break;
            case 0x43:
                cout << "\t  LD B,E";
                break;
            case 0x44:
                cout << "\t  LD B,H";
                break;
            case 0x45:
                cout << "\t  LD B,L";
                break;
            case 0x46:
                cout << "\t  LD B,(HL)";
                break;
            case 0x47:
                cout << "\t  LD B,A";
                break;
            case 0x48:
                cout << "\t  LD C,B";
                break;
            case 0x49:
                cout << "\t  LD C,C";
                break;
            case 0x4A:
                cout << "\t  LD C,D";
                break;
            case 0x4B:
                cout << "\t  LD C,E";
                break;
            case 0x4C:
                cout << "\t  LD C,H";
                break;
            case 0x4D:
                cout << "\t  LD C,L";
                break;
            case 0x4E:
                cout << "\t  LD C,(HL)";
                break;
            case 0x4F:
                cout << "\t  LD C,A";
                break;

            case 0x50:
                cout << "\t  LD D,B";
                break;
            case 0x51:
                cout << "\t  LD D,C";
                break;
            case 0x52:
                cout << "\t  LD D,D";
                break;
            case 0x53:
                cout << "\t  LD D,E";
                break;
            case 0x54:
                cout << "\t  LD D,H";
                break;
            case 0x55:
                cout << "\t  LD D,L";
                break;
            case 0x56:
                cout << "\t  LD D,(HL)";
                break;
            case 0x57:
                cout << "\t  LD D,A";
                break;
            case 0x58:
                cout << "\t  LD E,B";
                break;
            case 0x59:
                cout << "\t  LD E,C";
                break;
            case 0x5A:
                cout << "\t  LD E,D";
                break;
            case 0x5B:
                cout << "\t  LD E,E";
                break;
            case 0x5C:
                cout << "\t  LD E,H";
                break;
            case 0x5D:
                cout << "\t  LD E,L";
                break;
            case 0x5E:
                cout << "\t  LD E,(HL)";
                break;
            case 0x5F:
                cout << "\t  LD E,A";
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

            
            
            /*case 0x:
                
                unsigned char byte1;
                char byte1temp;
                inputfile.get(byte1temp);
                
                unsigned char byte2;
                char byte2temp;
                inputfile.get(byte2temp);
                
                byte1=byte1temp;
                byte2=byte2temp;
                
                printf(" %02x %02x  JMP $%02x%02x", byte1, byte2, byte2, byte1 );
                //cout << " " << inputfile.peek() << " " << inputfile.peek() << " " << "\tJMP";
                counter = counter+2; //Because we pulled 2 bytes out
                break;*/
            
                
        }
        
        cout << endl;
        
        counter = counter+1;

	}
}


