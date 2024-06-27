#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

//Constante que armazena o tamanho maximo para sua utilização em games
const int ROMSIZE = 0xFFF;

typedef unsigned char BYTE; //0 - 255
typedef unsigned short int WORD; // 0 - 65535

static Chip8CPU* Instance;
BYTE GameMemory[ROMSIZE];	// 0xFFF bytes
BYTE Registers[16];		// 16 Registradores, 1 byte
WORD Counter;           	// Contador do Programa
WORD FirstAddress;      	// Primeiro endereço de 16 bits
vector<WORD> Stack;           	// Pilha de 16 bits
BYTE KeyState[16] ;
BYTE DelayTimer ;
BYTE SoundTimer ;
};


void CPUInicialize() {
  FirstAddress = 0;
  Counter = 0x200;
  memset(Registers, 0, sizeof(Registers)); //Define os Registradores para 0
}

void LoadGame(char *game) {
  FILE *fgame;
  fgame = fopen(game, "rb");
  fread(&GameMemory[0x200], ROMSIZE, 1, fgame);
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
   case 0x1000: Opcode1NNN(opcode); break ; // salta opcode
   case 0x0000: // precisa quebrar ainda mais esse opcode
   {
     switch(opcode & 0x000F)
     {
       case 0x0000: Opcode00E0(opcode) ; break // limpar tela opcode
       case 0x000E: Opcode00EE(opcode) ; break // retorna subrotina
     }
   }
   break ;
   default: break; // opcode ainda a ser tratado
}

void CPUReset();
void ClearScreen() ;

void DecodeOpcode0(WORD opcode);
void DecodeOpcode8(WORD opcode);
void DecodeOpcodeE(WORD opcode);
void DecodeOpcodeF(WORD opcode);
void Opcode00EE();
void Opcode1NNN(WORD opcode);
void Opcode2NNN(WORD opcode);
void Opcode3XNN(WORD opcode);
void Opcode4XNN(WORD opcode);
void Opcode5XY0(WORD opcode);
void Opcode6XNN(WORD opcode);
void Opcode7XNN(WORD opcode);
void Opcode8XY0(WORD opcode);
void Opcode8XY1(WORD opcode);
void Opcode8XY2(WORD opcode);
void Opcode8XY3(WORD opcode);
void Opcode8XY4(WORD opcode);
void Opcode8XY5(WORD opcode);
void Opcode8XY6(WORD opcode);
void Opcode8XY7(WORD opcode);
void Opcode8XYE(WORD opcode);
void Opcode9XY0(WORD opcode);
void OpcodeANNN(WORD opcode);
void OpcodeBNNN(WORD opcode);
void OpcodeCXNN(WORD opcode);
void OpcodeDXYN(WORD opcode);
void OpcodeEX9E(WORD opcode);
void OpcodeEXA1(WORD opcode);
void OpcodeFX07(WORD opcode);
void OpcodeFX0A(WORD opcode);
void OpcodeFX15(WORD opcode);
void OpcodeFX18(WORD opcode);
void OpcodeFX1E(WORD opcode);
void OpcodeFX29(WORD opcode);
void OpcodeFX33(WORD opcode);
void OpcodeFX55(WORD opcode);
void OpcodeFX65(WORD opcode);
