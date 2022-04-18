#include <SDL2/SDL.h>

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
using namespace std;
SDL_Window *window = NULL;
SDL_Surface *surface = NULL;
char gpudata[7500];
char rom[0xffff] ; //16383 in dec
uint8_t cpuram[0xffff] ; //all address space
uint8_t y = 0;
uint8_t x = 0;
uint8_t a = 0;
uint8_t statusRegister = 0b10111101 ; // negative 1 =neg
uint8_t stackPointer = 0xff;
uint16_t programCounter = 0xfffc;
uint16_t address16bit = 0;
uint8_t address = 0;


  void drawscreen(){
    SDL_Rect rec;
    rec.x = 0;
    rec.y = 0,
    rec.w = 8;
    rec.h = 8;
  
    for (int i = 0x8000; i < 0xbfff; i++){
        uint8_t red = 0;
        uint8_t blue = 0;
        uint8_t green = 0;

        uint8_t temp = cpuram[i] & 0b00000001;
        if (temp == 0b00000001){blue = blue+85;};
        temp = cpuram[i] & 0b00000010;
        if (temp == 0b00000010){blue = blue+85;};

        temp = cpuram[i] & 0b00000100;
        if (temp == 0b00000100){green = green+85;};
        temp = cpuram[i] & 0b00001000;
        if (temp == 0b00001000){green = green+85;};

        temp = cpuram[i] & 0b00010000;
        if (temp == 0b00010000){red = red+85;};
        temp = cpuram[i] & 0b00100000;
        if (temp == 0b00100000){red = red+85;};
        
        SDL_FillRect(surface, &rec, SDL_MapRGB(surface->format, red, green, blue));

        rec.x += 8;

        if (rec.x >= 800){
            rec.x = 0 ;
            rec.y = rec.y + 8;
        }
    }
SDL_UpdateWindowSurface(window);
   } 
    


    
  ////////////////////////////////////////////code start/////////////////////////////////////////////  
    int main(int argc, char *argv[] )  
{

        if (argc < 3 ) {
        printf("error: missing command line arguments\n6502x <rom> <speed>\ns= faster a= slower");
        return 1;
    }

    int timedelay = 0;
    int debug = 0 ;

    char *value;
    value = argv[2]; // how often is the screen drawn
   timedelay += atoi(value);

   // if (argc = 3){ // delay in seconds between ticks
  //     value = argv[3];
  //      debug += atoi(value);
  //  }



    window = SDL_CreateWindow("SDL2 Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    if (!window)
        return -1;
    surface = SDL_GetWindowSurface(window);
    if (!surface)
        return -2;
SDL_UpdateWindowSurface(window);

ifstream inFile; 
ifstream myFile (argv[1], ios::in | ios::binary);
//ifstream myFile ("a.out", ios::in | ios::binary);
myFile.read (rom, 0xffff);
myFile.close();

/*
ifstream inFile2; 
ifstream myFile2 ("full_screen.data", ios::in | ios::binary);
myFile2.read (gpudata, 0x3fff);
myFile2.close();
 load into rom space only
uint16_t temp = 0xc000; //location in cpuram to put rom
for (int i = 0; i <= 0x3fff; i++){ //3fff max rom size
cpuram[temp] = rom[i];
temp++;
}
*/

uint16_t temp = 0x0; //load into ALL address space
for (uint16_t i = 0; i < 0xffff; i++){ //3fff max rom size
cpuram[i] = rom[i];
temp++;
}
/*
temp = 0x8000; //location in cpuram for screen data
for (int i = 0; i <= 0x3fff; i++){ //3fff max size its too much 
cpuram[temp] = gpudata[i];
temp++;
}
*/


//reset cpu
uint8_t addressLowByte = cpuram[programCounter];
programCounter++;
uint8_t addressHighByte = cpuram[programCounter];
programCounter = (addressHighByte<<8)+addressLowByte;

int refreshScreenDelay = 0;
uint8_t currentInst = 0;
printf("start\nprogramcounter = %x \n",programCounter);

    while (1)
    {

        currentInst = cpuram[programCounter];

   printf("IR=%x (z=%x c=%x) PC=%x a=%x y=%x x=%x \n",currentInst,(statusRegister & 0b00000010)>>1,(statusRegister & 0b00000001),programCounter,a,y,x);
    printf("0000: %x %x %x %x %x %x %x %x   %x %x %x %x %x %x %x %x\n",
             cpuram[0], cpuram[1], cpuram[2], cpuram[3], cpuram[4], cpuram[5], cpuram[6], cpuram[7],
            cpuram[8], cpuram[9], cpuram[10], cpuram[11], cpuram[12], cpuram[13], cpuram[14], cpuram[15]);
    printf("0100: %x %x %x %x %x %x %x %x   %x %x %x %x %x %x %x %x\n",
             cpuram[0x01f0], cpuram[0x01f1], cpuram[0x01f2], cpuram[0x01f3], cpuram[0x01f4], cpuram[0x01f5], cpuram[0x01f6], cpuram[0x01f7],
            cpuram[0x01f8], cpuram[0x01f9], cpuram[0x01fa], cpuram[0x1fb], cpuram[0x01fc], cpuram[0x01fd], cpuram[0x01fe], cpuram[0x01ff]);

  
        switch (currentInst){
            case 0x00: //brk
            break;

            case 0x01: // ORA(zp,x)
            break;

            case 0x04: //TSB zp
            break;

            case 0x05: //ORA zp
            break;

            case 0x06: //ASL zp
            programCounter++;
            cpuram[programCounter] = cpuram[programCounter]<<1;                        
            programCounter++;            
            break;

            case 0x07: //RMB0 zp
            break;

            case 0x08: //PHP s
            break;

            case 0x09: //ORA #
            break;

            case 0x0a: //ASL A

            break;

            case 0x0c: //TSB a
            break;

            case 0x0d: //ORA a
            break;

            case 0x0e: //ASL a
            break;

            case 0x0f: //BBR0 r
            break;

            case 0x10: //BPL r
            break;

            case 0x11: //ORA(zp),y
            break;

            case 0x12: //ORA(zp)
            break;

            case 0x14: //TRB zp
            break;

            case 0x15: //ORA zp,x
            break;

            case 0x16: //ASL zp,x
            programCounter++;
            address = cpuram[programCounter]+x;
            address = address & 0b00001111; // rollover and stay in zero page
            cpuram[address] = cpuram[address]<<1;                        
            programCounter++;            
            break;

            case 0x17: //RMB1 zp
            break;

            case 0x18: //CLC i
            statusRegister = statusRegister & 0b11111110; // unsetcary flag            
            programCounter++;

            break;

            case 0x19: //ORA a,y
            break;

            case 0x1a: // INC a
            programCounter++;
            a = a + 1;
            if (a == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (a > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag            
            break;

            case 0x1c: //TRB a
            break;

            case 0x1d: //ORA a,x
            break;

            case 0x1e: //ASL a,x
            break;

            case 0x1f: //BBR1 r
            break;

            case 0x20: //jsr a           
            programCounter++;
            addressLowByte = cpuram[programCounter];
            programCounter++;

            addressHighByte = 0x01; //stack address high byte
            address16bit = (addressHighByte<<8)+stackPointer;            
            cpuram[address16bit] = (programCounter>>8);
            stackPointer--;
            address16bit = (addressHighByte<<8)+stackPointer;
            cpuram[address16bit] = programCounter; // will only load first 8 bits
            stackPointer--;

            addressHighByte = cpuram[programCounter];
            programCounter = (addressHighByte<<8)+addressLowByte;             
            break;

            case 0x21: //AND(zp,x)
            programCounter++; 
            addressLowByte = cpuram[(cpuram[programCounter]+x)];
            addressHighByte = cpuram[(cpuram[programCounter]+x+1)];
            address16bit = (addressHighByte<<8)+addressLowByte;
            a = cpuram[address16bit] & a;
            if (a == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (a > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag            
            programCounter++;            
            break;

            case 0x24: //BIT zp
            break;

            case 0x25: //AND zp

            break;

            case 0x26: //ROL zp
            break;

            case 0x27: //RMB2 zp
            break;

            case 0x28: //PLP s
            break;

            case 0x29: //AND #
            programCounter++;           
            a = a & (cpuram[programCounter]);
            if (a == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (a > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag
            programCounter++;            
            break;

            case 0x2a: //ROL a
            break;

            case 0x2c: //BIT a
            break;

            case 0x2d: //AND a
            break;

            case 0x2e: //ROL a
            break;

            case 0x2f: //BBR2 r
            break;

            case 0x30: //BMI r
            break;

            case 0x31: //AND(zp),y
            break;

            case 0x32: //AND(zp)
            programCounter++; 
            addressLowByte = cpuram[(cpuram[programCounter])];
            addressHighByte = cpuram[(cpuram[programCounter]+1)];
            address16bit = (addressHighByte<<8)+addressLowByte;
            a = cpuram[address16bit] & a;
            if (a == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (a > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag            
            programCounter++;
            break;

            case 0x34: //BIT zp,x
            break;

            case 0x35: //AND zp,x
            break;

            case 0x36: //ROL zp,x
            break;

            case 0x37: //RMB3 zp
            break;

            case 0x38: //SEC i
            programCounter++;
            statusRegister = statusRegister | 0b00000001; //set cary flag
            break;

            case 0x39: //AND a,y
            break;

            case 0x3a: //DEC A
            break;

            case 0x3c: //BIT a,x
            break;

            case 0x3d: //AND a,x
            break;

            case 0x3e: //ROL a,x
            break;

            case 0x3f: //BBR3 r
            break;

            case 0x40: //RTI s
            break;

            case 0x41: //EOR(zp,x)
            break;

            case 0x45: //EOR zp
            break;

            case 0x46: //LSR zp
            programCounter++;
            cpuram[programCounter] = cpuram[programCounter]>>1;                        
            programCounter++;            
            break;

            case 0x47: //RMB4 zp
            break;

            case 0x48: //PHA s
            programCounter++;
            addressHighByte = 0x01; //stack address high byte
            address16bit = (addressHighByte<<8)+stackPointer;
            cpuram[address16bit] = a;
            stackPointer--;
            break;

            case 0x49: //EOR #
            break;

            case 0x4a: //LSR A
            break;

            case 0x4c: // JMP a jmp 2byte address
            programCounter++;
            addressLowByte = cpuram[programCounter];
            programCounter++;
            addressHighByte = cpuram[programCounter];
            programCounter = (addressHighByte<<8)+addressLowByte;            
            break;

            case 0x4d: //EOR a
            break;            

            case 0x4e: //LSR a
            break;

            case 0x4f: //BBR4 r
            break;

            case 0x50: //BVC r
            break;

            case 0x51: //EOR(zp),y
            break;

            case 0x52: //EOR(zp)
            programCounter++; 
            addressLowByte = cpuram[(cpuram[programCounter])];
            addressHighByte = cpuram[(cpuram[programCounter]+1)];
            address16bit = (addressHighByte<<8)+addressLowByte;
            a = cpuram[address16bit] ^ a;
            if (a == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (a > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag            
            programCounter++;            
            break;

            case 0x55: //EOR zp,x
            
            break;

            case 0x56: //LSR zp,x
            programCounter++;
            address = cpuram[programCounter]+x;
            address = address & 0b00001111; // rollover and stay in zero page
            cpuram[address] = cpuram[address]>>1;                        
            programCounter++;            
            break;

            case 0x57: //RMB5 zp
            break;

            case 0x58: //CLI i
            break;

            case 0x59: //EOR a,y
            break;

            case 0x5a: //PHY s
            break;

            case 0x5d: //EOR a,x
            break;

            case 0x5e: //LSR a,x
            break;

            case 0x5f: //BBR5
            break;

            case 0x60: //RTS s
            stackPointer++;
            addressHighByte = 0x01; //stack address high byte
            address16bit = (addressHighByte<<8)+stackPointer;
            addressLowByte = cpuram[address16bit];
            stackPointer++;
            address16bit++;
            addressHighByte = cpuram[address16bit];
            programCounter = (addressHighByte<<8)+addressLowByte;
            programCounter++;            
            break;

            case 0x61: //ADC(zp,x)

            break;

            case 0x64: //STZ zp
            programCounter++;
            cpuram[(cpuram[programCounter])] = 0 ;
            programCounter++;           
            break;

            case 0x65: //ADC zp
            programCounter++;
            if ((statusRegister & 0b00000001) == 0b00000001) a++; // add cary if set
            a = a + (cpuram [(cpuram[programCounter])] );
            if (a < (cpuram[programCounter]) ) statusRegister = statusRegister | 0b01000001; // setcary and overflow flag 
            if (a == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (a > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag           
            programCounter++;
            break;

            case 0x66: //ROR zp           
            break;

            case 0x67: //RMB6 zp
            break;

            case 0x68: //PLA s
            programCounter++;
            stackPointer++;
            addressHighByte = 0x01; //stack address high byte
            address16bit = (addressHighByte<<8)+stackPointer;
            a = cpuram[address16bit];            
            break;

            case 0x69: //ADC # 
            programCounter++;
            if ((statusRegister & 0b00000001) == 0b00000001) a++; // add cary if set
            a = a + (cpuram[programCounter]);
            if (a < (cpuram[programCounter]) ) statusRegister = statusRegister | 0b01000001; // setcary and overflow flag 
            if (a == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (a > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag           
            programCounter++;
            break;

            case 0x6a: //ROR A
            break;

            case 0x6c: //JMP(a)
            break;

            case 0x6d: //ADC a
            break;

            case 0x6e: //ROR a
            break;

            case 0x6f: //BBR6 r
            break;

            case 0x70: //BVS r
            break;

            case 0x71: //ADC(zp)
            break;

            case 0x72: //ADC(zp)
            break;

            case 0x74: //STZ zp,x
            programCounter++;
            cpuram[(cpuram[programCounter]+x)] = 0 ;
            programCounter++;            
            break;

            case 0x75: //ADC zp,x
            break;

            case 0x76: //ROR zp,x
            break;

            case 0x77: //RMB7 zp
            break;

            case 0x78: //SEI i
            break;

            case 0x79: //ADC a,y
            break;

            case 0x7a: //PLY s
            programCounter++;
            stackPointer++;
            addressHighByte = 0x01; //stack address high byte
            address16bit = (addressHighByte<<8)+stackPointer;
            y = cpuram[address16bit];             
            break;

            case 0x7c: //JMP(a,x)
            break;

            case 0x7d: //ADC a,x
            break;

            case 0x7e: //ROR a,x
            break;

            case 0x7f: //BBR7 r
            break;

            case 0x80: //BRA r
            break;

            case 0x81: //STA(zp,x)
            programCounter++; 
            addressLowByte = cpuram[(cpuram[programCounter]+x)];
            addressHighByte = cpuram[(cpuram[programCounter]+x+1)];
            address16bit = (addressHighByte<<8)+addressLowByte;
            cpuram[address16bit] = a ;          
            programCounter++;            
            break;

            case 0x84: //STY zp
            programCounter++;
            cpuram[(cpuram[programCounter])] = y ;
            programCounter++;
            break;

            case 0x85: //STA zp 1byte
            programCounter++;
            cpuram[(cpuram[programCounter])] = a ;
            programCounter++;
            break;

            case 0x86: //STZ zp ??????STX zp????
            programCounter++;
            address = cpuram[programCounter];
            cpuram[address] = x;                        
            programCounter++;            
            break;

            case 0x87: //SMB0 zp
            programCounter++;
            cpuram[(cpuram[programCounter])] = cpuram[(cpuram[programCounter])] | 0b00000001 ;
            programCounter++;
            break;

            case 0x88: //DEY i
            programCounter++;
            y = y-1;
            if (y == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (y > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag
            break;

            case 0x89: //BIT #
            break;

            case 0x8a: //TXA i
            programCounter++;
            a = x;
            if (a == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (a > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag          
            break;

            case 0x8c: //STY a
            programCounter++;
            addressLowByte = cpuram[programCounter];
            programCounter++;
            addressHighByte = cpuram[programCounter];
            address16bit = (addressHighByte<<8)+addressLowByte;
            cpuram[address16bit] = y;
            programCounter++;            
            break;

            case 0x8d:  //STA a 2byte address
            programCounter++;
            addressLowByte = cpuram[programCounter];
            programCounter++;
            addressHighByte = cpuram[programCounter];
            address16bit = (addressHighByte<<8)+addressLowByte;
            cpuram[address16bit] = a;
            programCounter++;
            break;

            case 0x8e: //STX a
            programCounter++;
            addressLowByte = cpuram[programCounter];
            programCounter++;
            addressHighByte = cpuram[programCounter];
            address16bit = (addressHighByte<<8)+addressLowByte;
            cpuram[address16bit] = x;
            programCounter++;            
            break;

            case 0x8f: //BBS0 r
            break;

            case 0x90: //BCC r
            break;

            case 0x91: //STA(zp),y
            programCounter++;
            address = cpuram[programCounter];
            addressLowByte = cpuram[address];
            addressHighByte =  cpuram[address+1];
            address16bit = (addressHighByte<<8)+addressLowByte;
            cpuram[address16bit+y] = a;
            programCounter++;

            break;

            case 0x92: //STA(zp)
            programCounter++;
            address = cpuram[programCounter];
            addressLowByte = cpuram[address];
            addressHighByte =  cpuram[address+1];
            address16bit = (addressHighByte<<8)+addressLowByte;
            cpuram[address16bit] = a;
            programCounter++;
            break;

            case 0x94: //STY zp,x
            programCounter++;
            address = cpuram[programCounter]+x;
            address = address & 0b00001111; // rollover and stay in zero page
            cpuram[address] = y;                        
            programCounter++;            
            break;

            case 0x95: //STA zp,x
            programCounter++;
            address = cpuram[programCounter]+x;
            address = address & 0b00001111; // rollover and stay in zero page
            cpuram[address] = a;                        
            programCounter++;            
            break;

            case 0x96: //STZ zp,y
            programCounter++;
            address = cpuram[programCounter]+y;
            address = address & 0b00001111; // rollover and stay in zero page
            cpuram[address] = 0;                        
            programCounter++;            
            break;

            case 0x97: //SMB1 zp
            programCounter++;
            cpuram[(cpuram[programCounter])] = cpuram[(cpuram[programCounter])] | 0b00000010 ;
            programCounter++;
            break;

            case 0x98: //TYA i
            programCounter++;
            a = y;
            if (a == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (a > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag
            break;

            case 0x99: //STA a,y
            programCounter++;
            addressLowByte = cpuram[programCounter];
            programCounter++;
            addressHighByte = cpuram[programCounter];
            address16bit = (addressHighByte<<8)+addressLowByte;
            cpuram[address16bit+y] = a;
            programCounter++;            
            break;

            case 0x9a: //TSX i
            break;

            case 0x9c: //STZ a
            break;

            case 0x9d: //STA a,x
            programCounter++;
            addressLowByte = cpuram[programCounter];
            programCounter++;
            addressHighByte = cpuram[programCounter];
            address16bit = (addressHighByte<<8)+addressLowByte;
            cpuram[address16bit+x] = a;
            programCounter++; 
            break;

            case 0x9e: //STZ a,x
            programCounter++;
            addressLowByte = cpuram[programCounter];
            programCounter++;
            addressHighByte = cpuram[programCounter];
            address16bit = (addressHighByte<<8)+addressLowByte;
            cpuram[address16bit+x] = 0;
            programCounter++;  
            break;

            case 0x9f: //BBS1 r
            break;

            case 0xa0: //LDY #
            programCounter++;
            y = cpuram[programCounter];
            if (y == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (y > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag
            programCounter++;            
            break;

            case 0xa1: //LDA(zp,x)
            programCounter++; 
            addressLowByte = cpuram[(cpuram[programCounter]+x)];
            addressHighByte = cpuram[(cpuram[programCounter]+x+1)];
            address16bit = (addressHighByte<<8)+addressLowByte;
            a = cpuram[address16bit];
            if (a == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (a > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag            
            programCounter++; 
            break;

            case 0xa2: //LDX #
            programCounter++;
            x = cpuram[programCounter];
            if (x == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (x > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag
            programCounter++;            
            break;

            case 0xa4: //LDY zp
            programCounter++;
            y = cpuram[(cpuram[programCounter])];
            if (y == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (y > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag            
            programCounter++;           
            break;

            case 0xa5: //LDA zp
            programCounter++;
            a = cpuram[(cpuram[programCounter])];
            if (a == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (a > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag            
            programCounter++;
            break;

            case 0xa6: //LDX zp
            programCounter++;
            x = cpuram[(cpuram[programCounter])];
            if (x == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (x > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag            
            programCounter++;            
            break;

            case 0xa7: //SMB2 zp
            programCounter++;
            cpuram[(cpuram[programCounter])] = cpuram[(cpuram[programCounter])] | 0b00000100 ;
            programCounter++;
            break;

            case 0xa8: //TAY i
            programCounter++;
            y = a;
            if (y == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (y > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag
            break;

            case 0xa9: //LDA # immediate
             programCounter++;
             a = cpuram[programCounter];
             if (a == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
             if (a > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag
             programCounter++;
             break;

            case 0xaa: //TAX i
            programCounter++;
            x = a;
            if (x == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (x > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag
            break;

            case 0xac: //LDY a
            programCounter++;
            addressLowByte = cpuram[programCounter];
            programCounter++;
            addressHighByte = cpuram[programCounter];
            address16bit = (addressHighByte<<8)+addressLowByte;
            y = cpuram[address16bit];
            if (y == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (y > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag
            programCounter++;            
            break;

            case 0xad: //LDA a
            programCounter++;
            addressLowByte = cpuram[programCounter];
            programCounter++;
            addressHighByte = cpuram[programCounter];
            address16bit = (addressHighByte<<8)+addressLowByte;
            a = cpuram[address16bit];
            if (a == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (a > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag
            programCounter++;
            break;

            case 0xae: //LDX a
            programCounter++;
            addressLowByte = cpuram[programCounter];
            programCounter++;
            addressHighByte = cpuram[programCounter];
            address16bit = (addressHighByte<<8)+addressLowByte;
            x = cpuram[address16bit];
            if (x == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (x > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag
            programCounter++;            
            break;

            case 0xaf: //BBS2 r
            break;

            case 0xb0: //BCS r
            break;

            case 0xb1: //LDA(zp),y
            programCounter++; 
            addressLowByte = cpuram[(cpuram[programCounter])];
            addressHighByte = cpuram[(cpuram[programCounter]+1)];
            address16bit = (addressHighByte<<8)+addressLowByte;
            a = cpuram[address16bit+y];
            if (a == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (a > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag            
            programCounter++;            
            break;

            case 0xb2: //LDA(zp)
            programCounter++; 
            addressLowByte = cpuram[(cpuram[programCounter])];
            addressHighByte = cpuram[(cpuram[programCounter]+1)];
            address16bit = (addressHighByte<<8)+addressLowByte;
            a = cpuram[address16bit];
            if (a == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (a > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag            
            programCounter++;             
            break;

            case 0xb4: //LDY zp,x
            programCounter++;
            address = cpuram[programCounter]+x;
            address = address & 0b00001111; // rollover and stay in zero page
            y = cpuram[address];            
            if (y == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (y > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag            
            programCounter++;  
            break;

            case 0xb5: //LDA zp,x
            programCounter++;
            address = cpuram[programCounter]+x;
            address = address & 0b00001111; // rollover and stay in zero page
            a = cpuram[address];            
            if (a == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (a > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag            
            programCounter++;            
            break;

            case 0xb6: //LDX zp,y
            programCounter++;
            address = cpuram[programCounter]+y;
            address = address & 0b00001111; // rollover and stay in zero page
            x = cpuram[address];            
            if (x == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (x > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag            
            programCounter++;            
            break;

            case 0xb7: //SMB3 zp
            programCounter++;
            cpuram[(cpuram[programCounter])] = cpuram[(cpuram[programCounter])] | 0b00001000 ;
            programCounter++;
            break;

            case 0xb8: //CLV i
            break;

            case 0xb9: //LDA A,y
            programCounter++;
            addressLowByte = cpuram[programCounter];
            programCounter++;
            addressHighByte = cpuram[programCounter];
            address16bit = (addressHighByte<<8)+addressLowByte;
            a = cpuram[address16bit+y] ;
            if (a == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (a > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag            
            programCounter++;            
            break;

            case 0xba: //TSX i
            break;

            case 0xbc: //LDY a,x
            programCounter++;
            addressLowByte = cpuram[programCounter];
            programCounter++;
            addressHighByte = cpuram[programCounter];
            address16bit = (addressHighByte<<8)+addressLowByte;
            y = cpuram[address16bit+x] ;
            if (a == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (a > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag            
            programCounter++;            
            break;

            case 0xbd: //LDA a,x
            programCounter++;
            addressLowByte = cpuram[programCounter];
            programCounter++;
            addressHighByte = cpuram[programCounter];
            address16bit = (addressHighByte<<8)+addressLowByte;
            a = cpuram[address16bit+x] ;
            if (a == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (a > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag            
            programCounter++;            
            break;

            case 0xbe: //LDX a,x
            break;

            case 0xbf: //BBS3 r
            break;

            case 0xc0: //CPY #
            programCounter++;
            address = cpuram[programCounter];
            y =  y - address ;
            if (y == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (y > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag
            programCounter++;
            y = y + address; //return a regester back             
            break;

            case 0xc1: //CMP(zp,x)
            break;

            case 0xc4: //CPY zp
            break;

            case 0xc5: //CMP zp NEEDS CHECKING!!!!!
            programCounter++;
            address = cpuram[(cpuram[programCounter])];
            a =  a - address ;
            if (a == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (a > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag
            programCounter++;
            a = a + address; //return a regester back
            break;

            case 0xc6: //DEC zp
            programCounter++;
            cpuram[(cpuram[programCounter])]-- ;
            if (cpuram[(cpuram[programCounter])] == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (cpuram[(cpuram[programCounter])] > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag            
            programCounter++;            
            break;

            case 0xc7: //SMB4 zp
            programCounter++;
            cpuram[(cpuram[programCounter])] = cpuram[(cpuram[programCounter])] | 0b00010000 ;
            programCounter++;            
            break;

            case 0xc8: //INY i
            programCounter++;
            y = y+1;
            if (y == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (y > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag
            break;

            case 0xc9: //CMP #
            programCounter++;
            address = cpuram[programCounter];
            a =  a - address ;
            if (a == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (a > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag
            programCounter++;
            a = a + address; //return a regester back            
            break;

            case 0xca: //DEX i
            programCounter++;
            x = x-1;
            if (x == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (x > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag            
            break;

            case 0xcb: //WAI I
            break;

            case 0xcc: //CPY a
            programCounter++;
            addressLowByte = cpuram[programCounter];
            programCounter++;
            addressHighByte = cpuram[programCounter];
            address16bit = (addressHighByte<<8)+addressLowByte;
            address = cpuram[address16bit];           
            y =  y - address ;
            if (y == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (y > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag
            programCounter++;
            y = y + address; //return a regester back 
            break;

            case 0xcd: //CMP a
            break;

            case 0xce: //DEC a
            break;

            case 0xcf: //BBS4
            break;

            case 0xd0: //BNE r
            programCounter++;
            if( (statusRegister & 0b00000010) == 0){
            addressHighByte = (programCounter>>8);
            addressLowByte = (programCounter)+cpuram[programCounter];
            programCounter = (addressHighByte<<8)+addressLowByte;
            }
            programCounter++;
            break;

            case 0xd1: //CMP(zp),y
            break;

            case 0xd2: //CMP(zp)
            break;

            case 0xd5: //CMP zp,x
            break;

            case 0xd6: //DEC zp,x
            programCounter++;
            cpuram[(cpuram[programCounter]+x)]-- ;
            if (cpuram[(cpuram[programCounter]+x)] == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (cpuram[(cpuram[programCounter]+x)] > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag            
            programCounter++;            
            break;

            case 0xd7: //SMB5 zp
            programCounter++;
            cpuram[(cpuram[programCounter])] = cpuram[(cpuram[programCounter])] | 0b00100000 ;
            programCounter++;            
            break;

            case 0xd8: //CLD i
            break;

            case 0xd9: //CMP a,y
            break;

            case 0xda: //PHX s
            break;

            case 0xdb: //STP I
            printf("halt in 5 sec");
            sleep(2);
            return 0;
            break;

            case 0xdd: //CMP a,x
            break;

            case 0xde: //DEC a,x
            break;

            case 0xdf: //BBS5 r
            break;

            case 0xe0: //CPX #
            programCounter++;
            address = cpuram[programCounter];
            x =  x - address ;
            if (x == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (x > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag
            programCounter++;
            x = x + address; //return a regester back 
            break;

            case 0xe1: //SBC(zp,x)
            break;

            case 0xe4: //CPX zp
            programCounter++;
            address = cpuram[programCounter];
            x =  x - address ;
            if (x == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (x > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag
            programCounter++;
            x = x + address; //return a regester back            
            break;

            case 0xe5: //SBC zp
            break;

            case 0xe6: //INC zp
            programCounter++;
            cpuram[(cpuram[programCounter])]++ ;
            if (cpuram[(cpuram[programCounter])] == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (cpuram[(cpuram[programCounter])] > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag                         
            programCounter++;            
            break;

            case 0xe7: //SMB6 zp
            programCounter++;
            cpuram[(cpuram[programCounter])] = cpuram[(cpuram[programCounter])] | 0b01000000 ;
            programCounter++;            
            break;

            case 0xe8: //INX i
            programCounter++;
            x = x+1;
            if (x == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (x > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag
            break;

            case 0xe9: //SBC #
            break;

            case 0xea: //NOP i
            programCounter++;
            sleep(10);
            break;

            case 0xec: //CPX a
            break;
 
            case 0xed: //SBC a
            break;

            case 0xee: //INC a
            programCounter++;
            addressLowByte = cpuram[programCounter];
            programCounter++;
            addressHighByte = cpuram[programCounter];
            address16bit = (addressHighByte<<8)+addressLowByte;
            cpuram[address16bit] = cpuram[address16bit]+1;           
            if (cpuram[address16bit] == 0) statusRegister = statusRegister | 0b00000010; // setzero flag            
            if (cpuram[address16bit] > 0) statusRegister = statusRegister & 0b11111101; // setzero flag 
            programCounter++;            

            break;

            case 0xef: //BBS6 r
            break;

            case 0xf0: //BEQ r
            programCounter++;
            if(statusRegister & (1<<1)){
            addressHighByte = (programCounter>>8);
            addressLowByte = (programCounter)+cpuram[programCounter];
            programCounter = (addressHighByte<<8)+addressLowByte;
            }
            programCounter++;
            break;

            case 0xf1: //SBC(zp),y
            break;

            case 0xf2: //SBC(zp)
            break;

            case 0xf5: //SBC zp,x
            break;

            case 0xf6: //INC zp,x
            programCounter++;
            cpuram[(cpuram[programCounter]+x)]-- ;
            if (cpuram[(cpuram[programCounter]+x)] == 0) statusRegister = statusRegister | 0b00000010; // setzero flag
            if (cpuram[(cpuram[programCounter]+x)] > 0) statusRegister = statusRegister & 0b11111101; // unsetzero flag             
            programCounter++;            
            break;

            case 0xf7: //SMB7 zp
            programCounter++;
            cpuram[(cpuram[programCounter])] = cpuram[(cpuram[programCounter])] | 0b10000000 ;
            programCounter++;            
            break;

            case 0xf8: //SED i
            break;

            case 0xf9: //SBC a,y
            break;

            case 0xfa: //PLX s
            programCounter++;
            stackPointer++;
            addressHighByte = 0x01; //stack address high byte
            address16bit = (addressHighByte<<8)+stackPointer;
            x = cpuram[address16bit];             
            break;

            case 0xfd: //SBC a,x
            break;

            case 0xfe: //INC a,x
            break;

            case 0xff: //BBS7
            break;


        }

    printf("IR=%x (z=%x c=%x) PC=%x a=%x y=%x x=%x \n",currentInst,(statusRegister & 0b00000010)>>1,(statusRegister & 0b00000001),programCounter,a,y,x);
    printf("0000: %x %x %x %x %x %x %x %x   %x %x %x %x %x %x %x %x\n",
             cpuram[0], cpuram[1], cpuram[2], cpuram[3], cpuram[4], cpuram[5], cpuram[6], cpuram[7],
            cpuram[8], cpuram[9], cpuram[10], cpuram[11], cpuram[12], cpuram[13], cpuram[14], cpuram[15]);
    printf("0100: %x %x %x %x %x %x %x %x   %x %x %x %x %x %x %x %x\n\n",
             cpuram[0x01f0], cpuram[0x01f1], cpuram[0x01f2], cpuram[0x01f3], cpuram[0x01f4], cpuram[0x01f5], cpuram[0x01f6], cpuram[0x01f7],
            cpuram[0x01f8], cpuram[0x01f9], cpuram[0x01fa], cpuram[0x1fb], cpuram[0x01fc], cpuram[0x01fd], cpuram[0x01fe], cpuram[0x01ff]);
       
        
        if (refreshScreenDelay == 0){   
         drawscreen();      
        refreshScreenDelay = timedelay ;
        }
        if (refreshScreenDelay > 0) refreshScreenDelay--;
        sleep(debug);

//handle keyboard input
	SDL_Event e;
	while( SDL_PollEvent( &e ) != 0 ) {
		switch (e.type) {
			

            case SDL_QUIT:
				return false;
			case SDL_KEYDOWN:
			    switch ( e.key.keysym.sym ) {
					case SDLK_q:
                        return 0; 
						break;
					case SDLK_a:
                        timedelay = timedelay - 1;
						break;
					case SDLK_s:
                        timedelay = timedelay + 1;
						break;                        
					// etc
				}
				
				break;
			case SDL_KEYUP:  
                       
				// can also test individual keys, modifier flags, etc, etc.
				break;
			case SDL_MOUSEMOTION:
				// etc.
				break;
		}
	}







    }
    
}



