#include <stdio.h>
#include <wiringPi.h>
#include <wiringSerial.h>
 
int main(int argc, char **argv)
{
    //Zmienna pomocnicza
    char a;
 
    //Zmienna deskryptora UART
    int descriptor;
 
    //Komunikat poczatkowy
    printf("Program testowy UART WiringPi!\n");
 
    if (wiringPiSetup() == -1)
    {
        printf("Nie mozna wystartowac wiringPi!\n");
        return 1;
    }
 
    //Otwieramy port szeregowy
    descriptor=serialOpen("/dev/ttyAMA0",9600);
printf("uart otwarty!\n");
 
    //Petla programu
    while (a!='q')
    {
        //Wysylamy przez UART liczbe 85
        serialPutchar(descriptor,(unsigned char) 85);
 
        //Czyscimy bufor
        serialFlush(descriptor);
 
        //Pobieramy znak
        scanf("%c",&a);
    }
 
    //Zamykamy port szeregowy
    serialClose(descriptor);
 
    //Koniec programu
    return 0;
}
