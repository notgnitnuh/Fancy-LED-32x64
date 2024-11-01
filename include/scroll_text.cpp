#include <avr/interrupt.h>
#include <LED.c>
#include <stdlib.h>
#include <ctype.h>

using namespace std;

void scroll_text(char* line, int size);
void Init();
void scroll_main(char* line, int size);
int draw_letter(Vector2 pos, char letter, int RGB);

void scroll_main(char* line, int size)
{
    Init();
    InitLED();
    while(true)
        scroll_text(line, size);
}

void scroll_text(char* line, int size)
{
    color blink[6] = {blue, cyan, red, pink, yellow, green};
    // char* line2 = "       ";
    char line2[size+16]; 
    for(int i=0; i<8; i++)
        line2[i] = ' ';
    for(int i=8; i<8+size; i++)
        line2[i] = tolower(line[i-8]);
    for(int i=8+size; i<size+16; i++)
        line2[i] = ' ';



    // for(int i=0; i<size; i++) 
    //     line2[i+7] = line[i];

    for(int q=0; q<6; q++)
    {
        for(int i=0; i<size+8; i++)
        {
            for(int j=9; j>=0; j--)
            {
                draw_letter({2,60+j}, line2[i+7], blink[q]);
                draw_letter({2,50+j}, line2[i+6], blink[q]);
                draw_letter({2,40+j}, line2[i+5], blink[q]);
                draw_letter({2,30+j}, line2[i+4], blink[q]);
                draw_letter({2,20+j}, line2[i+3], blink[q]);
                draw_letter({2,10+j}, line2[i+2], blink[q]);
                draw_letter({2,j}, line2[i+1], blink[q]);
                draw_letter({2,-10+j}, line2[i], blink[q]);
                _delay_ms(10); // Draw letter is super slow, can read text without any delay, noticeable speed depending on letters.
                draw_letter({2,60+j}, line2[i+7], off);
                draw_letter({2,50+j}, line2[i+6], off);
                draw_letter({2,40+j}, line2[i+5], off);
                draw_letter({2,30+j}, line2[i+4], off);
                draw_letter({2,20+j}, line2[i+3], off);
                draw_letter({2,10+j}, line2[i+2], off);
                draw_letter({2,j}, line2[i+1], off);
                draw_letter({2,-10+j}, line2[i], off);
            }
        }
    }


    // writeBox({1,1}, {19,10}, green);
    // for(int i=0; i<6; i++)
    // {
    //     draw_letter({2, 2}, 'j', blink[i]);
    //     draw_letter({2, 12}, 'o', blink[i]);
    //     draw_letter({2, 22}, 's', blink[i]);
    //     draw_letter({2, 32}, 'i', blink[i]);
    //     draw_letter({2, 42}, 'a', blink[i]);
    //     draw_letter({2, 52}, 'h', blink[i]);
    //     _delay_ms(1000);
    // }
    // for(int i=0; i<size; i++)
    // {
    //     draw_letter({2, i*2}, tolower(line[i]), blink[q]);
    //     _delay_ms(300);
    //     draw_letter({2, i*2}, tolower(line[i]), off);
    // }
}

