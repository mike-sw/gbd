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

int convertHextoInt(string hexstr) {
    return (int)strtol(hexstr.c_str(), 0, 16);
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
    //cout << setfill('*') << setw(15) << instruction;
    cout << "\t\t" << instruction;
    printByteAsHex(byte1); //Print the value
    //cout << setfill('-') << setw(30);
    cout << "\t\t";

    //Print the value of these bytes into the description of this function
    printByteAsHex(byte1);
    cout << impactedRegister;
    //cout << setfill('+') << setw(45) << description;
    cout << "\t" << description;

    //Advance our addresses appropriately
    counter=counter+2;
}

void print16BitLine(string instruction, string description, string impactedRegister, unsigned char byte1, unsigned char byte2)
{
    //We have to swap bytes 1 and 2 because of endianness
    //cout << " ";
    printByteAsHex(byte1);
    printByteAsHex(byte2);
                
    //Print the value of these bytes into our function
    //cout << setfill('*') << setw(15) << instruction;
    cout << "\t\t" << instruction;
    printByteAsHex(byte2);
    printByteAsHex(byte1);
    //cout << setfill('-') << setw(30);
    cout << "\t";

    //Print the value of these bytes into the description of this function
    printByteAsHex(byte2);
    printByteAsHex(byte1);
    cout << impactedRegister;
    cout << "\t" << description;
    //cout << setfill('+') << setw(45) << description;

    //Advance our addresses appropriately
    counter=counter+3;
}

void printLine(string instruction, string description)
{
    //cout << setfill('*') << setw(15) << instruction;
    //cout << setfill('+') << setw(45) << description;

    cout << "\t\t" << instruction << "\t\t" << description;
    counter++;
}

bool isAddressSpecial(int address, unsigned char data) //It's worth noting that address is passed as a DECIMAL
{

    //This data provided by the EXCELLENT documentation here: http://gbdev.gg8.se/wiki/articles/The_Cartridge_Header
    if(address==256) //100h, our OEP
    {
        cout << "\t\t<-- Gameboy starts execution here" << endl;
        return true;
    }
    if(address>=260 && address<=307)//Our nintendo logo
    {
        cout << "\t\t<-- Nintendo logo" << endl;
        return true;
    }
    if(address>=308 && address<=323)
    {
        cout << "\t\t<-- Game Title" << endl;
        return true;
    }
    if(address==325)//Our manufacturer/publisher code
    {
        cout << "\t\t<-- Manufacturer and Publisher code" << endl;
        return true;
    }
    if(address==326) //Do we have super gameboy functionality?
    {
        cout << "\t\t<-- SGB Functionality (";
        printByteAsHex(data);

        //Read our provided data and determine what the functionality is
        switch(data)
        {
            case 0x00: //No
                cout << "= No)" << endl;
                break;
            case 0x03: //Yes
                cout << "= Yes)" << endl;
                break;
            default:
                cout << " = Unknown)" << endl;
                break;
        }
        return true;
    }
    if(address==327) //What type of cartridge is this?
    {
        cout << "\t\t<-- Cartridge type, Memory Bank Controller, and Misc Hardware (";

        switch(data)
        {
            case 0x00: //Rom only
                cout << "ROM Only)" << endl;
                break;
            case 0x01:
                cout << "MBC1)" << endl;
                break;
            case 0x02:
                cout << "MBC1+RAM)" << endl;
                break;
            case 0x03:
                cout << "MBC1+RAM+Battery)" << endl;
                break;

            case 0x05:
                cout << "MBC2)" << endl;
                break;
            case 0x06:
                cout << "MBC2+Battery)" << endl;
                break;
            
            case 0x08:
                cout << "ROM+RAM)" << endl;
                break;
            case 0x09:
                cout << "ROM+RAM+Battery)" << endl;
                break;
            case 0x0B:
                cout << "MMM01)" << endl;
                break;
            case 0x0C:
                cout << "MMM01+RAM)" << endl;
                break;
            case 0x0D:
                cout << "MMM01+RAM+Battery)" << endl;
                break;
            case 0x0F:
                cout << "MBC3+Timer+Battery)" << endl;
                break;
            case 0x10:
                cout << "MBC3+Timer+RAM+Battery)" << endl;
                break;
            case 0x11:
                cout << "MBC3)" << endl;
                break;
            case 0x12:
                cout << "MBC3+RAM)" << endl;
                break;
            case 0x13:
                cout << "MBC3+RAM+Battery)" << endl;
                break;
            case 0x19:
                cout << "MBC5)" << endl;
                break;
            case 0x1A:
                cout << "MBC5+RAM)" << endl;
                break;
            case 0x1B:
                cout << "MBC5+RAM+Battery)" << endl;
                break;
            case 0x1C:
                cout << "MBC5+Rumble)" << endl;
                break;
            case 0x1D:
                cout << "MBC5+Rumble+RAM)" << endl;
                break;
            case 0x1E:
                cout << "MBC5+Rumble+RAM+Battery)" << endl;
                break;
            case 0x20:
                cout << "MBC6)" << endl;
                break;
            case 0x22:
                cout << "MBC7+Sensor+Rumble+RAM+Battery)" << endl;
                break;
            case 0xFC:
                cout << "Gameboy Pocket Camera)" << endl;
                break;
            case 0xFD:
                cout << "Bandai Tama5)" << endl;
                break;
            case 0xFE:
                cout << "HuC3)" << endl;
                break;
            case 0xFF:
                cout << "HuC1+RAM+Battery)" << endl;
                break;
            default:
                cout << "Unknown)" << endl;
                break;
        }

        return true;
    }
    if(address==328) //How big is our cartridge?
    {
        //We can calculate this by grabbing the value, multiplying by 32, and adding 32

        cout << "\t\t<-- The size of the cartridge" << endl;
        return true;
    }
    if(address==329) //How much RAM is available via the cartridge?
    {
        cout << "\t\t<-- Amount of RAM on the Cartridge (";
        switch(data)
        {
            case 0x00:
                cout << "None)" << endl;
                break;
            case 0x01:
                cout << "2kb)" << endl;
                break;
            case 0x02:
                cout << "8kb)" << endl;
                break;
            case 0x03:
                cout << "32kb)" << endl;
                break;
            case 0x04:
                cout << "128kb)" << endl;
                break;
            case 0x05:
                cout << "64kb)" << endl;
                break;
            default:
                cout << "Unknown)" << endl;
                break;
        }
        return true;
    }
    if(address==330) //Geographic distribution of the cartridge, basically "Are we in Japan?"
    {
        cout << "\t\t<-- Distribution (";
        switch(data)
        {
            case 0x00:
                cout << "Japan)" << endl;
                break;
            case 0x01:
                cout << "Non-Japan)" << endl;
                break;
            default:
                cout << "Unknown)" << endl;
                break;
        }
        return true;
    }
    if(address==331) //Legacy publisher code. If we show 33, then we need to go above to determine the publisher
    {
        cout << "\t\t<-- Legacy publisher code (";
        switch(data)
        {
            case 0x33:
                cout << "See 0x0144-0x0155. This ROM is too new!)" << endl;
                break;
            default:
                cout << "Unknown)" << endl;
                break;
        }
        return true;
    }
    if(address==332) //ROM Version number. Usually 0, but it could be something else
    {
        cout << "\t\t<-- ROM Version number (";
        printByteAsHex(data);
        cout << ")" << endl;
        return true;
    }
    if(address==333) //header checksum of bytes 0x0134 through 0x014C
    {
        cout << "\t\t<-- Header (0x0134 - 0x014C) checksum (";
        printByteAsHex(data);
        cout << ")" << endl;
        return true;
    }
    if(address==334 || address==335) //Global ROM Checksum (unused)
    {
        cout << "\t\t<-- Full-ROM checksum (";
        printByteAsHex(data);
        cout << ")" << endl;
        return true;
    }
    
    return false;

}

