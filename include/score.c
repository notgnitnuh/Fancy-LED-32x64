#include <avr/delay.h>
#include <avr/io.h>

void displayMeter(uint64_t score);
void InitScore();


void InitScore()
{
    DDRK = 0xFF;
    PORTK = 0xFF;
}

void displayMeter(uint64_t score)
{
    if (score > 280)
        PORTK = ~0xFF;
    else if (score > 240)
        PORTK = ~0x7F;
    else if (score > 200)
        PORTK = ~0x3F;
    else if (score > 160)
        PORTK = ~0x1F;
    else if (score > 120)
        PORTK = ~0x0F;
    else if (score > 80)
        PORTK = ~0x07;
    else if (score > 40)
        PORTK = ~0x03;
    else
        PORTK = ~0x01;
}