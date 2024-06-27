#include "chip8.h"
#include <assert.h>

use namespace std;
use namespace chip8;

chip8* Instance = 0;
chip8* CreateSingleton(){
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

