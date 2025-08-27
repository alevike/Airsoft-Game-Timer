#include "buzzer.h"
void clearBuzzer(void)
{  
  digitalWrite(tonepin, LOW);
}

void bipp(int duration) //non-blocking active buzzer
{
   digitalWrite(tonepin, HIGH);
   timer.setTimeout(duration, clearBuzzer);
    
}

void Bbipp(int duration) //blocking active buzzer - used in menu
{
  digitalWrite(tonepin, HIGH);
  delay(duration);
  digitalWrite(tonepin, LOW);
}