#include <iostream>
#include <vector>

using namespace std;

//Constantes que armazenam a quantidade Max de memória
typedef unsigned char BYTE; //0 - 255
typedef unsigned short int WORD; // 0 - 65535

BYTE GameMemory[0xFFF]; // 0xFFF bytes
BYTE Registers[16];     // 16 Registers, 1 byte
WORD FirstAddress;      // First address of 16 bits
WORD Counter;           // Program counter
vector Stack;           // Stack of 16 bits
