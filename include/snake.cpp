#include <avr/interrupt.h>
#include <LED.c>
#include <stdlib.h>

enum direction{up=0, down=1, left=2, right=3};

struct snake_bit
{
    Vector2 pos;
    //direction dir;
    bool alive;
};



void Init();
int snake_main();
bool update_snake(direction& dir, snake_bit* snake, int& length, Vector2& apple);
void kill_snake(snake_bit* snake, int& length);
Vector2 spawn_apple(snake_bit* snake, int length);
const uint8_t snake_color = blue;

const int max_length = 200;

int snake_main()
{
    bool alive = true;
    Init();
    InitLED();
    srand(10);
    snake_bit snake[max_length] = {0};
    int snake_length = 4;
    direction snake_dir = up;
    Vector2 apple = {rand() % 30, rand() % 60};

    //make boarder
    writeBox({1,1},{30,62}, cyan);
    
    apple = spawn_apple(snake, snake_length);
    while(true)
    {
        //initialize first bit of snake
        alive = true;
        snake_length = 4;
        snake_dir = right;
        snake[0] ={{5,3},true};// {{rand()%30+1, rand()%60+1}, true};
        snake[1] ={{4,3},true};
        snake[2] ={{3,3},true};
        snake[3] ={{2,3},true};
        snake[4] ={{1,3},true};
        
        //run till dead
        while(alive)
        {
            alive = update_snake(snake_dir, snake, snake_length, apple);    
            updateBit(snake[snake_length].pos, off);
            _delay_ms(10);
        }
        kill_snake(snake, snake_length);
    }
}

//update snake location
bool update_snake(direction& dir, snake_bit* snake, int& length, Vector2& apple)
{
    //update body
    for(int i=length; i>0; i--)
    {

        if(snake[i].alive == true)
        {
            if(snake[i].pos == snake[0].pos)
            {
                return false;
            }
            // updateBit(snake_ptr->pos, off);
            snake[i].pos = snake[i-1].pos;
            updateBit(snake[i].pos, snake_color);

        }
    }

    //move head
    if(apple.y > snake[0].pos.y && dir != down)
    {
        snake[0].pos.y++;
        updateBit(snake[0].pos,snake_color);
        dir = up;
    }
    else if (apple.x < snake[0].pos.x && dir != right)
    {
        snake[0].pos.x--;
        updateBit(snake[0].pos,snake_color);
        dir = left;
    }
    else if (apple.y < snake[0].pos.y && dir != up)
    {
        snake[0].pos.y--;
        updateBit(snake[0].pos,snake_color);
        dir = down;
    }
    else //if (snake[0].pos.x < 29)
    {
        snake[0].pos.x++;
        updateBit(snake[0].pos,snake_color);
        dir = right;
        if(snake[0].pos.x > 28)
            dir = up;
    }
    // else 
    // {
    //     snake[0].pos.y++;
    //     updateBit(snake[0].pos,snake_color);
    //     dir = up;
    // }
    

    //if snakehead == apple, spawn apple
    if(snake[0].pos == apple)
    {
        length++;
        snake[length].alive = true;
        apple = spawn_apple(snake, length);
    }

    
    return true;
}

Vector2 spawn_apple(snake_bit* snake, int length)
{
    bool apple_placed = false;
    Vector2 apple = {rand() % 26+3, rand() % 58 + 4};
    while(!apple_placed)
    {
        apple = {rand() % 26+3, rand() % 58 + 4};
        apple_placed = true;
        for(int i=0; i<length; i++)
        {
            if(apple == snake[i].pos)
                apple_placed = false;
        }
    }
    updateBit(apple, red);
    return apple;
}


void kill_snake(snake_bit* snake, int& length)
{
    for(int i=0; i<64; i++)
    {
        snake[i].alive = false;
        updateBit(snake[i].pos, off);
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