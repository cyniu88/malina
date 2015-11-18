#include <wiringPi.h>
#include <cstdio>
#include <ctime>
#include <cstdlib>
#include "LiquidCrystal_I2C.h"


#define GPIO_ADDR 0x27

using namespace std;

void lcd_example()
{
	LiquidCrystal_I2C lcd(GPIO_ADDR, 16, 2);
	time_t t;
	struct tm* ltime;
	char buf[32];
	memset(buf, 0, sizeof(buf));
	lcd.init();
	lcd.backlight();
	for (;;) {
		time(&t);
		ltime = localtime(&t);
		snprintf(buf, 32, "%2d:%2d:%2d",
			ltime->tm_hour,
			ltime->tm_min,
			ltime->tm_sec);
		lcd.print(buf);
		printf("%s\n", buf);
		delay(1000);
	}
}

int main()
{
	if (wiringPiSetup() == -1) {
		fprintf(stderr, "could not set up wiringPi\n");
	}
	lcd_example();
	
}

