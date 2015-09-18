     #include <wiringPi.h>
    #include <pcf8574.h>
    #include <lcd.h>

    #define LCDOFS(x) (lcdofs + x)
    const int lcdofs = 0x64;

    int main(void)
    {
        int fd;

        pcf8574Setup(lcdofs, 0x27);
        wiringPiSetup();

        pinMode(LCDOFS(1), OUTPUT);
        pinMode(LCDOFS(3), OUTPUT);
        digitalWrite(LCDOFS(1), 0);
        digitalWrite(LCDOFS(3), 1);

        fd = lcdInit(4, 20, 4, LCDOFS(0), LCDOFS(2),
            LCDOFS(4), LCDOFS(5), LCDOFS(6), LCDOFS(7), 0, 0, 0, 0);
        lcdHome(fd);
        lcdClear(fd);
        lcdPrintf(fd, „Stay Tuned!“);
    } 
