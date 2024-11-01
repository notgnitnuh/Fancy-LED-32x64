#include <avr/delay.h>
#include <avr/io.h>

//Helpful LCD control defines
#define LCD_Reset             0b00110000  //reset LCD to put in 4-bit mode
#define LCD_4bit_enable       0b00100000  //4-bit data - can't set the line display or fonts until this is set
#define LCD_4bit_mode         0b00101000  //2-line display, 5x8 font
#define LCD_4bit_displayOFF    0b00001000  //set display off
#define LCD_4bit_displayON     0b00001100  //set display on - no blink
#define LCD_4bit_displayON_B1  0b00001101  //set display on - with blink
#define LCD_4bit_displayCLEAR  0b00000001  //replace all chars with "space"
#define LCD_4bit_entryMODE     0b00000110  //set curser to write/read from left -> right
#define LCD_4bit_cursorSET     0b10000000  //set cursor position

//For two line mode
#define LineOneStart 0x00
#define LineTwoStart 0x40

// Pin definitions for PORTB control lines
#define LCD_EnablePin 1
#define LCD_RegisterSelectPin 0


void LCD_init();
void LCD_E_RS_init();
void LCD_write_4bits(uint8_t);
void LCD_EnablePulse(void);
void LCD_write_instruction(uint8_t);
void LCD_write_char(char);
void NewLine(uint16_t line);
void PrintString(char* string);
void LCD_write_line(char *line);

void LCD_init()
{
  //wait for power up - more than 30ms for vdd to rise to 4.5V
  _delay_ms(100);

  //we need to reset controller to enable 4-bit mode
  LCD_E_RS_init();  //Set the E and RS pins active low for each LCD reset

  //reset and wait for activiation
  LCD_write_4bits(LCD_Reset);
  _delay_ms(10);

  //now we can set the LCD to 4-bit mode
  LCD_write_4bits(LCD_4bit_enable);
  _delay_us(80);

  //set up LCD modes
  /*
  At this point we are operating in 4-bit mode
  (which means we have to send the high-nibble and low-nibble separate)
  and can now set the line numbers and font size
  */
  LCD_write_instruction(LCD_4bit_mode);
  _delay_us(80);  //delay must be > 39us

  LCD_write_instruction(LCD_4bit_displayCLEAR);
  _delay_ms(80);  //must be > 1.53ms

  LCD_write_instruction(LCD_4bit_entryMODE);
  _delay_us(80);  //must be > 39us

  LCD_write_instruction(LCD_4bit_displayON);
  _delay_us(80);  //must be > 39us
}


void LCD_E_RS_init()
{
  //set up the E and RS lines to active low for the reset function
  PORTB &= -(1<<LCD_EnablePin);
  PORTB &= -(1<<LCD_RegisterSelectPin);
}

void LCD_write_4bits(uint8_t Data)
{
  //only sends the data to the upper 4 bits of PORTA
  PORTA &= 0b00001111;  //clear upper nybble of PORTA
  PORTA |= Data;        //write data to data lines on PORTA

  //need to pulse enable to send data
  LCD_EnablePulse(); 
}

void LCD_write_instruction(uint8_t Instruction)
{
  //ensure RS is low
  //PORTB &= -(1<<LCD_RegisterSelectPin);
  LCD_E_RS_init();

  LCD_write_4bits(Instruction);   //write high nybble
  LCD_write_4bits(Instruction<<4);//write low nybble
}

// Pulse the Enable pin on the LCD controller to write/read the data lines - should be a t least 230ns pulse width //
void LCD_EnablePulse(void)
{
    // Set the enable bit low -> high -> low //
    //PORTB &= ~(1<<LCD_EnablePin); //Set enable low //
    //_delay_us(1); // what to ensure the pin is low //
    PORTB |= (1<<LCD_EnablePin); //Set enable high //
    _delay_us(1); // wait to ensure the pin is high //
    PORTB &= ~(1<<LCD_EnablePin); // Set enable low //
    _delay_us(1); // wait to ensure the pin is low //
}

void LCD_write_char(char Data)
{
    // Set up the E and RS lines for data writing //
    PORTB |= (1<<LCD_RegisterSelectPin); // Ensure RS pin is set high //
    PORTB &= ~(1<<LCD_EnablePin); // Ensure the enable pin is low //
    LCD_write_4bits(Data); //write the upper nybble //
    LCD_write_4bits(Data<<4); // write the lower nybble //
    _delay_us(80); // need to wait > 43us //
}

void PrintString(char* string)
{
  LCD_write_instruction(LCD_4bit_displayCLEAR);
  NewLine(LineOneStart);
  LCD_write_line(string);
  return;
}

//Switch to line given and clear
void NewLine(uint16_t line)
{
  LCD_write_instruction(LCD_4bit_cursorSET | line);
  _delay_us(10);
  for(int i=0; i<16; i++)
    LCD_write_char(' ');

  _delay_us(10);
  LCD_write_instruction(LCD_4bit_cursorSET | line);
}

void LCD_write_line(char *line)
{
  for(int i=0; i<16; i++)
    LCD_write_char(line[i]);
}