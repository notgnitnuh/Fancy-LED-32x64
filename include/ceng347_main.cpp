#include <avr/interrupt.h>
#include <LED.c>
#include <score.c>

//LED matrix info https://www.waveshare.com/wiki/RGB-Matrix-P3-64x32
//Solenoid Control info https://media.digikey.com/pdf/Data%20Sheets/Adafruit%20PDFs/412_Web.pdf
int ceng347_main();
void Init();
void InitSolenoidTimer();
void fire_solenoid();
bool updateGame(Note *allNotes, Note note, uint8_t dropRate);
void generateOutput(uint8_t speed);
void checkColumn(Note *allNotes, uint8_t column);

//drop delay gives delay before drop
struct Song{
  uint8_t column;
  uint8_t dropDelay;
};


//map
volatile uint8_t hit_mask = 0x00; //for interrupts
volatile uint16_t ticks;          //for timing
volatile uint64_t score;          //name says it all

//difficulty levels for note speed
enum difficulty {expert = 15, hard =20, medium=25, easy=30 };


int ceng347_main()
{
  score = 50;
  Init();
  InitLED();
  InitScore();
  int songSelect = 2; //select song for play

  /* allNotes=array of notes, 16 max; startNotes=column start enums */
  Note allNotes[16] = {0};
  Note startNotes[4] = {{{2, 65}, red, true},{{10, 65}, green, true},{{18, 65}, blue, true},{{26, 65}, yellow, true}};

  /* initial demo songs, noteNum keeps track of note to output */
  Song song0[12] = {{0,10},{1,10},{2,10},{3,10},{2,10},{1,10},{0,10},{1,10},{2,10},{3,10},{2,10},{1,10}};
  Song song1[22] = {{0,7},{0,7},{0,7},{0,7},{1,7},{1,7},{1,7},{0,7},{1,7},{2,7},{3,10},{3,10},{3,15},{3,7},{3,7},{3,7},{0,7},{2,7},{1,7},{3,10},{2,7},{1,25}};
  Song song2[8] = {{2,10},{3,10},{0,10},{3,10},{1,10},{1,10},{0,10},{3,10}};
  Song song3[50] = {{2,14},{0,14},{1,7},{1,7},{0,7},{1,14},{1,7},{0,7},{1,7},{3,14},{3,14}};
  uint8_t noteNum = 0;

  //main loop
  while(1)
  {
    /*
    Update Game - (Game Logic, Physics, )
    Generate Output - (Generates output to the LED Matrix)
    */

    //update game with premade song, TODO: should be cleaned up in the future
    if(songSelect == 1)
    {
      //update game, if new note written, increment noteNum
      if(updateGame(allNotes, startNotes[song1[noteNum].column], song1[noteNum].dropDelay))
        noteNum++;
      if(noteNum >= 22)  //cycling through song
        noteNum = 0;
    }   
    else if(songSelect == 2)
    {
      //update game, if new note written, increment noteNum
      if(updateGame(allNotes, startNotes[song2[noteNum].column], song2[noteNum].dropDelay))
        noteNum++;
      if(noteNum >= 8)  //cycling through song
        noteNum = 0;
    }   
    else if(songSelect == 3)
    {
      //update game, if new note written, increment noteNum
      if(updateGame(allNotes, startNotes[song3[noteNum].column], song3[noteNum].dropDelay))
        noteNum++;
      if(noteNum >= 11)  //cycling through song
        noteNum = 0;
    }
    else
    {
      //update game, if new note written, increment noteNum
      if(updateGame(allNotes, startNotes[song0[noteNum].column], song0[noteNum].dropDelay))
        noteNum++;
      if(noteNum >= 12)  //cycling through song
        noteNum = 0;
    }

    //update ticks, 
    generateOutput(medium);
  }

  return 1;
}


/* allNotes=array of alive/dead notes; note=next note to fall */
bool updateGame(Note *allNotes, Note note, uint8_t dropRate)
{
  bool noteFound = false;

  //If enough time has passed, spawn another note
  if(ticks > dropRate)
  {
    ticks = 0;
    //Find the first open position and initialize the note with pos, color, and alive
    for(int i=0; !noteFound && i<16; i++)
    {
      if(allNotes[i].alive == false)
      {
        allNotes[i] = note;
        noteFound = true; 
      }
    }
  }

  //Apply physics to all notes
  for(int i = 0; i < 16; i++)
  {
    if (allNotes[i].alive)
      dropNote(allNotes[i]);
  }

  //Note cleanup
  for (int i = 0; i < 16; i++)
  {
    if((allNotes[i].pos.y < -6) && (allNotes[i].alive))
    {
      if (score > 0)
      {
        score -= 5;
      }
      allNotes[i].alive = false;
    }
  }

  //write base boundary
  writeBox({0,2},{7,7}, pink);
  writeBox({8,2},{15,7}, pink);
  writeBox({16,2},{23,7}, pink);
  writeBox({24,2},{31,7}, pink);

  //If interrupt received, perform a check to see if a note was hit correctly
  if(hit_mask & 0x01)
    checkColumn(allNotes, 0);
  if(hit_mask & 0x02)
    checkColumn(allNotes, 1);
  if(hit_mask & 0x04)
    checkColumn(allNotes, 2);
  if(hit_mask & 0x08)
    checkColumn(allNotes, 3);
  
  //call solenoid function to fire if interrupt given
  fire_solenoid();

  return noteFound;
}

