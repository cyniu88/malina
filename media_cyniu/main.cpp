#include <stdio.h>
#include <wiringPi.h>

// LED Pin - wiringPi pin 0 is BCM_GPIO 17.

#define LED     7

int main (void)
{
  printf ("Raspberry Pi - Gertboard Blink\n") ;

  wiringPiSetup () ;

  pinMode (LED, OUTPUT) ;

  for (int i=0;i<50;++i)
  {
    digitalWrite (LED, 1) ;     // On
    delay (500) ;               // mS
    digitalWrite (LED, 0) ;     // Off
    delay (500) ;
  }
  return 0 ;
}
