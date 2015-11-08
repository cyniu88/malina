#include <wiringPi.h>

#include <lcd.h>

#include <pcf8574.h>

using namespace std;
static int screen;

//define new pins
#define AF_BASE 100

#define AF_RS   (AF_BASE + 0)
#define AF_RW   (AF_BASE + 1)
#define AF_E    (AF_BASE + 2)

#define AF_BL   (AF_BASE + 3)

#define AF_D1   (AF_BASE + 4)
#define AF_D2   (AF_BASE + 5)
#define AF_D3   (AF_BASE + 6)
#define AF_D4   (AF_BASE + 7)

//initalize function for WiringPi, LCD and PCF8574 module.
void Initalize()
{


}

int main()
{

//WiringPi initalize
wiringPiSetupSys ();

//LCD initalize
screen = lcdInit(2, 16, 4, AF_RS, AF_E, AF_D1, AF_D2, AF_D3, AF_D4, 0, 0, 0, 0);

//PCF8574 initalize
pcf8574Setup(AF_BASE, 0x27);

//Turning backlight ON
pinMode (AF_BL, OUTPUT);
digitalWrite (AF_BL, 1);

//Set LCD into write mode.
pinMode (AF_RW, OUTPUT);
digitalWrite (AF_RW, 0);



//Print function
lcdClear(screen);
lcdPosition (screen,0,0);
lcdPuts(screen, "Hello World!");
return 0;
}
