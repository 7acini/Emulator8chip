#include "chip8.h"
#include <cassert>

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

void ClearScreen( ){
	for (int x = 0; x < 640; x++){
		for (int y = 0 ; y < 320; y++){
			ScreenData[y][x][0] = 255 ;
			ScreenData[y][x][1] = 255 ;
			ScreenData[y][x][2] = 255 ;
		}
	}
}


void DecreaseTimers( ){
	if (DelayTimer > 0)
		m_DelayTimer-- ;

	if (SoundTimer > 0)
		m_SoundTimer--;

	if (SoundTimer > 0)
		PlayBeep( );
}

void ExecuteOpcode( )
{
	WORD opcode = GetNextOpcode( ) ;
	switch (opcode & 0xF000){
		case 0x0000: DecodeOpcode0(opcode);
			break;
		case 0x1000: Opcode1NNN(opcode);
			break;
		case 0x2000: Opcode2NNN(opcode);
			break ;
		case 0x3000: Opcode3XNN(opcode);
			break ;
		case 0x4000: Opcode4XNN(opcode);
			break ;
		case 0x5000: Opcode5XY0(opcode);
			break ;
		case 0x6000: Opcode6XNN(opcode);
			break ;
		case 0x7000: Opcode7XNN(opcode);
			break ;
		case 0x8000: DecodeOpcode8(opcode);
			break ;
		case 0x9000: Opcode9XY0(opcode);
			break ;
		case 0xA000: OpcodeANNN(opcode);
			break ;
		case 0xB000: OpcodeBNNN(opcode);
			break ;
		case 0xC000: OpcodeCXNN(opcode);
			break ;
		case 0xD000: OpcodeDXYN(opcode);
			break ;
		case 0xE000: DecodeOpcodeE(opcode);
			break ;
		case 0xF000: DecodeOpcodeF(opcode);
			break ;
		default: break;
	}
}

void DecodeOpcode8(WORD opcode){
	switch (opcode & 0xF){
		case 0x0: Opcode8XY0(opcode); 
			break;
		case 0x1: Opcode8XY1(opcode);
			break;
		case 0x2: Opcode8XY2(opcode);
			break;
		case 0x3: Opcode8XY3(opcode);
			break;
		case 0x4: Opcode8XY4(opcode);
			break;
		case 0x5: Opcode8XY5(opcode);
			break;
		case 0x6: Opcode8XY6(opcode);
			break;
		case 0x7: Opcode8XY7(opcode);
			break;
		case 0xE: Opcode8XYE(opcode);
			break;
		default: break;
	}
}

void DecodeOpcode0(WORD opcode){
	switch (opcode & 0xF){
	case 0x0: ClearScreen( );
		break ;
	case 0xE: Opcode00EE( ); 
		break ;
	default: break;
	}
}

void DecodeOpcodeE(WORD opcode)
{
	switch(opcode & 0xF)
	{
		case 0xE: OpcodeEX9E(opcode) ; break ;
		case 0x1: OpcodeEXA1(opcode) ; break ;
		default:break ;
	}
}

void DecodeOpcodeF(WORD opcode)
{
	switch(opcode & 0xFF)
	{
		case 0x07: OpcodeFX07(opcode) ; break ;
		case 0x0A: OpcodeFX0A(opcode) ; break ;
		case 0x15: OpcodeFX15(opcode) ; break ;
		case 0x18: OpcodeFX18(opcode) ; break ;
		case 0x1E: OpcodeFX1E(opcode) ; break ;
		case 0x29: OpcodeFX29(opcode) ; break ;
		case 0x33: OpcodeFX33(opcode) ; break ;
		case 0x55: OpcodeFX55(opcode) ; break ;
		case 0x65: OpcodeFX65(opcode) ; break ;
		default: break ;
	}
}

/// return from subroutine
void Opcode00EE( )
{
	m_ProgramCounter = m_Stack.back( ) ;
	m_Stack.pop_back( ) ;
}

// jump to address NNN
void Chip8CPU::Opcode1NNN(WORD opcode)
{
	m_ProgramCounter = opcode & 0x0FFF ;
}

// call subroutine NNN
void Opcode2NNN(WORD opcode)
{
	m_Stack.push_back(m_ProgramCounter) ;
	m_ProgramCounter = opcode & 0x0FFF ;
}

// skip next instruction if VX == NN
void Opcode3XNN(WORD opcode)
{
	int nn = opcode & 0x00FF ;
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;

	if (m_Registers[regx] == nn)
		m_ProgramCounter += 2 ;
}

// skip next instruction if VX != NN
void Opcode4XNN(WORD opcode)
{
	int nn = opcode & 0x00FF ;
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;

	if (m_Registers[regx] != nn)
		m_ProgramCounter += 2 ;
}

