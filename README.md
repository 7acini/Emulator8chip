# CHIP-8

CHIP-8 é uma linguagem de programação interpretada, desenvolvida por Joseph Weisbecker em seu microprocessador 1802. Foi inicialmente usada nos microcomputadores COSMAC VIP e Telmac 1800, que eram microcomputadores de 8 bits fabricados no meio da década de 1970.

CHIP-8 foi projetado para ser fácil de programar, além de usar menos memória do que outras linguagens de programação como BASIC. 

Interpretadores foram criados para muitos dispositivos, como computadores domésticos, microcomputadores, calculadoras gráficas, telefones celulares e consoles de videogame.

## Comunidade

CHIP-8 foi utilizado em uma ampla gama de dispositivos ao longo do tempo, e a primeira comunidade a utilizar CHIP-8 começou na década de 1970 em microcomputadores. Eles compartilhavam extensões e programas em boletins informativos como o VIPER da ARESCO para usuários do COSMAC VIP ou o DREAMER para usuários do DREAM 6800. No boletim informativo VIPER, os três primeiros números detalhavam o código de máquina para o interpretador CHIP-8 para o VIP.

Na década de 1990, interpretadores CHIP-8 começaram a ser criados para calculadoras gráficas. Exemplos notáveis incluem CHIP-48 e SCHIP para o HP-48.

As aplicações CHIP-8 incluem jogos originais, demos, bem como recriações de jogos populares de outros sistemas. Com algumas aplicações CHIP-8 sendo de domínio público, usando licenças como a licença Creative Commons Zero.

## Extensões e Variações CHIP-8

Durante os anos 1970 e 1980, os usuários de CHIP-8 compartilhavam programas CHIP-8, mas também mudanças e extensões para o interpretador CHIP-8, como na revista VIPER para o COSMAC VIP. Essas extensões incluíam CHIP-10 e Hi-Res CHIP-8, que introduziam uma resolução mais alta do que o padrão 64x32, e CHIP-8C e CHIP-8X, que estendiam as capacidades de exibição monocromática para suportar cores limitadas, entre outras características.

## Descrição da Máquina Virtual

### Memória

CHIP-8 era mais comumente implementado em sistemas de 4K, como o Cosmac VIP e o Telmac 1800. Essas máquinas tinham 4096 (0x1000) locais de memória, todos com 8 bits (um byte), de onde vem o termo CHIP-8. No entanto, o interpretador CHIP-8 em si ocupava os primeiros 512 bytes do espaço de memória nessas máquinas. Por esse motivo, a maioria dos programas escritos para o sistema original começa na localização da memória 512 (0x200) e não acessa nenhuma memória abaixo da localização 512 (0x200). Os 256 bytes superiores (0xF00-0xFFF) são reservados para atualização da tela, e os 96 bytes abaixo disso (0xEA0-0xEFF) foram reservados para a pilha de chamadas, uso interno e outras variáveis.

### Registradores

CHIP-8 possui 16 registradores de dados de 8 bits nomeados V0 a VF. O registro VF funciona também como uma flag para algumas instruções; assim, deve ser evitado. Em uma operação de adição, VF é a flag de carry, enquanto na subtração, é a flag "sem empréstimo". Na instrução de desenho, VF é definido em caso de colisão de pixels.

### Pilha

A pilha é usada apenas para armazenar endereços de retorno quando sub-rotinas são chamadas. A versão original do RCA 1802 alocava 48 bytes para até 12 níveis de aninhamento; implementações modernas geralmente têm mais.

### Temporizadores

CHIP-8 possui dois temporizadores. Ambos contam para baixo a 60 hertz, até chegarem a 0.

- Temporizador de atraso: Este temporizador é destinado a ser usado para cronometrar os eventos dos jogos. Seu valor pode ser definido e lido.
- Temporizador de som: Este temporizador é usado para efeitos sonoros. Quando seu valor não é zero, um som de bipe é emitido. Seu valor só pode ser definido.

### Entrada

A entrada é feita com um teclado hexadecimal que possui 16 teclas variando de 0 a F. As teclas '8', '4', '6' e '2' são tipicamente usadas para entrada direcional. Três opcodes são usados para detectar entrada. Um salta uma instrução se uma tecla específica for pressionada, enquanto outro faz o mesmo se uma tecla específica não for pressionada. O terceiro aguarda uma pressão de tecla e, em seguida, a armazena em um dos registradores de dados.

### Gráficos e som

