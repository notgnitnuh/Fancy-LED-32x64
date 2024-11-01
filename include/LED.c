#include <avr/io.h>
#include <avr/delay.h>

//vector for coordinates
struct Vector2
{
	int x;
	int y;

};
  
inline bool operator==(const Vector2& lhs, const Vector2& rhs)
{
  if(lhs.x == rhs.x && lhs.y == rhs.y)
    return true;
  else
    return false;
}

//Data necessary to write and maniuplate notes
struct Note
{
  Vector2 pos;
  uint8_t RGB;
  int notetype;
};

//function declarations
void clearDisplay();
void drawNote(Vector2 pos, uint8_t RGB, int notetype);
void drawNote1(Vector2 pos, uint8_t RGB);
void drawNote2(Vector2 pos, uint8_t RGB);
void drawNote3(Vector2 pos, uint8_t RGB);
void dropNote(Vector2 &pos);
void InitLED();
void InitMatrixRefresh();
void updateBrightness();
void updateBit(Vector2 POS, int RGB);
void writeBox(Vector2 coord1, Vector2 coord2, uint8_t RGB);
void writeDisplay();

//x,y,brightness
int matrix[16][64] = {0};
int matrixBrightness[32][64] = {0};
int brightlevel = 0;

enum color{red=0b00100100, blue=0b10010000, green=0b01001000, yellow=0b01101100, cyan=0b11011000, pink=0b10110100, white=0b11111100, off=0}; //for LED colors

#define CLOCK = 11; //PB5
#define LATCH = 10; //PB4
#define OE = 9;     //PH6

/*
    R1 = 24 PA2
    G1 = 25 PA3
    B1 = 26 PA4
    R2 = 27 PA5
    G2 = 28 PA6
    B2 = 29 PA7
*/
//draw a predesigned note at position pos of color RGB
void drawNote(Vector2 pos, uint8_t RGB, int notetype)
{
  switch (notetype)
  {
  case 1:
    drawNote1(pos, RGB);
    break;
  case 2:
    drawNote2(pos, RGB);
    break;
  case 3:
    drawNote3(pos, RGB);
    break;
  default:
    break;
  }
  return;
}

//default note designed for class
void drawNote1(Vector2 pos, uint8_t RGB)
{
  //create box with pos at bottom left
  for(uint8_t j = 0; j < 4; j++)
  {
      for(uint8_t i = 0; i < 4; i++)
      {
        //if(i+j < 5)
          updateBit({pos.x+i,pos.y+j}, RGB);
      }
  }

  //make white box inside colored box
  for(uint8_t i = 0; i < 2; i++)
  {
    for(uint8_t j = 0; j < 2; j++)
    {
      if(RGB != off)
        updateBit({pos.x+i+1,pos.y+j+1}, white);
    }

    //add lines to sides of squares
    updateBit({pos.x-1,pos.y+1+i}, RGB);  //left
    updateBit({pos.x+4,pos.y+1+i}, RGB);  //right
  }
}

//simple 8th note design
void drawNote2(Vector2 pos, uint8_t RGB)
{
  updateBit({pos.x + 1, pos.y}, RGB);
  updateBit({pos.x + 1, pos.y+1}, RGB);
  updateBit({pos.x, pos.y+1}, RGB);
  updateBit({pos.x + 1, pos.y+2}, RGB);
  updateBit({pos.x + 2, pos.y+1}, RGB);

  for(int i=0; i<5; i++)
  {
    updateBit({pos.x+2, pos.y + 2+i}, RGB);
  }
  
  updateBit({pos.x + 3, pos.y+6}, RGB);
  updateBit({pos.x + 4, pos.y+5}, RGB);
  updateBit({pos.x + 4, pos.y+4}, RGB);
  updateBit({pos.x + 5, pos.y+3}, RGB);
}

//simple 8th note design
void drawNote3(Vector2 pos, uint8_t RGB)
{
  updateBit({pos.x, pos.y+1}, RGB);
  updateBit({pos.x+1, pos.y}, RGB);
  updateBit({pos.x+2, pos.y}, RGB);
  updateBit({pos.x+3, pos.y}, RGB);
  updateBit({pos.x+4, pos.y+1}, RGB);
  for(int i=0; i<2; i++)
  {
    updateBit({pos.x+1, pos.y+3+i}, RGB);
    updateBit({pos.x+3, pos.y+3+i}, RGB);
  }
}

//draw box with coord1=bottom left, coord2= upper right
void writeBox(Vector2 coord1, Vector2 coord2, uint8_t RGB)
{
  for(int i=0; i<coord2.x - coord1.x+1; i++)
  {
    updateBit({coord1.x +i, coord1.y}, RGB);
    updateBit({coord1.x +i, coord2.y}, RGB);
  }
  for(int i=0; i<coord2.y - coord1.y; i++)
  {
    updateBit({coord1.x, coord1.y+i}, RGB);
    updateBit({coord2.x, coord1.y+i}, RGB);
  }
}

