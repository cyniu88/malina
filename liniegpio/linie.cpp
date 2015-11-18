#include <iostream>
#include <wiringPi.h>
#include "../arduPi/arduPi.h"
char k ='0';
int main (void)
{
  
if (wiringPiSetup()==-1)
    {
	std::cout << "nie dziala\n" << std::endl;
    return 1;
    }   

  pinMode (7, OUTPUT) ;
   
  
    digitalWrite (7, HIGH) ;// delay (500) ;
 std::cin >> k;
    digitalWrite (7,  LOW) ; //delay (500) ;
  std::cin >> k;

 pinMode (7, INPUT);
  return 0 ;
}