A resolução original da tela CHIP-8 é de 64x32 pixels, e a cor é monocromática. Os gráficos são desenhados na tela exclusivamente desenhando sprites, que têm 8 pixels de largura e podem ter de 1 a 15 pixels de altura. Os pixels do sprite são XOR'd com os pixels da tela correspondentes. Em outras palavras, os pixels do sprite que estão definidos invertem a cor do pixel da tela correspondente, enquanto os pixels do sprite que não estão definidos não fazem nada. A flag de carry (VF) é definida como 1 se algum pixel da tela for alterado de definido para não definido quando um sprite é desenhado e definida como 0 caso contrário. Isso é usado para detecção de colisão.

Como descrito anteriormente, um som de bipe é reproduzido quando o valor do temporizador de som não é zero.

### Tabela de Opcodes

CHIP-8 possui 35 opcodes, que têm todos dois bytes de comprimento e são armazenados em big-endian. Os opcodes estão listados abaixo, em hexadecimal e com os seguintes símbolos:

- NNN: endereço
- NN: constante de 8 bits
- N: constante de 4 bits
- X e Y: identificador de registrador de 4 bits
- PC: Contador de Programa
- I: registrador de 12 bits (para endereço de memória)

Houve muitas implementações do conjunto de instruções CHIP-8 desde 1978. A seguinte especificação é baseada na especificação SUPER-CHIP de 1991 (mas sem os opcodes adicionais que fornecem funcionalidades estendidas), pois essa é a extensão mais comumente encontrada hoje. As notas de rodapé indicam incompatibilidades com o conjunto de instruções CHIP-8 original de 1978.

