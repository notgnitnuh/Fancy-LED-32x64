#include <avr/interrupt.h>
#include <LED.c>
#include <stdlib.h>



void Init();
void rainfall();
void streaks();
void boarder_lines();


// int matrix2[16][64] = {0};

int real_main()
{
    Init();
    InitLED();
    srand(10);
    // matrix[1][1] = 0b11111111;


    boarder_lines();
    //streaks();
    rainfall();
    return 0;
}

void boarder_lines()
{
  Vector2 pos1 = {0,0};

  while(true)
  {
    updateBit(pos1,red);
    for(int i=0; i<31; i++)
    {
      _delay_ms(10);
      updateBit(pos1, off);
      pos1.x++;
      updateBit(pos1,red);
    }
    for(int i=0; i<63; i++)
    {
      _delay_ms(10);
      updateBit(pos1, off);
      pos1.y++;
      updateBit(pos1,red);
    }
    for(int i=31; i>0; i--)
    {
      _delay_ms(10);
      updateBit(pos1, off);
      pos1.x--;
      updateBit(pos1,red);
    }
    for(int i=63; i>0; i--)
    {
      _delay_ms(10);
      updateBit(pos1, off);
      pos1.y--;
      updateBit(pos1,red);
    }

  }

}

// void streaks()
// {

// }

void rainfall()
{  
    Note test[10];
    test[0] = {{rand() % 26 + 1, 0}, red, 3}; 
    test[1] = {{2,4}, blue, 3};
    test[2] = {{3,11}, green, 2};
    test[3] = {{4,18}, yellow, 1};
    test[4] = {{24,26}, green, 2};
    test[5] = {{26,32}, cyan, 2};
    test[6] = {{11,39}, green, 1};
    test[7] = {{4,45}, green, 1};
    test[8] = {{18,53}, white, 2};
    test[9] = {{26,60}, green, 1};
    while(true)
    {

        for(int i=0; i<72; i++)
        {
            _delay_ms(50);
            for(int j=0; j<10; j++)
            {
              if(test[j].pos.y > -10)
                dropNote(test[j]);
              else
              {
                test[j].pos.y = 67;
                test[j].pos.x = rand() % 27+1;
                test[j].notetype = rand() % 3+1;
              }
            }
            updateBrightness();
        }
    }

}


/* clock interrupt for matrix refresh */
ISR(TIMER1_OVF_vect)
{
  // test_function();
  writeDisplay();
}

void Init()
{
  // set timer interrupt stuffs
  InitMatrixRefresh();

  // set PORTB for output (Matrix display)
  DDRB = 0xFF;
  // Set PORTB, PORTD, PORTL to known states
  PORTB = 0xFF;

  //Initialize External Interrupts on PORTD3:0
  sei();  //enable global interrupt enable flag so that interrupts can be processed
}