//expand box from center to outside
void expandBox(Vector2 coord1, Vector2 coord2, uint8_t RGB)
{
  int width = (coord2.x - coord1.x) / 2;
  for(int i=0; i< width; i++)
  {
    writeBox({coord1.x+width-i, coord1.y}, {coord2.x-width+i, coord2.y}, RGB);
    _delay_ms(5);
    writeBox({coord1.x+width-i, coord1.y}, {coord2.x-width+i, coord2.y}, off);
  }
  writeBox(coord1, coord2, RGB);
}

//erase note, and rewrite down one pixel
void dropNote(Note &note)
{
  drawNote(note.pos, off, note.notetype);
  note.pos.y -= 1;
  drawNote(note.pos, note.RGB, note.notetype);
}


//write matrix array to display
void writeDisplay()
{
  for(int j=0; j<16; j++)
  {
    PORTF = j;  //select address

    for (int i=0; i<64; i++)
    {
        // if((matrixBrightness[j][i] & (1<<brightlevel)) != 1 )
        //   PORTA = 0;
        // else
          PORTA = matrix[j][i]; //write byte
        // if(j==15)
        //   PORTA = off;
        
        PORTB |= 0b00100000;  //set CLK
        PORTB &= 0b11011111;  //clear CLK
        
        // PORTA = off;
        // PORTB |= 0b00100000;  //set CLK
        // PORTB &= 0b11011111;  //clear CLK
    }

    clearDisplay(); //necessary for some reason
    PORTH = 0x00;

    PORTB |= 0b00010000;  //Set latch
    PORTB &= 0b11101111;  //clear latch
  } 
  brightlevel++;
  if(brightlevel > 8)
    brightlevel = 0;

    
}

//give x and y coordinate, and enumed RGB value
//x value slightly goofy, addresses 0-1 and 14-15 are swapped on our matrix
void updateBit(Vector2 POS, int RGB)
{
  if(POS.y > 63 || POS.y < 0 || POS.x < 0 || POS.x > 31)   //error check
    return;

  int temp_color = 0;
  matrixBrightness[POS.x][POS.y] = 0xFFFF;

  if(POS.x < 16)
  {
    RGB &= 0b00011100;                                    //keep only lower RGB bits
    if(POS.x<2)
    {
      temp_color = matrix[POS.x+14][POS.y] & 0b11100011;
      matrix[POS.x+14][POS.y] = temp_color | RGB;
    }
    else
    {
      temp_color = matrix[POS.x-2][POS.y] & 0b11100011;   //clear lower RGB bits
      matrix[POS.x-2][POS.y] = temp_color | RGB;          //or new value into lower RGB bits
    }
  }
  else
  {
    RGB &= 0b11100000;                                    //keep only upper RGB bits
    if(POS.x<18)
    {
      temp_color = matrix[POS.x-2][POS.y] & 0b00011111;   //clear higher RGB bits
      matrix[POS.x-2][POS.y] = temp_color | RGB;
    }
    else
    {
      temp_color = matrix[POS.x-18][POS.y] & 0b00011111;
      matrix[POS.x-18][POS.y] = temp_color | RGB;         //or new value into higher RGB bits
    }
  }
}

void updateBrightness()
{
  for(int i=0; i<32; i++)
  {
    for(int j=0; j<64; j++)
      matrixBrightness[i][j] = matrixBrightness[i][j] >> 1;
  }
}

//turn off OE
void clearDisplay()
{
  PORTH = 0xFF;
}

//Initialize the LED Matrix by first setting port direction and initializing to a known state
void InitLED()
{
  DDRA = 0xFF;
  DDRB = 0xFF;
  DDRH = 0xFF;
  DDRF = 0xFF;

  PORTA = 0x00; //RGB
  PORTB = 0x00; //clk, latch
  PORTH = 0x00; //OE
  PORTF = 0x00; //Address
}

void InitMatrixRefresh()
{
  /* Timer for LED matrix refresh */
  TCCR1A = 0;    // set entire TCCR1A register to 0
  TCCR1B = 0;    // set entire TCCR1B register to 0 
  // enable Timer1 overflow interrupt:
  TIMSK1 |= (1 << TOIE1); //Atmega8 has no TIMSK1 but a TIMSK register
  // Set CS10 bit so timer runs at clock speed: (no prescaling)
  TCCR1B |= (1 << CS10); // Sets bit CS10 in TCCR1B
}