// skip next instruction if VX == VY
void Opcode5XY0(WORD opcode)
{
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;
	int regy = opcode & 0x00F0 ;
	regy >>= 4 ;

	if (m_Registers[regx] == m_Registers[regy])
		m_ProgramCounter += 2 ;
}

// sets VX to nn
void Opcode6XNN(WORD opcode)
{
	int nn = opcode & 0x00FF ;
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;

	m_Registers[regx] = nn;
}

// adds NN to vx. carry not affected
void Opcode7XNN(WORD opcode)
{
	int nn = opcode & 0x00FF ;
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;

	m_Registers[regx] += nn;
}

// set vx to vy
void Opcode8XY0(WORD opcode)
{
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;
	int regy = opcode & 0x00F0 ;
	regy >>= 4 ;

	m_Registers[regx] = m_Registers[regy] ;
}

// VX = VX | VY
void Opcode8XY1(WORD opcode)
{
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;
	int regy = opcode & 0x00F0 ;
	regy >>= 4 ;

	m_Registers[regx] = m_Registers[regx] | m_Registers[regy] ;
}

// VX = VX & VY
void Opcode8XY2(WORD opcode)
{
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;
	int regy = opcode & 0x00F0 ;
	regy >>= 4 ;

	m_Registers[regx] = m_Registers[regx] & m_Registers[regy] ;
}

// VX = VX xor VY
void Opcode8XY3(WORD opcode)
{
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;
	int regy = opcode & 0x00F0 ;
	regy >>= 4 ;

	m_Registers[regx] = m_Registers[regx] ^ m_Registers[regy] ;
}

// add vy to vx. set carry to 1 if overflow otherwise 0
void Opcode8XY4(WORD opcode)
{
	m_Registers[0xF] = 0 ;
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;
	int regy = opcode & 0x00F0 ;
	regy >>= 4 ;

	int value = m_Registers[regx] + m_Registers[regy] ;

	if (value > 255)
		m_Registers[0xF] = 1 ;

	m_Registers[regx] = m_Registers[regx] + m_Registers[regy] ;
}

// sub vy from vx. set carry to 1 if no borrow otherwise 0
void Opcode8XY5(WORD opcode)
{
	m_Registers[0xF] = 1 ;

	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;
	int regy = opcode & 0x00F0 ;
	regy >>= 4 ;

	if (m_Registers[regx] < m_Registers[regy])
		m_Registers[0xF] = 0 ;

	m_Registers[regx] = m_Registers[regx] - m_Registers[regy] ;
}

// Shifts VX right by one. VF is set to the value of the least significant bit of VX before the shift.
void Opcode8XY6(WORD opcode)
{
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;

	m_Registers[0xF] = m_Registers[regx] & 0x1 ;
	m_Registers[regx] >>= 1 ;
}

// Sets VX to VY minus VX. VF is set to 0 when there's a borrow, and 1 when there isn't.
void Opcode8XY7(WORD opcode)
{
	m_Registers[0xF] = 1 ;

	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;
	int regy = opcode & 0x00F0 ;
	regy >>= 4 ;

	if (m_Registers[regy] < m_Registers[regx])
		m_Registers[0xF] = 0 ;

	m_Registers[regx] = m_Registers[regy] - m_Registers[regx] ;
}

// Shifts VX left by one. VF is set to the value of the most significant bit of VX before the shift
void Opcode8XYE(WORD opcode)
{
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;

	m_Registers[0xF] = m_Registers[regx] >> 7 ;
	m_Registers[regx] <<= 1 ;

}

// skip next instruction if VX != VY
void Opcode9XY0(WORD opcode)
{
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;
	int regy = opcode & 0x00F0 ;
	regy >>= 4 ;

	if (m_Registers[regx] != m_Registers[regy])
		m_ProgramCounter += 2 ;
}

// set I to nnn
void OpcodeANNN(WORD opcode)
{
	m_AddressI = opcode & 0x0FFF ;
}

// jump to address NNN + V0
void OpcodeBNNN(WORD opcode)
{
	int nnn = opcode & 0x0FFF ;
	m_ProgramCounter = m_Registers[0] + nnn ;
}

// set vx to rand + NN
void OpcodeCXNN(WORD opcode)
{
	int nn = opcode & 0x00FF ;
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;

	m_Registers[regx] = rand() & nn ;
}

// Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels.
// As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn,
// and to 0 if that doesn't happen
void OpcodeDXYN(WORD opcode)
{
	const int SCALE = 10 ;
	int regx = opcode & 0x0F00 ;
	regx = regx >> 8 ;
	int regy = opcode & 0x00F0 ;
	regy = regy >> 4 ;

	int coordx = m_Registers[regx] * SCALE;
	int coordy = m_Registers[regy] * SCALE ;
	int height = opcode & 0x000F ;

	m_Registers[0xf] = 0 ;

	for (int yline = 0; yline < height; yline++)
	{
		// this is the data of the sprite stored at m_GameMemory[m_AddressI]
		// the data is stored as a line of bytes so each line is indexed by m_AddressI + yline
		BYTE data = (m_GameMemory[m_AddressI+yline]);

		// for each of the 8 pixels in the line
		int xpixel = 0 ;
		int xpixelinv = 7 ;
		for(xpixel = 0; xpixel < 8; xpixel++, xpixelinv--)
		{

			// is ths pixel set to 1? If so then the code needs to toggle its state
			int mask = 1 << xpixelinv ;
			if (data & mask)
			{
				int x = (xpixel*SCALE) + coordx ;
				int y = coordy + (yline*SCALE) ;

				int colour = 0 ;

				// a collision has been detected
				if (m_ScreenData[y][x][0] == 0)
				{
					colour = 255 ;
					m_Registers[15]=1;
				}

				// colour the pixel
				for (int i = 0; i < SCALE; i++)
				{
					for (int j = 0; j < SCALE; j++)
					{
						m_ScreenData[y+i][x+j][0] = colour ;
						m_ScreenData[y+i][x+j][1] = colour ;
						m_ScreenData[y+i][x+j][2] = colour ;
					}
				}

			}
		}
	}
}

//Skips the next instruction if the key stored in VX is pressed.
void OpcodeEX9E(WORD opcode)
{
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;
	int key = m_Registers[regx] ;

	if (m_KeyState[key] == 1)
		m_ProgramCounter+=2 ;
}

// Skips the next instruction if the key stored in VX isn't pressed.
void OpcodeEXA1(WORD opcode)
{
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;
	int key = m_Registers[regx] ;

	if (m_KeyState[key] == 0)
		m_ProgramCounter+=2 ;
}

// Sets VX to the value of the delay timer.
void OpcodeFX07(WORD opcode)
{
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;

	m_Registers[regx] = m_DelayTimer ;
}

// A key press is awaited, and then stored in VX.
void OpcodeFX0A(WORD opcode)
{
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;

	int keypressed = GetKeyPressed( ) ;

	if (keypressed == -1)
	{
		m_ProgramCounter -= 2 ;
	}
	else
	{
		m_Registers[regx] = keypressed ;
	}
}

// delay to vx
void OpcodeFX15(WORD opcode)
{
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;

	m_DelayTimer = m_Registers[regx] ;
}

// sound to vx
void OpcodeFX18(WORD opcode)
{
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;

	m_SoundTimer = m_Registers[regx] ;
}

// adds vx to I
void OpcodeFX1E(WORD opcode)
{
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;

	m_AddressI += m_Registers[regx] ;
}

//Sets I to the location of the sprite for the character in VX. Characters 0-F (in hexadecimal) are represented by a 4x5 font.
void OpcodeFX29(WORD opcode)
{
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;
	m_AddressI = m_Registers[regx]*5;
}

//Stores the Binary-coded decimal representation of VX at the addresses I, I plus 1, and I plus 2.
void OpcodeFX33(WORD opcode)
{
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;

	int value = m_Registers[regx] ;

	int hundreds = value / 100 ;
	int tens = (value / 10) % 10 ;
	int units = value % 10 ;

	m_GameMemory[m_AddressI] = hundreds ;
	m_GameMemory[m_AddressI+1] = tens ;
	m_GameMemory[m_AddressI+2] = units ;
}

// Stores V0 to VX in memory starting at address I.
void OpcodeFX55(WORD opcode)
{
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;

	for (int i = 0 ; i <= regx; i++)
	{
		m_GameMemory[m_AddressI+i] = m_Registers[i] ;
	}

	m_AddressI= m_AddressI+regx+1 ;
}

//Fills V0 to VX with values from memory starting at address I.
void OpcodeFX65(WORD opcode)
{
	int regx = opcode & 0x0F00 ;
	regx >>= 8 ;

	for (int i = 0 ; i <= regx; i++)
	{
		m_Registers[i] = m_GameMemory[m_AddressI+i]  ;
	}

	m_AddressI= m_AddressI+regx+1 ;
}

WORD GetNextOperationalCode(){
  WORD op = 0;
  op = GameMemory[Counter]; // no exemplo op é 0xAB
  op <<= 8; // desloca 8 bits para esquerda, op é 0xAB00
  op |= GameMemory[Counter + 1]; // op é 0xABCD
  Counter += 2;
  return op;
}

WORD opcode = GetNextOperationalCode(); // suponha que isso retorne 0x1234
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
