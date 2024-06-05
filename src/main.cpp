#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

//Constantes que armazenam a quantidade Max de memória
typedef unsigned char BYTE; //0 - 255
typedef unsigned short int WORD; // 0 - 65535

BYTE GameMemory[0xFFF]; // 0xFFF bytes
BYTE Registers[16];     // 16 Registradores, 1 byte
WORD Counter;           // Contador do Programa
WORD FirstAddress;      // Primeiro endereço de 16 bits
vector Stack;           // Pilha de 16 bits

void CPUInicialize() {
  FirstAddress = 0;
  Counter = 0x200;
  memset(Registers, 0, sizeof(Registers)); //Define os Registradores para 0
}