| Opcode | Tipo    | C Pseudo     | Explicação                                                                                                                                                       |
|--------|---------|--------------|------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| 0NNN   | Call    |              | Chama rotina de código de máquina (RCA 1802 para COSMAC VIP) no endereço NNN. Não é necessário para a maioria das ROMs.                                        |
| 00E0   | Display | disp_clear() | Limpa a tela.                                                                                                                                                   |
| 00EE   | Fluxo   | return;      | Retorna de uma sub-rotina.                                                                                                                                       |
| 1NNN   | Fluxo   | goto NNN;    | Salta para o endereço NNN.                                                                                                                                      |
| 2NNN   | Fluxo   | *(0xNNN)()   | Chama sub-rotina em NNN.                                                                                                                                        |
| 3XNN   | Cond    | if (Vx == NN) | Pula a próxima instrução se VX for igual a NN (geralmente a próxima instrução é um salto para pular um bloco de código).                                       |
| 4XNN   | Cond    | if (Vx != NN) | Pula a próxima instrução se VX não for igual a NN (geralmente a próxima instrução é um salto para pular um bloco de código).                                   |
| 5XY0   | Cond    | if (Vx == Vy)| Pula a próxima instrução se VX for igual a VY (geralmente a próxima instrução é um salto para pular um bloco de código).                                       |
| 6XNN   | Const   | Vx = NN      | Define VX para NN.                                                                                                                                               |
| 7XNN   | Const   | Vx += NN     | Adiciona NN a VX (a flag de carry não é alterada).                                                                                                               |
| 8XY0   | Assig   | Vx = Vy      | Define VX com o valor de VY.                                                                                                                                    |
| 8XY1   | BitOp   | Vx \|= Vy    | Define VX como VX ou VY. (operação OR bit a bit).                                                                                                                |
| 8XY2   | BitOp   | Vx &= Vy    | Define VX como VX e VY. (operação AND bit a bit).                                                                                                                |
| 8XY3[a] | BitOp   | Vx ^= Vy    | Define VX como VX xor VY.                                                                                                                                       |
| 8XY4   | Math    | Vx += Vy     | Adiciona VY a VX. VF é definido como 1 quando há um overflow e 0 quando não há.                                                                                  |
| 8XY5   | Math    | Vx -= Vy     | VY é subtraído de VX. VF é definido como 0 quando há um underflow e 1 quando não há. (ou seja, VF é definido como 1 se VX >= VY e 0 se não).                   |
| 8XY6[a] | BitOp   | Vx >>= 1    | Desloca VX para a direita em 1, então armazena o bit menos significativo de VX antes do deslocamento em VF.                                                   |
| 8XY7[a] | Math    | Vx = Vy - Vx | Define VX como VY menos VX. VF é definido como 0 quando há um underflow e 1 quando não há. (ou seja, VF é definido como 1 se VY >= VX).                      |
| 8XYE[a] | BitOp   | Vx <<= 1    | Desloca VX para a esquerda em 1, então define VF como 1 se o bit mais significativo de VX antes desse deslocamento estiver definido, ou 0 se não estiver definido. |
| 9XY0   | Cond    | if (Vx != Vy)| Pula a próxima instrução se VX não for igual a VY. (Geralmente a próxima instrução é um salto para pular um bloco de código).                                |
| ANNN   | MEM     | I = NNN      | Define I para o endereço NNN.                                                                                                                                    |
| BNNN   | Fluxo   | PC = V0 + NNN| Salta para o endereço NNN mais V0.                                                                                                                               |
| CXNN   | Rand    | Vx = rand() & NN | Define VX para o resultado de uma operação AND bit a bit em um número aleatório (Normalmente: 0 a 255) e NN.                                                    |
| DXYN   | Display | draw(Vx, Vy, N)| Desenha um sprite na coordenada (VX, VY) que tem uma largura de 8 pixels e uma altura de N pixels. Cada linha de 8 pixels é lida como codificada por bits a partir da localização de memória I; o valor de I não muda após a execução dessa instrução. Como descrito anteriormente, VF é definido como 1 se algum pixel da tela for alterado de definido para não definido quando o sprite é desenhado e como 0 se isso não acontecer. |
| EX9E   | KeyOp   | if (key() == Vx) | Pula a próxima instrução se a tecla armazenada em VX for pressionada (geralmente a próxima instrução é um salto para pular um bloco de código).             |
| EXA1   | KeyOp   | if (key() != Vx) | Pula a próxima instrução se a tecla armazenada em VX não for pressionada (geralmente a próxima instrução é um salto para pular um bloco de código).         |
| FX07   | Timer   | Vx = get_delay() | Define VX para o valor do temporizador de atraso.                                                                                                                |
| FX0A   | KeyOp   | Vx = get_key() | Uma pressão de tecla é aguardada e, em seguida, armazenada em VX (operação de bloqueio, todas as instruções são interrompidas até o próximo evento de tecla).             |
| FX15   | Timer   | delay_timer(Vx)| Define o temporizador de atraso para VX.                                                                                                                          |
| FX18   | Sound   | sound_timer(Vx)| Define o temporizador de som para VX.                                                                                                                             |
| FX1E   | MEM     | I += Vx       | Adiciona VX a I. VF não é afetado.                                                                                                                               |
| FX29   | MEM     | I = sprite_addr[Vx] | Define I para a localização do sprite para o caractere em VX. Os caracteres 0-F (

em hexadecimal) são representados por uma fonte de 4x5.                                                                           |
| FX33   | BCD     |              | Armazena a representação decimal codificada em binário de VX, com o dígito das centenas na memória no local I, o dígito das dezenas no local I+1 e o dígito das unidades no local I+2. |
| FX55   | MEM     | reg_dump(Vx, &I) | Armazena de V0 a VX (inclusive VX) na memória, começando no endereço I. O deslocamento de I é aumentado em 1 para cada valor escrito, mas I mesmo não é modificado. |
| FX65   | MEM     | reg_load(Vx, &I)| Preenche de V0 a VX (inclusive VX) com valores da memória, começando no endereço I. O deslocamento de I é aumentado em 1 para cada valor lido, mas I mesmo não é modificado. |
  
Observações:

- Os opcodes lógicos 8XY3, 8XY6, 8XY7 e 8XYE não foram documentados na especificação original do CHIP-8, pois todos os opcodes 8000 eram despachados para instruções na ALU do 1802, e não localizados no próprio interpretador; portanto, esses quatro opcodes adicionais foram presumivelmente funcionalidades não intencionais.
- Os opcodes de deslocamento de bits 8XY6 e 8XYE (as instruções de deslocamento de bits), que na verdade eram opcodes não documentados no interpretador original, deslocavam o valor no registrador VY e armazenavam o resultado em VX. As implementações CHIP-48 e SCHIP, em vez disso, ignoravam VY e simplesmente deslocavam VX.
- A maioria das instruções FX1E dos interpretadores CHIP-8 não afeta VF, com uma exceção: o interpretador CHIP-8 para o Commodore Amiga define VF como 1 quando há um overflow de intervalo (I+VX>0xFFF), e como 0 quando não há. O único jogo conhecido que depende desse comportamento é Spacefight 2091!, enquanto pelo menos um jogo, Animal Race, depende de VF não ser afetado.
- Na implementação original do CHIP-8, e também no CHIP-48, I é deixado incrementado após a execução dessa instrução. No SCHIP, I é deixado inalterado.