/* see if any notes are within range of target, adjust score appropriately */
void checkColumn(Note *allNotes, uint8_t column)
{
  bool hitAny = false;
  for(int i=0; i<16; i++)
  { //LED column coordinates: 2, 10, 18, 26
    //Make sure note is in the correct column
    if(allNotes[i].alive && (allNotes[i].pos.x == column*8+2))
    {
      //check y value
      if(allNotes[i].pos.y==2)          //perfect
      { 
        score += 10;                                  //update score
        drawNote(allNotes[i].pos, off);               //If in hit box, erase note
        allNotes[i].alive = false;                    //set note to dead
        hitAny = true;                                //mark that note was hit
        expandBox({column*8,2},{column*8+8, 7}, blue);//animate hit box
      }
      else if(allNotes[i].pos.y>=-1 && allNotes[i].pos.y<= 6) //good
      {
        score += 5;                                   //update score
        drawNote(allNotes[i].pos, off);               //If in hit box, erase note
        allNotes[i].alive = false;                    //set to dead
        hitAny = true;                                //mark that note was hit
        expandBox({column*8,2},{column*8+8, 7}, green); //animate hit box
      }
    }
  }

  if(!hitAny && score >= 5)    //miss
    score -=5;                 //adjust score

}


/* increment ticks at given speed and output score */
void generateOutput(uint8_t speed)
{
  //Write the display
  _delay_ms(speed);
  ticks += 1;
  displayMeter(score);
}

/* write out hit_mask to solenoid pins */
void fire_solenoid()
{
  //Enable timer
  TCCR3B = 0x0B;
  PORTL |= hit_mask;
}

/* clock interrupt for matrix refresh */
ISR(TIMER1_OVF_vect)
{
  writeDisplay();
}

ISR(TIMER3_COMPA_vect)
{
  PORTL &= 0x00;  //turn off solenoids
  hit_mask = 0x00;//clear hit mask
  TCCR3B &= 0xF8; //disable timer
  TCNT3 = 0x0000; //ensure timer count is 0
}

/* interrupts mask */
ISR(INT0_vect)
{
  TCCR3B |= (1 << CS10);  // Sets bit CS10 in TCCR3B to start counting
  hit_mask |= 0x01;       // Set solenoid mask bit
  fire_solenoid();
}
ISR(INT1_vect)
{
  TCCR3B |= (1 << CS10);  // Sets bit CS10 in TCCR3B to start counting
  hit_mask |= 0x02;       // Set solenoid mask bit
  fire_solenoid();
}
ISR(INT2_vect)
{
  TCCR3B |= (1 << CS10);  // Sets bit CS10 in TCCR3B to start counting
  hit_mask |= 0x04;       // Set solenoid mask bit
  fire_solenoid();
}
ISR(INT3_vect)
{
  TCCR3B |= (1 << CS10);  // Sets bit CS10 in TCCR3B to start counting
  hit_mask |= 0x08;       // Set solenoid mask bit
  fire_solenoid();
}

void Init()
{
  // set timer interrupt stuffs
  InitMatrixRefresh();
  InitSolenoidTimer();

  // set PORTB for output (Matrix display)
  DDRB = 0xFF;
  // set PORTL for output (solenoids)
  DDRL = 0xFF;
  // set PORTD 3:0 as input to read IR beam
  DDRD = 0x00;
  // Set PORTB, PORTD, PORTL to known states
  PORTB = 0xFF;
  PORTD = 0xFF;
  PORTL = 0x00;

  //Initialize External Interrupts on PORTD3:0
  sei();  //enable global interrupt enable flag so that interrupts can be processed
  //set each interrupt in portd3:0 to be triggered on a both edges to catch beam break, and beam connect
  EICRA = 0xAA;

  //enable interrupt on PORTD0
  EIMSK |= 0x0F;
}


void InitSolenoidTimer()
{
  //initialize CTC Mode with no output compare pins
  TCCR3A = 0x00;
  TCCR3B = 0x00;//0x0B
  TCNT3 = 0x0000;
  TIMSK3 |= (1 << OCIE3A);

  //set to 20ms cycle and 1.5ms 
  OCR3A = 50000;
  OCR3B = 50005;
}