int main(int argc, char * argv[])
{
    string startAssemblyAtAddressStr="0";
    int targetAddress=0;
    int stopAddress=9999999;
    if(argc<2)
    {
        cout << endl << "gbd - Gameboy Binary Disassembler" << endl << "https://github.com/mike-sw" << endl << endl;
        cout << "Usage: gbd filename.gb" << endl << endl;
        cout << "Arguments:" << endl << "-start - Start disassembling at a specific address" << endl;
        cout << "-stop - Stop disassembly at a specific address" << endl << endl;
        //exit();
    }
    else
    {
                cout << "Disassembling " << argv[1] << "..." << endl;

        //We may or may not have arguments following, but if we do, let's interpret them here
        if(argc>=2) //We have a file name, and an additional set of arguments
        {
            //OK, if we have more arguments, we want to step through them all
            for(int arg=2;arg<argc;arg++) //Start with the second argument (technically the 3rd...)
            {
                string argument(argv[arg]);

                if(argument=="-a" || argument=="-start") //Start disassembly at this address
                {
                    startAssemblyAtAddressStr=argv[arg+1];
                    //cout << "arg = " << argv[arg+1] << endl;
                    //The user's given us their desired address in hex, so let's convert it to decimal for our counter

                    targetAddress=convertHextoInt(argv[arg+1]); //Now counter holds the appropriate decimal number
                    cout << "Starting at 0x" << startAssemblyAtAddressStr << "..." << endl;


                }
                if(argument=="-stop")
                {
                    stopAddress=convertHextoInt(argv[arg+1]);
                    cout << "Stopping at 0x" << argv[arg+1] << "..." << endl;
                }
            }
        }

        //Now let's do a little error catching to let the user know that they're asking for something unreasonable
        if(targetAddress>stopAddress) //The user specifies a start address which is after the stop address
        {
            cout << "Warning: We can't stop disassembly before we've started! (targetAddress>stopAddress)" << endl;
        }

        cout << endl;

        //counter = stoi(startAssemblyAtAddressStr);

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
            if(counter >= targetAddress && counter <= stopAddress)
            {
                cout << setw(4) << setfill('0') << hex << counter << ":" << setw(1); //Print our current address in the left margin

                *buffer = getNext8Bits(inputfile); //Read a byte from the file
                printByteAsHex(*buffer);//Print our opcode
                
                unsigned char thisByte = *buffer; //Get the current opcode by itself

                //We only want to print opcodes as if they're ASCII if they actually fall in the appropriate range
                //We don't want to try to print the unprintable
                if(*buffer >= 32 && *buffer<=126) cout << "   " << (char)*buffer;
                

                /*

                    Gameboy .gb files are basically images of the cartridge. Like Windows PE files, these are composite formats,
                    with lots of moving parts. There's some interesting things to note about how these .gb files are built.

                    The OEP (Original Entry Point) for a gameboy game is 0x0100. This is where we start execution.

                    Addresses 0x0104 - 0x0133 is the bytes that make up the Nintendo startup logo

                    We want to note these in our disassembler output, and not disassemble them!

                */

                if(isAddressSpecial(counter, thisByte))
                {
                    counter++;
                }
                else
                {

                    /*
                        Now, as we parse our opcodes, print what they are

                    */

                    switch(*buffer)
                    {
                        case 0x00:
                            printLine("NOP", "No Operation");
                            break;
                        case 0x01: //LD BC,d16
                           
                            //Since this is a 16-bit function, pull both arguments
                           byte1 = getNext8Bits(inputfile);
                           byte2 = getNext8Bits(inputfile);
                           
                            print16BitLine("LD BC", "Load a 16-bit immediate value into registers BC", "->BC", byte1, byte2);

                            break;
                        case 0x02:
                            printLine("LD (BC),A","Load an 8-bit value from A into 16-bit register BC");
                            break;
                        case 0x03:
                            printLine("INC BC", "Increment Register");
                            //cout << "INC BC\t\t\t\t\tIncrement Register";
                            break;
                        case 0x04:
                            printLine("INC B","Increment Register");
                            break;
                        case 0x05:
                            printLine("DEC B", "Decrement Register");
                            break;
                        case 0x06: //LD B, d8

                            byte1 = getNext8Bits(inputfile);
                           
                            print8BitLine("LD B", "Load a 8-bit immediate value into register B", "->B", byte1);

                            break;
                        case 0x07: //RCLA, check here for more info: https://hax.iimarckus.org/topic/1617/
                            printLine("RLCA","Rotate Left, carry A");
                            break;
                        case 0x08:
                            printLine("LD (a16),SP", "");
                            break;
                        case 0x09:
                            printLine("ADD HL, BC", "Add the values in registers HL and BC");
                            break;
                        case 0x0A:
                            printLine("LD A,(BC)", "");
                            break;
                        case 0x0B:
                            printLine("DEC BC","Decrement Register");
                            break;
                        case 0x0C:
                            printLine("INC C","Increment Register");
                            break;
                        case 0x0D:
                            printLine("DEC C","Decrement Register");
                            break;
                        case 0x0E:
                            byte1 = getNext8Bits(inputfile);
                           
                            print8BitLine("LD c,", "Load a 8-bit immediate value into register C", "->C", byte1);
                           // printLine("LD C,d8", "");
                            break;
                        case 0x0F:
                            printLine("RRCA","");
                            break;

                        case 0x10:
                            printLine("STOP 0","Stop the CPU");
                            break;
                        case 0x11:
                            byte1 = getNext8Bits(inputfile);
                            byte2 = getNext8Bits(inputfile);
                           
                            print16BitLine("LD DE,", "Load a 16-bit immediate value into registers DE", "->DE", byte1,byte2);

                            //printLine("LD DE,d16", "");
                            break;
                        case 0x12:
                            printLine("LD (DE),A","");
                            break;
                        case 0x13:
                            printLine("INC DE","Increment Register");
                            break;
                        case 0x14:
                            printLine("INC D","Increment Register");
                            break;
                        case 0x15:
                            printLine("DEC D", "Decrement Register");
                            break;
                        case 0x16:
                            byte1 = getNext8Bits(inputfile);
                           
                            print8BitLine("LD D,", "Load a 8-bit immediate value into register D", "->D", byte1);
                            //printLine("LD D,d8", "");
                            break;
                        case 0x17:
                            printLine("RLA","");
                            break;
                        case 0x18:
                            printLine("JR r8", "");
                            break;
                        case 0x19:
                            printLine("ADD HL,DE","");
                            break;
                        case 0x1A:
                            printLine("LD A,(DE)","");
                            break;
                        case 0x1B:
                            printLine("DEC DE", "Decrement Register");
                            break;
                        case 0x1C:
                            printLine("INC E", "Increment Register");
                            break;
                        case 0x1D:
                            printLine("DEC E", "Decrement Register");
                            break;
                        case 0x1E:
                            //printLine("LD E,d8", "");

                            byte1 = getNext8Bits(inputfile);
                           
                            print8BitLine("LD E,", "Load a 8-bit immediate value into register E", "->E", byte1);

                            break;
                        case 0x1F:
                            printLine("RRA","");
                            break;

                        case 0x20:
                            printLine("JR NZ,r8", "");
                            break;
                        case 0x21:
                            //printLine("LD HL,d16", "");

                            byte1 = getNext8Bits(inputfile);
                            byte2 = getNext8Bits(inputfile);
                           
                            print16BitLine("LD HL,", "Load a 16-bit immediate value into registers HL", "->HL", byte1,byte2);

                            break;
                        case 0x22:
                            printLine("LD(HL+),A","");
                            break;
                        case 0x23:
                            printLine("INC HL","Increment Register");
                            break;
                        case 0x24:
                            printLine("INC H","Increment Register");
                            break;
                        case 0x25:
                            printLine("DEC H","Decrement Register");
                            break;
                        case 0x26:
                            //printLine("LD H,d8", "");

                            byte1 = getNext8Bits(inputfile);
                           
                            print8BitLine("LD H,", "Load a 8-bit immediate value into register H", "->H", byte1);
                            break;
                        case 0x27:
                            printLine("DAA","Converts A into packed BCD");
                            break;
                        case 0x28:
                            printLine("JR Z,r8", "");
                            break;
                        case 0x29:
                            printLine("ADD HL,HL","");
                            break;
                        case 0x2A:
                            printLine("LD A,(HL+)","");
                            break;
                        case 0x2B:
                            printLine("DEC HL","Decrement Register");
                            break;
                        case 0x2C:
                            printLine("INC L","Increment Register");
                            break;
                        case 0x2D:
                            printLine("DEC L","Decrement Register");
                            break;
                        case 0x2E:
                            byte1 = getNext8Bits(inputfile);
                           
                            print8BitLine("LD L,", "Load a 8-bit immediate value into register L", "->L", byte1);

                            //printLine("LD L,d8", "");
                            break;
                        case 0x2F:
                            printLine("CPL","Complement A");
                            break;

                        case 0x30:
                            printLine("JR NC,r8", "");
                            break;
                        case 0x31:
                            //printLine("LD SP,d16", "");

                            byte1 = getNext8Bits(inputfile);
                            byte2 = getNext8Bits(inputfile);
                           
                            print16BitLine("LD HL,", "Load a 16-bit immediate value into registers HL", "->HL", byte1,byte2);

                            break;
                        case 0x32:
                            printLine("LD (HL-),A","");
                            break;
                        case 0x33:
                            printLine("INC SP","Increment Register");
                            break;
                        case 0x34:
                            printLine("INC (HL)","Increment Register");
                            break;
                        case 0x35:
                            printLine("DEC (HL)","Decrement Register");
                            break;
                        case 0x36:
                            printLine("LD (HL),d8", "");
                            break;
                        case 0x37:
                            printLine("SCF","");
                            break;
                        case 0x38:
                            printLine("JR C,r8", "");
                            break;
                        case 0x39:
                            printLine("ADD HL,SP","");
                            break;
                        case 0x3A:
                            printLine("LD A,(HL-)","");
                            break;
                        case 0x3B:
                            printLine("DEC SP","Decrement Register");
                            break;
                        case 0x3C:
                            printLine("INC A","Increment Register");
                            break;
                        case 0x3D:
                            printLine("DEC A","Decrement Register");
                            break;
                        case 0x3E:
                            //printLine("LD A,d8", "");

                            byte1 = getNext8Bits(inputfile);
                           
                            print8BitLine("LD A,", "Load a 8-bit immediate value into register A", "->A", byte1);
                            break;
                        case 0x3F:
                            printLine("CCF","");
                            break;

                        case 0x40:
                            printLine("LD B,B","B->B\tCopy register B into itself, usually used as a flag reset");
                            break;
                        case 0x41:
                            printLine("LD B,C","C->B\tCopy the value in register C into register B");
                            break;
                        case 0x42:
                            printLine("LD B,D","D->B\tCopy the value in register D into register B");
                            break;
                        case 0x43:
                            printLine("LD B,E","E->B\tCopy the value in register E into register B");
                            break;
                        case 0x44:
                            printLine("LD B,H","H->B\tCopy the value in register H into register B");
                            break;
                        case 0x45:
                            printLine("LD B,L","L->B\tCopy the value in register L into register B");
                            break;
                        case 0x46:
                            printLine("LD B,(HL)","HL->B\tCopy the value in register HL into register B");
                            break;
                        case 0x47:
                            printLine("LD B,A","A->B\tCopy the value in register A into register B");
                            break;
                        case 0x48:
                            printLine("LD C,B","B->C\tCopy the value in register B into register C");
                            break;
                        case 0x49:
                            printLine("LD C,C","C->C\tCopy register C into itself, usually used as a flag reset");
                            break;
                        case 0x4A:
                            printLine("LD C,D","D->C\tCopy the value in register D into register C");
                            break;
                        case 0x4B:
                            printLine("LD C,E","E->C\tCopy the value in register E into register C");
                            break;
                        case 0x4C:
                            printLine("LD C,H","H->C\tCopy the value in register H into register C");
                            break;
                        case 0x4D:
                            printLine("LD C,L","L->C\tCopy the value in register L into register C");
                            break;
                        case 0x4E:
                            printLine("LD C,(HL)","HL->C\tCopy the value in register HL into register C");
                            break;
                        case 0x4F:
                            printLine("LD C,A","A->C\tCopy the value in register A into register C");
                            break;

                        case 0x50:
                            printLine("LD D,B","B->D\tCopy the value in register B into register D");
                            break;
                        case 0x51:
                            printLine("LD D,C","C->D\tCopy the value in register C into register D");
                            break;
                        case 0x52:
                            printLine("LD D,D","D->D\tCopy register D into itself, usually used as a flag reset");
                            break;
                        case 0x53:
                            printLine("LD D,E","E->D\tCopy the value in register E into register D");
                            break;
                        case 0x54:
                            printLine("LD D,H","H->D\tCopy the value in register H into register D");
                            break;
                        case 0x55:
                            printLine("LD D,L","L->D\tCopy the value in register L into register D");
                            break;
                        case 0x56:
                            printLine("LD D,(HL)","HL->D\tCopy the value in register HL into register D");
                            break;
                        case 0x57:
                            printLine("LD D,A","A->D\tCopy the value in register A into register D");
                            break;
                        case 0x58:
                            printLine("LD E,B","B->E\tCopy the value in register B into register E");
                            break;
                        case 0x59:
                            printLine("LD E,C","C->E\tCopy the value in register C into register E");
                            break;
                        case 0x5A:
                            printLine("LD E,D","E->E\tCopy the value in register E into register E");
                            break;
                        case 0x5B:
                            printLine("LD E,E","E->E\tCopy register E into itself, usually used as a flag reset");
                            break;
                        case 0x5C:
                            printLine("LD E,H","H->E\tCopy the value in register H into register E");
                            break;
                        case 0x5D:
                            printLine("LD E,L","L->E\tCopy the value in register L into register E");
                            break;
                        case 0x5E:
                            printLine("LD E,(HL)","HL->E\tCopy the value in register HL into register E");
                            break;
                        case 0x5F:
                            printLine("LD E,A","A->E\tCopy the value in register A into register E");
                            break;

                        case 0x60:
                            printLine("LD H,B","B->H\tCopy the value in register B into register H");
                            break;
                        case 0x61:
                            printLine("LD H,C","C->H\tCopy the value in register C into register H");
                            break;
                        case 0x62:
                            printLine("LD H,D","D->H\tCopy the value in register D into register H");
                            break;
                        case 0x63:
                            printLine("LD H,B","B->H\tCopy the value in register B into register H");
                            break;
                        case 0x64:
                            printLine("LD H,H","H->H\tCopy the value in register H into itself");
                            break;
                        case 0x65:
                            printLine("LD H,L","L->H\tCopy the value in register L into register H");
                            break;
                        case 0x66:
                            printLine("LD H,(HL)","");
                            break;
                        case 0x67:
                            printLine("LD H,A","A->H\tCopy the value in register A into register H");
                            break;
                        case 0x68:
                            printLine("LD L,B","B->H\tCopy the value in register B into register H");
                            break;
                        case 0x69:
                            printLine("LD L,C","C->L\tCopy the value in register C into register L");
                            break;
                        case 0x6A:
                            printLine("LD L,D","D->L\tCopy the value in register D into register L");
                            break;
                        case 0x6B:
                            printLine("LD L,E","E->L\tCopy the value in register E into register L");
                            break;
                        case 0x6C:
                            printLine("LD L,H","H->L\tCopy the value in register H into register L");
                            break;
                        case 0x6D:
                            printLine("LD L,L","L->L\tCopy the value in register D into itself");
                            break;
                        case 0x6E:
                            printLine("LD L,(HL)","");
                            break;
                        case 0x6F:
                            printLine("LD L,A","A->L\tCopy the value in register A into register L");
                            break;
                        case 0x70:
                            printLine("LD (HL),B","");
                            break;
                        case 0x71:
                            printLine("LD (HL),C","");
                            break;
                        case 0x72:
                            printLine("LD (HL),D","");
                            break;
                        case 0x73:
                            printLine("LD (HL),E","");
                            break;
                        case 0x74:
                            printLine("LD (HL),H","");
                            break;
                        case 0x75:
                            printLine("LD (HL),L","");
                            break;
                        case 0x76:
                            printLine("HALT","Pause the CPU until an interrupt occurs");
                            break;
                        case 0x77:
                            printLine("LD (HL),A","");
                            break;
                        case 0x78:
                            printLine("LD A,B","B->A\tCopy the value in register B into register A");
                            break;
                        case 0x79:
                            printLine("LD A,C","C->A\tCopy the value in register C into register A");
                            break;
                        case 0x7A:
                            printLine("LD A,D","D->A\tCopy the value in register D into register A");
                            break;
                        case 0x7B:
                            printLine("LD A,E","E->A\tCopy the value in register E into register A");
                            break;
                        case 0x7C:
                            printLine("LD A,H","H->A\tCopy the value in register H into register A");
                            break;
                        case 0x7D:
                            printLine("LD A,L","L->A\tCopy the value in register L into register A");
                            break;
                        case 0x7E:
                            printLine("LD A,(HL)","");
                            break;
                        case 0x7F:
                            printLine("LD A,A","C->A\tCopy the value in register C into itself");
                            break;
                        case 0x80:
                            printLine("ADD A,B","A+B\tAdd the values in registers A and B, with the result stored in A");
                            break;
                        case 0x81:
                            printLine("ADD A,C","A+C\tAdd the values in registers A and C, with the result stored in A");
                            break;
                        case 0x82:
                            printLine("ADD A,D","A+D\tAdd the values in registers A and D, with the result stored in A");
                            break;
                        case 0x83:
                            printLine("ADD A,E","A+E\tAdd the values in registers A and E, with the result stored in A");
                            break;
                        case 0x84:
                            printLine("ADD A,H","A+H\tAdd the values in registers A and H, with the result stored in A");
                            break;
                        case 0x85:
                            printLine("ADD A,L","A+L\tAdd the values in registers A and L, with the result stored in A");
                            break;
                        case 0x86:
                            printLine("ADD A,(HL)","A+HL\tAdd the values in registers A and HL, with the result stored in A");
                            break;
                        case 0x87:
                            printLine("ADD A,A","A+A\tAdd the values in registers A and A, with the result stored in A");
                            break;
                        case 0x88:
                            printLine("ADC A,B","A+B\tAdd A+B, and carry flag to A");
                            break;
                        case 0x89:
                            printLine("ADC A,C","A+B\tAdd A+C, and carry flag to A");
                            break;
                        case 0x8A:
                            printLine("ADC A,D","A+B\tAdd A+D, and carry flag to A");
                            break;
                        case 0x8B:
                            printLine("ADC A,E","A+B\tAdd A+E, and carry flag to A");
                            break;
                        case 0x8C:
                            printLine("ADC A,H","A+B\tAdd A+H, and carry flag to A");
                            break;
                        case 0x8D:
                            printLine("ADC A,L","A+B\tAdd A+L, and carry flag to A");
                            break;
                        case 0x8E:
                            printLine("ADC A,(HL)","A+B\tAdd A+HL, and carry flag to A");
                            break;
                        case 0x8F:
                            printLine("ADC A,A","A+B\tAdd A+A, and carry flag to A");
                            break;

                        case 0x90:
                            printLine("SUB B","");
                            break;
                        case 0x91:
                            printLine("SUB C","");
                            break;
                        case 0x92:
                            printLine("SUB D","");
                            break;
                        case 0x93:
                            printLine("SUB E","");
                            break;
                        case 0x94:
                            printLine("SUB H","");
                            break;
                        case 0x95:
                            printLine("SUB L","");
                            break;
                        case 0x96:
                            printLine("SUB (HL)","");
                            break;
                        case 0x97:
                            printLine("SUB A","");
                            break;
                        case 0x98:
                            printLine("SBC A,B","");
                            break;
                        case 0x99:
                            printLine("SBC A,C","");
                            break;
                        case 0x9A:
                            printLine("SBC A,D","");
                            break;
                        case 0x9B:
                            printLine("SBC A,E","");
                            break;
                        case 0x9C:
                            printLine("SBC A,H","");
                            break;
                        case 0x9D:
                            printLine("SBC A,L","");
                            break;
                        case 0x9E:
                            printLine("SBC A,(HL)","");
                            break;
                        case 0x9F:
                            printLine("SBC A,A","");
                            break;

                        case 0xA0:
                            printLine("AND B","B && A\tDo the binary AND operation against B and A");
                            break;
                        case 0xA1:
                            printLine("AND C","C && A\tDo the binary AND operation against C and A");
                            break;
                        case 0xA2:
                            printLine("AND D","D && A\tDo the binary AND operation against D and A");
                            break;
                        case 0xA3:
                            printLine("AND E","E && A\tDo the binary AND operation against E and A");
                            break;
                        case 0xA4:
                            printLine("AND H","H && A\tDo the binary AND operation against H and A");
                            break;
                        case 0xA5:
                            printLine("AND L","L && A\tDo the binary AND operation against L and A");
                            break;
                        case 0xA6:
                            printLine("AND (HL)","HL && A\tDo the binary AND operation against HL and A");
                            break;
                        case 0xA7: //AND A               
                            printLine("AND A","A && A\tDo the binary AND operation against A and A");
                            break;
                        case 0xA8:
                            printLine("XOR B","A XOR B\tXor the values in registers A and B, with the result stored in A");
                            break;
                        case 0xA9:
                            printLine("XOR C","A XOR C\tXor the values in registers A and C, with the result stored in A");
                            break;
                        case 0xAA:
                            printLine("XOR D","A XOR D\tXor the values in registers A and D, with the result stored in A");
                            break;
                        case 0xAB:
                            printLine("XOR E","A XOR E\tXor the values in registers A and E, with the result stored in A");
                            break;
                        case 0xAC:
                            printLine("XOR H","A XOR H\tXor the values in registers A and H, with the result stored in A");
                            break;
                        case 0xAD:
                            printLine("XOR L","A XOR L\tXor the values in registers A and L, with the result stored in A");
                            break;
                        case 0xAE:
                            printLine("XOR (HL)","A XOR HL\tXor the values in registers A and HL, with the result stored in A");
                            break;
                        case 0xAF:
                            printLine("XOR A","A XOR A\tXor the values in register A against itself. A now equals zero.");
                            break;

                        case 0xB0:
                            printLine("OR B","A || B\tOr the values in registers A and B, with the result stored in A");
                            break;
                        case 0xB1:
                            printLine("OR C","A || C\tOr the values in registers A and C, with the result stored in A");
                            break;
                        case 0xB2:
                            printLine("OR D","A || D\tOr the values in registers A and D, with the result stored in A");
                            break;
                        case 0xB3:
                            printLine("OR E","A || E\tOr the values in registers A and E, with the result stored in A");
                            break;
                        case 0xB4:
                            printLine("OR H","A || H\tOr the values in registers A and H, with the result stored in A");
                            break;
                        case 0xB5:
                            printLine("OR L","A || L\tOr the values in registers A and L, with the result stored in A");
                            break;
                        case 0xB6:
                            printLine("OR (HL)","A || HL\tOr the values in registers A and HL, with the result stored in A");
                            break;
                        case 0xB7:
                            printLine("OR A","A || A\tA OR'd by itself is just the value of A. This is used to clear flags.");
                            break;
                        case 0xB8:
                            printLine("CP B","if(A==B)\tCompare A to B. Essentially do A-B, and see if ZF is set");
                            break;
                        case 0xB9:
                            printLine("CP C","if(A==C)\tCompare A to C. Essentially do A-C, and see if ZF is set");
                            break;
                        case 0xBA:
                            printLine("CP D","if(A==D)\tCompare A to D. Essentially do A-D, and see if ZF is set");
                            break;
                        case 0xBB:
                            printLine("CP E","if(A==E)\tCompare A to E. Essentially do A-E, and see if ZF is set");
                            break;
                        case 0xBC:
                            printLine("CP H","if(A==H)\tCompare A to H. Essentially do A-H, and see if ZF is set");
                            break;
                        case 0xBD:
                            printLine("CP L","if(A==L)\tCompare A to L. Essentially do A-L, and see if ZF is set");
                            break;
                        case 0xBE:
                            printLine("CP (HL)","if(A==HL)\tCompare A to HL. Essentially do A-HL, and see if ZF is set");
                            break;
                        case 0xBF:
                            printLine("CP A","if(A==A)\tCompare A to A. Essentially do A-A. ZF will always be set, as A-A=0");
                            break;

                        case 0xC0:
                            printLine("RET NZ","");
                            break;
                        case 0xC1:
                            printLine("POP BC","Pop two bytes off of the stack, and store them in BC");
                            break;
                        case 0xC2:
                            printLine("JP NZ,a16","");
                            break;
                        case 0xC3: //JP a16
                            byte1 = getNext8Bits(inputfile);
                            byte2 = getNext8Bits(inputfile);
                           
                            print16BitLine("JP ", "Jump to the specified 16-bit address", "->PC", byte1, byte2);

                            //printLine("JP a16", "");
                            break;
                        case 0xC4:
                            printLine("CALL NZ,a16", "");
                            break;
                        case 0xC5:
                            printLine("PUSH BC","BC->Stack\tPush this register pair onto the stack");
                            break;
                        case 0xC6:
                            printLine("ADD A,d8", "");
                            break;
                        case 0xC7:
                            printLine("RST 00H\t\t\t\t\tCall the code at 00",""); //According to https://www.cemetech.net/forum/viewtopic.php?t=6744
                            break;
                        case 0xC8:
                            printLine("RET Z","Return if the Zero flag is set");
                            break;
                        case 0xC9:
                            printLine("RET","Pop two bytes from the stack and jump to that address");
                            break;
                        case 0xCA:
                            printLine("JP Z,a16", "");
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
                                    printLine("RLC B","");
                                    break;
                                case 0x01:
                                    printLine("RLC C","");
                                    break;
                                case 0x02:
                                    printLine("RLC D","");
                                    break;
                                case 0x03:
                                    printLine("RLC E","");
                                    break;
                                case 0x04:
                                    printLine("RLC H","");
                                    break;
                                case 0x05:
                                    printLine("RLC L","");
                                    break;
                                case 0x06:
                                    printLine("RLC (HL)","");
                                    break;
                                case 0x07:
                                    printLine("RLC A","");
                                    break;
                                case 0x08:
                                    printLine("RRC B","");
                                    break;
                                case 0x09:
                                    printLine("RRC C","");
                                    break;
                                case 0x0A:
                                    printLine("RRC D","");
                                    break;
                                case 0x0B:
                                    printLine("RRC E","");
                                    break;
                                case 0x0C:
                                    printLine("RRC H","");
                                    break;
                                case 0x0D:
                                    printLine("RRC L","");
                                    break;
                                case 0x0E:
                                    printLine("RRC (HL)","");
                                    break;
                                case 0x0F:
                                    printLine("RRC A","");
                                    break;

                                case 0x10:
                                    printLine("RL B","");
                                    break;
                                case 0x11:
                                    printLine("RL C","");
                                    break;
                                case 0x12:
                                    printLine("RL D","");
                                    break;
                                case 0x13:
                                    printLine("RL E","");
                                    break;
                                case 0x14:
                                    printLine("RL H","");
                                    break;
                                case 0x15:
                                    printLine("RLC L","");
                                    break;
                                case 0x16:
                                    printLine("RLC (HL)","");
                                    break;
                                case 0x17:
                                    printLine("RL A","");
                                    break;
                                case 0x18:
                                    printLine("RR B","");
                                    break;
                                case 0x19:
                                    printLine("RR C","");
                                    break;
                                case 0x1A:
                                    printLine("RR D","");
                                    break;
                                case 0x1B:
                                    printLine("RR E","");
                                    break;
                                case 0x1C:
                                    printLine("RR H","");
                                    break;
                                case 0x1D:
                                    printLine("RR L","");
                                    break;
                                case 0x1E:
                                    printLine("RR (HL)","");
                                    break;
                                case 0x1F:
                                    printLine("RR A","");
                                    break;

                                case 0x20:
                                    printLine("SLA B","");
                                    break;
                                case 0x21:
                                    printLine("SLA C","");
                                    break;
                                case 0x22:
                                    printLine("SLA D","");
                                    break;
                                case 0x23:
                                    printLine("SLA E","");
                                    break;
                                case 0x24:
                                    printLine("SLA H","");
                                    break;
                                case 0x25:
                                    printLine("SLA L","");
                                    break;
                                case 0x26:
                                    printLine("SLA (HL)","");
                                    break;
                                case 0x27:
                                    printLine("SLA A","");
                                    break;
                                case 0x28:
                                    printLine("SRA B","");
                                    break;
                                case 0x29:
                                    printLine("SRA C","");
                                    break;
                                case 0x2A:
                                    printLine("SRA D","");
                                    break;
                                case 0x2B:
                                    printLine("SRA E","");
                                    break;
                                case 0x2C:
                                    printLine("SRA H","");
                                    break;
                                case 0x2D:
                                    printLine("SRA L","");
                                    break;
                                case 0x2E:
                                    printLine("SRA (HL)","");
                                    break;
                                case 0x2F:
                                    printLine("SRA A","");
                                    break;

                                case 0x30:
                                    printLine("SWAP B","");
                                    break;
                                case 0x31:
                                    printLine("SWAP C","");
                                    break;
                                case 0x32:
                                    printLine("SWAP D","");
                                    break;
                                case 0x33:
                                    printLine("SWAP E","");
                                    break;
                                case 0x34:
                                    printLine("SWAP H","");
                                    break;
                                case 0x35:
                                    printLine("SWAP L","");
                                    break;
                                case 0x36:
                                    printLine("SWAP (HL)","");
                                    break;
                                case 0x37:
                                    printLine("SWAP A","");
                                    break;
                                case 0x38:
                                    printLine("SRL B","");
                                    break;
                                case 0x39:
                                    printLine("SRL C","");
                                    break;
                                case 0x3A:
                                    printLine("SRL D","");
                                    break;
                                case 0x3B:
                                    printLine("SRL E","");
                                    break;
                                case 0x3C:
                                    printLine("SRL H","");
                                    break;
                                case 0x3D:
                                    printLine("SRL L","");
                                    break;
                                case 0x3E:
                                    printLine("SRL (HL)","");
                                    break;
                                case 0x3F:
                                    printLine("SRL A","");
                                    break;

                                case 0x40:
                                    printLine("BIT 0,B","");
                                    break;
                                case 0x41:
                                    printLine("BIT 0,C","");
                                    break;
                                case 0x42:
                                    printLine("BIT 0,D","");
                                    break;
                                case 0x43:
                                    printLine("BIT 0,E","");
                                    break;
                                case 0x44:
                                    printLine("BIT 0,H","");
                                    break;
                                case 0x45:
                                    printLine("BIT 0,L","");
                                    break;
                                case 0x46:
                                    printLine("BIT 0,(HL)","");
                                    break;
                                case 0x47:
                                    printLine("BIT 0,A","");
                                    break;
                                case 0x48:
                                    printLine("BIT 1,B","");
                                    break;
                                case 0x49:
                                    printLine("BIT 1,C","");
                                    break;
                                case 0x4A:
                                    printLine("BIT 1,D","");
                                    break;
                                case 0x4B:
                                    printLine("BIT 1,E","");
                                    break;
                                case 0x4C:
                                    printLine("BIT 1,H","");
                                    break;
                                case 0x4D:
                                    printLine("BIT 1,L","");
                                    break;
                                case 0x4E:
                                    printLine("BIT 1,(HL)","");
                                    break;
                                case 0x4F:
                                    printLine("BIT 1,A","");
                                    break;

                                case 0x50:
                                    printLine("BIT 2,B","");
                                    break;
                                case 0x51:
                                    printLine("BIT 2,C","");
                                    break;
                                case 0x52:
                                    printLine("BIT 2,D","");
                                    break;
                                case 0x53:
                                    printLine("BIT 2,E","");
                                    break;
                                case 0x54:
                                    printLine("BIT 2,H","");
                                    break;
                                case 0x55:
                                    printLine("BIT 2,L","");
                                    break;
                                case 0x56:
                                    printLine("BIT 2,(HL)","");
                                    break;
                                case 0x57:
                                    printLine("BIT 2,A","");
                                    break;
                                case 0x58:
                                    printLine("BIT 3,B","");
                                    break;
                                case 0x59:
                                    printLine("BIT 3,C","");
                                    break;
                                case 0x5A:
                                    printLine("BIT 3,D","");
                                    break;
                                case 0x5B:
                                    printLine("BIT 3,E","");
                                    break;
                                case 0x5C:
                                    printLine("BIT 3,H","");
                                    break;
                                case 0x5D:
                                    printLine("BIT 3,L","");
                                    break;
                                case 0x5E:
                                    printLine("BIT 3,(HL)","");
                                    break;
                                case 0x5F:
                                    printLine("BIT 3,A","");
                                    break;

                                case 0x60:
                                    printLine("BIT 4,B","");
                                    break;
                                case 0x61:
                                    printLine("BIT 4,C","");
                                    break;
                                case 0x62:
                                    printLine("BIT 4,D","");
                                    break;
                                case 0x63:
                                    printLine("BIT 4,E","");
                                    break;
                                case 0x64:
                                    printLine("BIT 4,H","");
                                    break;
                                case 0x65:
                                    printLine("BIT 4,L","");
                                    break;
                                case 0x66:
                                    printLine("BIT 4,(HL)","");
                                    break;
                                case 0x67:
                                    printLine("BIT 4,A","");
                                    break;
                                case 0x68:
                                    printLine("BIT 5,B","");
                                    break;
                                case 0x69:
                                    printLine("BIT 5,C","");
                                    break;
                                case 0x6A:
                                    printLine("BIT 5,D","");
                                    break;
                                case 0x6B:
                                    printLine("BIT 5,E","");
                                    break;
                                case 0x6C:
                                    printLine("BIT 5,H","");
                                    break;
                                case 0x6D:
                                    printLine("BIT 5,L","");
                                    break;
                                case 0x6E:
                                    printLine("BIT 5,(HL)","");
                                    break;
                                case 0x6F:
                                    printLine("BIT 5,A","");
                                    break;

                                case 0x70:
                                    printLine("BIT 6,B","");
                                    break;
                                case 0x71:
                                    printLine("BIT 6,C","");
                                    break;
                                case 0x72:
                                    printLine("BIT 6,D","");
                                    break;
                                case 0x73:
                                    printLine("BIT 6,E","");
                                    break;
                                case 0x74:
                                    printLine("BIT 6,H","");
                                    break;
                                case 0x75:
                                    printLine("BIT 6,L","");
                                    break;
                                case 0x76:
                                    printLine("BIT 6,(HL)","");
                                    break;
                                case 0x77:
                                    printLine("BIT 6,A","");
                                    break;
                                case 0x78:
                                    printLine("BIT 7,B","");
                                    break;
                                case 0x79:
                                    printLine("BIT 7,C","");
                                    break;
                                case 0x7A:
                                    printLine("BIT 7,D","");
                                    break;
                                case 0x7B:
                                    printLine("BIT 7,E","");
                                    break;
                                case 0x7C:
                                    printLine("BIT 7,H","");
                                    break;
                                case 0x7D:
                                    printLine("BIT 7,L","");
                                    break;
                                case 0x7E:
                                    printLine("BIT 7,(HL)","");
                                    break;
                                case 0x7F:
                                    printLine("BIT 7,A","");
                                    break;

                                case 0x80:
                                    printLine("RES 0,B","");
                                    break;
                                case 0x81:
                                    printLine("RES 0,C","");
                                    break;
                                case 0x82:
                                    printLine("RES 0,D","");
                                    break;
                                case 0x83:
                                    printLine("RES 0,E","");
                                    break;
                                case 0x84:
                                    printLine("RES 0,H","");
                                    break;
                                case 0x85:
                                    printLine("RES 0,L","");
                                    break;
                                case 0x86:
                                    printLine("RES 0,(HL)","");
                                    break;
                                case 0x87:
                                    printLine("RES 0,A","");
                                    break;
                                case 0x88:
                                    printLine("RES 1,B","");
                                    break;
                                case 0x89:
                                    printLine("RES 1,C","");
                                    break;
                                case 0x8A:
                                    printLine("RES 1,D","");
                                    break;
                                case 0x8B:
                                    printLine("RES 1,E","");
                                    break;
                                case 0x8C:
                                    printLine("RES 1,H","");
                                    break;
                                case 0x8D:
                                    printLine("RES 1,L","");
                                    break;
                                case 0x8E:
                                    printLine("RES 1,(HL)","");
                                    break;
                                case 0x8F:
                                    printLine("RES 1,A","");
                                    break;

                                case 0x90:
                                    printLine("RES 2,B","");
                                    break;
                                case 0x91:
                                    printLine("RES 2,C","");
                                    break;
                                case 0x92:
                                    printLine("RES 2,D","");
                                    break;
                                case 0x93:
                                    printLine("RES 2,E","");
                                    break;
                                case 0x94:
                                    printLine("RES 2,H","");
                                    break;
                                case 0x95:
                                    printLine("RES 2,L","");
                                    break;
                                case 0x96:
                                    printLine("RES 2,(HL)","");
                                    break;
                                case 0x97:
                                    printLine("RES 2,A","");
                                    break;
                                case 0x98:
                                    printLine("RES 3,B","");
                                    break;
                                case 0x99:
                                    printLine("RES 3,C","");
                                    break;
                                case 0x9A:
                                    printLine("RES 3,D","");
                                    break;
                                case 0x9B:
                                    printLine("RES 3,E","");
                                    break;
                                case 0x9C:
                                    printLine("RES 3,H","");
                                    break;
                                case 0x9D:
                                    printLine("RES 3,L","");
                                    break;
                                case 0x9E:
                                    printLine("RES 3,(HL)","");
                                    break;
                                case 0x9F:
                                    printLine("RES 3,A","");
                                    break;

                                case 0xA0:
                                    printLine("RES 4,B","");
                                    break;
                                case 0xA1:
                                    printLine("RES 4,C","");
                                    break;
                                case 0xA2:
                                    printLine("RES 4,D","");
                                    break;
                                case 0xA3:
                                    printLine("RES 4,E","");
                                    break;
                                case 0xA4:
                                    printLine("RES 4,H","");
                                    break;
                                case 0xA5:
                                    printLine("RES 4,L","");
                                    break;
                                case 0xA6:
                                    printLine("RES 4,(HL)","");
                                    break;
                                case 0xA7:
                                    printLine("RES 4,A","");
                                    break;
                                case 0xA8:
                                    printLine("RES 5,B","");
                                    break;
                                case 0xA9:
                                    printLine("RES 5,C","");
                                    break;
                                case 0xAA:
                                    printLine("RES 5,D","");
                                    break;
                                case 0xAB:
                                    printLine("RES 5,E","");
                                    break;
                                case 0xAC:
                                    printLine("RES 5,H","");
                                    break;
                                case 0xAD:
                                    printLine("RES 5,L","");
                                    break;
                                case 0xAE:
                                    printLine("RES 5,(HL)","");
                                    break;
                                case 0xAF:
                                    printLine("RES 5,A","");
                                    break;

                                case 0xB0:
                                    printLine("RES 6,B","");
                                    break;
                                case 0xB1:
                                    printLine("RES 6,C","");
                                    break;
                                case 0xB2:
                                    printLine("RES 6,D","");
                                    break;
                                case 0xB3:
                                    printLine("RES 6,E","");
                                    break;
                                case 0xB4:
                                    printLine("RES 6,H","");
                                    break;
                                case 0xB5:
                                    printLine("RES 6,L","");
                                    break;
                                case 0xB6:
                                    printLine("RES 6,(HL)","");
                                    break;
                                case 0xB7:
                                    printLine("RES 6,A","");
                                    break;
                                case 0xB8:
                                    printLine("RES 7,B","");
                                    break;
                                case 0xB9:
                                    printLine("RES 7,C","");
                                    break;
                                case 0xBA:
                                    printLine("RES 7,D","");
                                    break;
                                case 0xBB:
                                    printLine("RES 7,E","");
                                    break;
                                case 0xBC:
                                    printLine("RES 7,H","");
                                    break;
                                case 0xBD:
                                    printLine("RES 7,L","");
                                    break;
                                case 0xBE:
                                    printLine("RES 7,(HL)","");
                                    break;
                                case 0xBF:
                                    printLine("RES 7,A","");
                                    break;

                                case 0xC0:
                                    printLine("SET 0,B","");
                                    break;
                                case 0xC1:
                                    printLine("SET 0,C","");
                                    break;
                                case 0xC2:
                                    printLine("SET 0,D","");
                                    break;
                                case 0xC3:
                                    printLine("SET 0,E","");
                                    break;
                                case 0xC4:
                                    printLine("SET 0,H","");
                                    break;
                                case 0xC5:
                                    printLine("SET 0,L","");
                                    break;
                                case 0xC6:
                                    printLine("SET 0,(HL)","");
                                    break;
                                case 0xC7:
                                    printLine("SET 0,A","");
                                    break;
                                case 0xC8:
                                    printLine("SET 1,B","");
                                    break;
                                case 0xC9:
                                    printLine("SET 1,C","");
                                    break;
                                case 0xCA:
                                    printLine("SET 1,D","");
                                    break;
                                case 0xCB:
                                    printLine("SET 1,E","");
                                    break;
                                case 0xCC:
                                    printLine("SET 1,H","");
                                    break;
                                case 0xCD:
                                    printLine("SET 1,L","");
                                    break;
                                case 0xCE:
                                    printLine("SET 1,(HL)","");
                                    break;
                                case 0xCF:
                                    printLine("SET 1,A","");
                                    break;


                                case 0xD0:
                                    printLine("SET 2,B","");
                                    break;
                                case 0xD1:
                                    printLine("SET 2,C","");
                                    break;
                                case 0xD2:
                                    printLine("SET 2,D","");
                                    break;
                                case 0xD3:
                                    printLine("SET 2,E","");
                                    break;
                                case 0xD4:
                                    printLine("SET 2,H","");
                                    break;
                                case 0xD5:
                                    printLine("SET 2,L","");
                                    break;
                                case 0xD6:
                                    printLine("SET 2,(HL)","");
                                    break;
                                case 0xD7:
                                    printLine("SET 2,A","");
                                    break;
                                case 0xD8:
                                    printLine("SET 3,B","");
                                    break;
                                case 0xD9:
                                    printLine("SET 3,C","");
                                    break;
                                case 0xDA:
                                    printLine("SET 3,D","");
                                    break;
                                case 0xDB:
                                    printLine("SET 3,E","");
                                    break;
                                case 0xDC:
                                    printLine("SET 3,H","");
                                    break;
                                case 0xDD:
                                    printLine("SET 3,L","");
                                    break;
                                case 0xDE:
                                    printLine("SET 3,(HL)","");
                                    break;
                                case 0xDF:
                                    printLine("SET 3,A","");
                                    break;

                                case 0xE0:
                                    printLine("SET 4,B","");
                                    break;
                                case 0xE1:
                                    printLine("SET 4,C","");
                                    break;
                                case 0xE2:
                                    printLine("SET 4,D","");
                                    break;
                                case 0xE3:
                                    printLine("SET 4,E","");
                                    break;
                                case 0xE4:
                                    printLine("SET 4,H","");
                                    break;
                                case 0xE5:
                                    printLine("SET 4,L","");
                                    break;
                                case 0xE6:
                                    printLine("SET 4,(HL)","");
                                    break;
                                case 0xE7:
                                    printLine("SET 4,A","");
                                    break;
                                case 0xE8:
                                    printLine("SET 5,B","");
                                    break;
                                case 0xE9:
                                    printLine("SET 5,C","");
                                    break;
                                case 0xEA:
                                    printLine("SET 5,D","");
                                    break;
                                case 0xEB:
                                    printLine("SET 5,E","");
                                    break;
                                case 0xEC:
                                    printLine("SET 5,H","");
                                    break;
                                case 0xED:
                                    printLine("SET 5,L","");
                                    break;
                                case 0xEE:
                                    printLine("SET 5,(HL)","");
                                    break;
                                case 0xEF:
                                    printLine("SET 5,A","");
                                    break;

                                case 0xF0:
                                    printLine("SET 6,B","");
                                    break;
                                case 0xF1:
                                    printLine("SET 6,C","");
                                    break;
                                case 0xF2:
                                    printLine("SET 6,D","");
                                    break;
                                case 0xF3:
                                    printLine("SET 6,E","");
                                    break;
                                case 0xF4:
                                    printLine("SET 6,H","");
                                    break;
                                case 0xF5:
                                    printLine("SET 6,L","");
                                    break;
                                case 0xF6:
                                    printLine("SET 6,(HL)","");
                                    break;
                                case 0xF7:
                                    printLine("SET 6,A","");
                                    break;
                                case 0xF8:
                                    printLine("SET 7,B","");
                                    break;
                                case 0xF9:
                                    printLine("SET 7,C","");
                                    break;
                                case 0xFA:
                                    printLine("SET 7,D","");
                                    break;
                                case 0xFB:
                                    printLine("SET 7,E","");
                                    break;
                                case 0xFC:
                                    printLine("SET 7,H","");
                                    break;
                                case 0xFD:
                                    printLine("SET 7,L","");
                                    break;
                                case 0xFE:
                                    printLine("SET 7,(HL)","");
                                    break;
                                case 0xFF:
                                    printLine("SET 7,A","");
                                    break;
                            }
                            
                            //counter++;//Increase our counter, as we've read another byte from our source file
                            //This way, our addresses going forward will be correct...

                            break;
                        case 0xCC:
                            printLine("CALL Z,a16","");
                            break;
                        case 0xCD: //CALL a16

                            byte1 = getNext8Bits(inputfile);
                            byte2 = getNext8Bits(inputfile);
                           
                            print16BitLine("CALL ", "Call a function at the address specified", "->PC", byte1,byte2);

                            //printLine("CALL a16","");
                            break;
                        case 0xCE:
                            printLine("ADC A,d8","");
                            break;
                        case 0xCF:
                            printLine("RST 08H", "Push current address onto the stack, and jump to 0x0010");
                            break;

                        case 0xD0:
                            printLine("RET NC", "");
                            break;
                        case 0xD1:
                            printLine("POP DE", "");
                            break;
                        case 0xD2:
                            printLine("JP NC,a16", "");
                            break;
                        case 0xD3:
                            printLine("Unused", "");
                            break;
                        case 0xD4:
                            printLine("CALL NC,16", "");
                            break;
                        case 0xD5:
                            printLine("PUSH DE", "");
                            break;
                        case 0xD6:
                            printLine("SUB d8", "");
                            break;
                        case 0xD7:
                            printLine("RST 10H", "Push current address onto the stack, and jump to 0x0010");
                            break;
                        case 0xD8:
                            printLine("RET C", "");
                            break;
                        case 0xD9:
                            printLine("RETI", "\tPop two bytes from the stack, go to that address, and enable interrupts");
                            break;
                        case 0xDA:
                            byte1 = getNext8Bits(inputfile);
                            byte2 = getNext8Bits(inputfile);
                           
                            print16BitLine("JP C,", "If the Carry flag is set, jump to the immediately following 16-bit address", "->PC", byte1,byte2);

                           // printLine("JP C,a16", "");
                            break;
                        case 0xDB:
                            printLine("Unused", "");
                            break;
                        case 0xDC:
                            printLine("CALL C,a16", "");
                            break;
                        case 0xDD:
                            printLine("Unused", "");
                            break;
                        case 0xDE:
                            printLine("SBC A,d8", "");
                            break;
                        case 0xDF:
                            printLine("RST 18H", "Push current address onto the stack, and jump to 0x0018");
                            break;



                        case 0xE0:
                            printLine("LDH (a8),A", "");
                            break;
                        case 0xE1:
                            printLine("POP HL", "");
                            break;
                        case 0xE2:
                            printLine("LD (C),A", "");
                            break;
                        case 0xE3:
                            printLine("Unused", "");
                            break;
                        case 0xE4:
                            printLine("Unused", "");
                            break;
                        case 0xE5:
                            printLine("PUSH HL\t\t\t\t\tPush the contents of HL onto the stack", "");
                            break;
                        case 0xE6: //AND d8
                            byte1 = getNext8Bits(inputfile);
                           
                            print8BitLine("AND A", "Do the binary AND operation of this immediate value and A", " && A", byte1);


                            //printLine("AND d8", "");
                            break;
                        case 0xE7:
                            printLine("RST 20H", "Push current address onto the stack, and jump to 0x0020");
                            break;
                        case 0xE8:
                            printLine("ADD SP,r8", "");
                            break;
                        case 0xE9:
                            printLine("JP (HL)", "");
                            break;
                        case 0xEA:
                            printLine("LD (a16),A", "");
                            break;
                        case 0xEB:
                            printLine("Unused", "");
                            break;
                        case 0xEC:
                            printLine("Unused", "");
                            break;
                        case 0xED:
                            printLine("Unused", "");
                            break;
                        case 0xEE:
                            byte1 = getNext8Bits(inputfile);
                           
                            print8BitLine("XOR ", "XOR this value against A", " XOR A->A", byte1);
                            //printLine("XOR d8", "");
                            break;
                        case 0xEF:
                            printLine("RST 28H", "Push current address onto the stack, and jump to 0x0028");
                            break;

                        case 0xF0:
                            byte1 = getNext8Bits(inputfile);
                           
                            print8BitLine("LDH A,", "Load", "+FF00 as pointer->A", byte1);
                           //printLine("LDH A,(a8)", "");
                            break;
                        case 0xF1:
                            printLine("POP AF", "");
                            break;
                        case 0xF2:
                            printLine("LD A,(C)", "");
                            break;
                        case 0xF3:
                            printLine("DI", "Disable interrupts");
                            break;
                        case 0xF4:
                            printLine("Unused", "");
                            break;
                        case 0xF5:
                            printLine("PUSH AF\t\t\t\tPush the contents of AF onto the stack", "");
                            break;
                        case 0xF6:
                            printLine("OR d8", "");
                            break;
                        case 0xF7:
                            printLine("RST 30H", "Push current address onto the stack, and jump to 0x0030");
                            break;
                        case 0xF8:
                            printLine("LD HL,SP+r8", "");
                            break;
                        case 0xF9:
                            printLine("LD SP,HL", "");
                            break;
                        case 0xFA:
                            printLine("LD A,(a16)", "");
                            break;
                        case 0xFB:
                            printLine("EI", "Enable Interrupts, but only after this instruction finishes executing");
                            break;
                        case 0xFC:
                            printLine("Unused", "");
                            break;
                        case 0xFD:
                            printLine("Unused", "");
                            break;
                        case 0xFE://CP d8
                            byte1 = getNext8Bits(inputfile);
                           
                            print8BitLine("CP ", "Subtract this value from A", "] A-this value->A", byte1);
                           // printLine("CP d8", "");
                            break;
                        case 0xFF:
                            printLine("RST 38H", "Push current address onto the stack, and jump to 0x0038");
                            break;
                    }
                    
                    cout << endl;
                    
                   // counter = counter+1;

            	}
            }
            else //Because we're starting assembly somewhere else, we don't care about this data. So let's burn it...
            {
                getNext8Bits(inputfile);
                counter++;
            }
        }
    }
}


