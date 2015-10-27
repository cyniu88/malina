#include <wiringPi.h>
int main (void)
{
  wiringPiSetup () ;
  pinMode (3, OUTPUT) ;
   
  
    digitalWrite (0, HIGH) ; delay (500) ;
    digitalWrite (0,  LOW) ; delay (500) ;
  
  return 0 ;
}
