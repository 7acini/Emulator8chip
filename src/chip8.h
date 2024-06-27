#include <cstring>
#include <iostream>
#include <vector>

using namespace std;

//Constante que armazena o tamanho maximo para sua utilização em games
const int ROMSIZE = 0xFFF;

typedef unsigned char BYTE; //0 - 255
typedef unsigned short int WORD; // 0 - 65535

class Singleton {
	public:
		~Chip8();
		static Chip8* Sigleton();
//O Singleton é um padrão de projeto criacional, que garante que apenas um objeto desse tipo exista e forneça um único ponto de acesso a ele para qualquer outro código.
		void LoadGame(char *game);
		void ExecuteNextOpcode();
		void DecreaseTimers();
		void KeyPressed(int key);
		void NotKeyPressed(int key);
//Devido a um "erro" no glDrawPixels devemos colocar o eixo y antes do eixo x
		BYTE Screen[320][640][3];
	private:
		Chip8();
		WORD GetNextOpcode();
		void PlayBeep();
		int GetKeyPressed();

		void CPUInicialize();
		void ClearScreen() ;

		static Chip8CPU* Instance;
		BYTE GameMemory[ROMSIZE];	// 0xFFF bytes
		BYTE Registers[16];		// 16 Registradores, 1 byte
		WORD Counter;           	// Contador do Programa
		WORD FirstAddress;      	// Primeiro endereço de 16 bits
		vector<WORD> Stack;           	// Pilha de 16 bits
		BYTE KeyState[16] ;
		BYTE DelayTimer ;
		BYTE SoundTimer ;

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
};
