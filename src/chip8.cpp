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

void LoadGame(char *game) {
  FILE *fgame;
  fgame = fopen(game, "rb");
  fread(&GameMemory[0x200], 0xFFF, 1, fgame);
  fclose(fgame);
}

WORD GetNextOperationalCode(){
  WORD op = 0;
  op = GameMemory[Counter]; // no exemplo op é 0xAB
  op <<= 8; // desloca 8 bits para esquerda, op é 0xAB00
  op |= GameMemory[Counter + 1]; // op é 0xABCD
  Counter += 2;
  return op;
}

WORD opcode = GetNextOpcode(); // suponha que isso retorne 0x1234
// decodifica o opcode
switch (opcode & 0xF000)
{
   case 0x1000: Opcode1NNN(opcode); quebrar ; // salta opcode
   case 0x0000: // precisa quebrar ainda mais esse opcode
   {
     switch(opcode & 0x000F)
     {
       case 0x0000: 0pcode00E0(opcode) ; break // limpar tela opcode
       case 0x000E: Opcode00EE(opcode) ; break // retorna subrotina
     }
   }
   break ;
   padrão: pausa; // opcode ainda a ser tratado
}

void Opcode1NNN (WORD opcode)
{
  Counter = opcode & 0x0FFF; // lembre-se que estamos interessados ​​apenas no NNN do opcode 1NNN
}