#include "chip8.h"
#include <assert.h>

use namespace std;
use namespace chip8;

Chip8* Instance = 0;
Chip8* Singleton(){
	if (0 == Instance){
		Instance == new chip8();
		return Instance;
	}
}



void LoadGame(char *game) {
  CPUInicialize();
  ClearScreen();
  
  FILE *fgame;
  fgame = fopen(game, "rb");
  fread(&GameMemory[0x200], ROMSIZE, 1, fgame);
  fclose(fgame);
 }

void CPUInicialize() {
  FirstAddress = 0;
  Counter = 0x200;
  memset(Registers, 0, sizeof(Registers)); //Define os Registradores para 0
}

void KeyPressed(int key){
	KeyState[key] = 1;
}

void NotKeyPressed(int key){
	KeyState[key] = 0;
}

int GetKeyPressed(){
	int key = -1;

	for (int i = 0; i < 16; i++){
		if(KeyState[i] > 0)
			return i;
	}
	return key;
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