int draw_letter(Vector2 pos, char letter, int RGB)
{
    switch(letter)
    {
    case('a'):
        writeBox({pos.x, pos.y+1}, {pos.x+7, pos.y+6}, RGB);
        writeBox({pos.x+1, pos.y}, {pos.x+6, pos.y+7}, RGB);
        writeBox({pos.x+6, pos.y}, {pos.x+15, pos.y+1}, RGB);
        writeBox({pos.x+6, pos.y+6}, {pos.x+15, pos.y+7}, RGB);
        break;
    case('b'):
        writeBox({pos.x, pos.y},{pos.x+6, pos.y+6}, RGB);
        writeBox({pos.x+6, pos.y},{pos.x+15, pos.y+6}, RGB);
        writeBox({pos.x+1, pos.y+1},{pos.x+5, pos.y+7}, RGB);
        writeBox({pos.x+7, pos.y+1},{pos.x+14, pos.y+7}, RGB);
        break;
    case('c'):
        writeBox({pos.x+2, pos.y}, {pos.x+13, pos.y+1}, RGB);
        writeBox({pos.x, pos.y+2}, {pos.x+1, pos.y+7}, RGB);
        writeBox({pos.x+14, pos.y+2}, {pos.x+15, pos.y+7}, RGB);
        updateBit({pos.x+1, pos.y+1}, RGB);
        updateBit({pos.x+14, pos.y+1}, RGB);
        break;
    case('d'):
        writeBox({pos.x, pos.y}, {pos.x+15, pos.y+6}, RGB);
        writeBox({pos.x+1, pos.y+1}, {pos.x+14, pos.y+7}, RGB);
        break;
    case('e'):
        writeBox({pos.x, pos.y}, {pos.x+15, pos.y+1}, RGB);
        writeBox({pos.x, pos.y}, {pos.x+1, pos.y+7}, RGB);
        writeBox({pos.x+6, pos.y}, {pos.x+7, pos.y+6}, RGB);
        writeBox({pos.x+14, pos.y}, {pos.x+15, pos.y+7}, RGB);
        break;
    case('f'):
        writeBox({pos.x, pos.y}, {pos.x+15, pos.y+1}, RGB);
        writeBox({pos.x, pos.y}, {pos.x+1, pos.y+7}, RGB);
        writeBox({pos.x+6, pos.y}, {pos.x+7, pos.y+6}, RGB);
        break;
    case('g'):
        writeBox({pos.x+1, pos.y}, {pos.x+14, pos.y+1}, RGB);
        writeBox({pos.x, pos.y+1}, {pos.x+1, pos.y+6}, RGB);
        writeBox({pos.x+14, pos.y+1}, {pos.x+15, pos.y+6}, RGB);
        writeBox({pos.x+10, pos.y+6}, {pos.x+14, pos.y+7}, RGB);
        writeBox({pos.x+8, pos.y+5}, {pos.x+9, pos.y+7}, RGB);
        break;
    case('h'):
        writeBox({pos.x, pos.y}, {pos.x+15, pos.y+1}, RGB);
        writeBox({pos.x+6, pos.y+2}, {pos.x+7, pos.y+5}, RGB);
        writeBox({pos.x, pos.y+6}, {pos.x+15, pos.y+7}, RGB);
        break;
    case('i'):
        writeBox({pos.x, pos.y+3}, {pos.x+15, pos.y+4}, RGB);        
        writeBox({pos.x, pos.y}, {pos.x+1, pos.y+7}, RGB);
        writeBox({pos.x+14, pos.y}, {pos.x+15, pos.y+7}, RGB);
        break;
    case('j'):
        writeBox({pos.x, pos.y+4}, {pos.x+14, pos.y+5}, RGB);        
        writeBox({pos.x, pos.y+1}, {pos.x+1, pos.y+7}, RGB);
        writeBox({pos.x+14, pos.y+1}, {pos.x+15, pos.y+4}, RGB);
        writeBox({pos.x+12, pos.y}, {pos.x+14, pos.y+1}, RGB);
        break;
    case('k'):
        writeBox({pos.x, pos.y}, {pos.x+15, pos.y+1}, RGB);        
        writeBox({pos.x+6, pos.y+2}, {pos.x+7, pos.y+4}, RGB);
        writeBox({pos.x+5, pos.y+4}, {pos.x+5, pos.y+5}, RGB);
        writeBox({pos.x+4, pos.y+5}, {pos.x+4, pos.y+6}, RGB);
        writeBox({pos.x, pos.y+6}, {pos.x+3, pos.y+7}, RGB);
        writeBox({pos.x+8, pos.y+4}, {pos.x+8, pos.y+5}, RGB);
        writeBox({pos.x+8, pos.y+5}, {pos.x+9, pos.y+6}, RGB);
        writeBox({pos.x+10, pos.y+6}, {pos.x+15, pos.y+7}, RGB);
        break;
    case('l'):
        writeBox({pos.x, pos.y}, {pos.x+15, pos.y+1}, RGB);
        writeBox({pos.x+14, pos.y+2}, {pos.x+15, pos.y+7}, RGB);
        break;
    case('m'):
        writeBox({pos.x+1, pos.y}, {pos.x+15, pos.y+1}, RGB);
        writeBox({pos.x+1, pos.y+6}, {pos.x+15, pos.y+7}, RGB);
        writeBox({pos.x+1, pos.y+3}, {pos.x+15, pos.y+4}, RGB);
        writeBox({pos.x, pos.y+1}, {pos.x+1, pos.y+6}, RGB);
        break;
    case('n'):
        writeBox({pos.x, pos.y}, {pos.x+15, pos.y+1}, RGB);
        writeBox({pos.x, pos.y+6}, {pos.x+15, pos.y+7}, RGB);
        writeBox({pos.x+1, pos.y+2}, {pos.x+4, pos.y+2}, RGB);
        writeBox({pos.x+4, pos.y+3}, {pos.x+7, pos.y+3}, RGB);
        writeBox({pos.x+7, pos.y+4}, {pos.x+10, pos.y+4}, RGB);
        writeBox({pos.x+10, pos.y+5}, {pos.x+13, pos.y+5}, RGB);
        break;
    case('o'):
        writeBox({pos.x+1, pos.y}, {pos.x+14, pos.y+7}, RGB);
        writeBox({pos.x, pos.y+1}, {pos.x+15, pos.y+6}, RGB);
        break;
    case('p'):
        writeBox({pos.x, pos.y}, {pos.x+15, pos.y+1}, RGB);
        writeBox({pos.x, pos.y+1}, {pos.x+7, pos.y+6}, RGB);
        writeBox({pos.x+1, pos.y+1}, {pos.x+6, pos.y+7}, RGB);
        break;
    case('q'):
        writeBox({pos.x+1, pos.y}, {pos.x+14, pos.y+6}, RGB);
        writeBox({pos.x, pos.y+1}, {pos.x+15, pos.y+5}, RGB);
        writeBox({pos.x+14, pos.y+7}, {pos.x+15, pos.y+7}, RGB);
        writeBox({pos.x+11, pos.y+4}, {pos.x+12, pos.y+4}, RGB);
        writeBox({pos.x+10, pos.y+3}, {pos.x+11, pos.y+3}, RGB);
        break;
    case('r'):
        writeBox({pos.x+8, pos.y}, {pos.x+15, pos.y+1}, RGB);        
        writeBox({pos.x, pos.y}, {pos.x+7, pos.y+6}, RGB);
        writeBox({pos.x+1, pos.y+1}, {pos.x+6, pos.y+7}, RGB);
        writeBox({pos.x+8, pos.y+4}, {pos.x+8, pos.y+5}, RGB);
        writeBox({pos.x+8, pos.y+5}, {pos.x+9, pos.y+6}, RGB);
        writeBox({pos.x+10, pos.y+6}, {pos.x+15, pos.y+7}, RGB);
        break;
    case('s'):
        writeBox({pos.x, pos.y+1}, {pos.x+1, pos.y+7}, RGB);
        writeBox({pos.x+14, pos.y}, {pos.x+15, pos.y+6}, RGB);
        writeBox({pos.x+6, pos.y+1}, {pos.x+7, pos.y+6}, RGB);
        writeBox({pos.x+1, pos.y}, {pos.x+5, pos.y+1}, RGB);
        writeBox({pos.x+7, pos.y+6}, {pos.x+14, pos.y+7}, RGB);
        break;
    case('t'):
        writeBox({pos.x, pos.y}, {pos.x+1, pos.y+7}, RGB);
        writeBox({pos.x, pos.y+3}, {pos.x+15, pos.y+4}, RGB);
        break;
    case('u'):
        writeBox({pos.x, pos.y}, {pos.x+14, pos.y+1}, RGB);
        writeBox({pos.x, pos.y+6}, {pos.x+14, pos.y+7}, RGB);
        writeBox({pos.x+14, pos.y+1}, {pos.x+15, pos.y+6}, RGB);
        break;
    case('v'):
        writeBox({pos.x, pos.y}, {pos.x+6, pos.y+1}, RGB);
        writeBox({pos.x+6, pos.y+1}, {pos.x+10, pos.y+2}, RGB);
        writeBox({pos.x+10, pos.y+2}, {pos.x+12, pos.y+3}, RGB);
        writeBox({pos.x+13, pos.y+3}, {pos.x+15, pos.y+4}, RGB);
        writeBox({pos.x+10, pos.y+4}, {pos.x+12, pos.y+5}, RGB);
        writeBox({pos.x+6, pos.y+5}, {pos.x+10, pos.y+6}, RGB);
        writeBox({pos.x, pos.y+6}, {pos.x+6, pos.y+7}, RGB);
        break;
    case('w'):
        writeBox({pos.x, pos.y}, {pos.x+14, pos.y+1}, RGB);
        writeBox({pos.x, pos.y+6}, {pos.x+14, pos.y+7}, RGB);
        writeBox({pos.x, pos.y+3}, {pos.x+14, pos.y+4}, RGB);
        writeBox({pos.x+14, pos.y+1}, {pos.x+15, pos.y+6}, RGB);
        break;
    case('x'):
        {
        int j=0, k=1, l=0, m=1;
        for(int i=0; i<16; i++)
        {
            if(i>4 && i<11){j++;k++;}
            if(i>5 && i<12){l++;m++;}
            updateBit({pos.x+i, pos.y+j}, RGB);
            updateBit({pos.x+15-i, pos.y+l}, RGB);
            updateBit({pos.x+i, pos.y+k}, RGB);
            updateBit({pos.x+15-i, pos.y+m}, RGB);
        }
        break;
        }
    case('y'):
        writeBox({pos.x, pos.y}, {pos.x+4, pos.y+1}, RGB);
        writeBox({pos.x, pos.y+6}, {pos.x+4, pos.y+7}, RGB);
        writeBox({pos.x+7, pos.y+3}, {pos.x+15, pos.y+4}, RGB);
        writeBox({pos.x+5, pos.y+1}, {pos.x+5, pos.y+2}, RGB);
        writeBox({pos.x+6, pos.y+2}, {pos.x+6, pos.y+3}, RGB);
        writeBox({pos.x+5, pos.y+5}, {pos.x+5, pos.y+6}, RGB);
        writeBox({pos.x+6, pos.y+4}, {pos.x+6, pos.y+5}, RGB);
        break;
    case('z'):
        {
        int j=0, k=1;
        writeBox({pos.x, pos.y}, {pos.x+1, pos.y+7}, RGB);
        writeBox({pos.x+14, pos.y}, {pos.x+15, pos.y+7}, RGB);
        for(int i=0; i<14; i++)
        {
            if(i>5 && i<12){j++;k++;}
            updateBit({pos.x+15-i, pos.y+j}, RGB);
            updateBit({pos.x+15-i, pos.y+k}, RGB);

        }
        break;
        }
    case(' '):
        break;
    default:    // print null symbol
        writeBox({pos.x+1, pos.y}, {pos.x+14, pos.y+7}, RGB);
        writeBox({pos.x, pos.y+1}, {pos.x+15, pos.y+6}, RGB);
        writeBox({pos.x+1, pos.y+5}, {pos.x+4, pos.y+5}, RGB);
        writeBox({pos.x+4, pos.y+4}, {pos.x+7, pos.y+4}, RGB);
        writeBox({pos.x+7, pos.y+3}, {pos.x+10, pos.y+3}, RGB);
        writeBox({pos.x+10, pos.y+2}, {pos.x+13, pos.y+2}, RGB);
        break;
    }
    return 0;
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