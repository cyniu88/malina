#include "irda_logic.h"
#include "../functions/master_LCD.h"
irda_logic::irda_logic()
{
    who='!';
   // pinMode(LED7, OUTPUT); // LED  na wyjscie  GPIO
    digitalWrite(LED7,OFF);
}
//irda_logic::who='!';

void irda_logic::_add(char X)
{

    if (who=='!')
    {
        if (X!='m'&& X!='P')
        {
            char_queue._add(X);
        }

        else if (X=='P')
        {
            who = 'P';
            char_queue._add('v');  // przy wlaczeniu porjektora zatrzymujemy muzyke :)
        }
        else {
            who = 'm';
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////////////// obsluga projektora
    else if (who=='P')
    {
        //std::cout << "jestem w projektorze  " << std::endl;
        if ( X=='e')
        {
            who = '!';
         //   std::cout << "koniec sterowania  projektorem" << std::endl;
        }
        else if (X=='+')
        {
            system("echo -n + > /tmp/cmd");  // podglasniamy
        }
        else if (X=='-')
        {
            system("echo -n - > /tmp/cmd");  // wyciszamy
        }
        else if (X=='o')
        {
            system("echo -n p > /tmp/cmd");  //pauza play
        }
        else if (X=='v')
        {
            system("echo -n q > /tmp/cmd");  // zamykanie omxplayera
        }
        else if (X=='b')
        {
            system("echo -n $'\x1b\x5b\x43' > /tmp/cmd");  // do przodu
        }
        else if (X=='z')
        {
            system("echo -n $'\x1b\x5b\x43' > /tmp/cmd");  // do tylu

        }
        else if (X=='N')
        {
            system("echo -n o > /tmp/cmd");  // do przodu
        }
        else if (X=='G')
        {
            system("echo -n i > /tmp/cmd");  // do tylu

        }
    }
  /////////////////////////////////////////////////////////////////////////////////////////////  oblsuga menu
    else if (who=='m')
    {
        std::cout << "jestem w menu  az wcisne ok lub exit " << X <<std::endl;
        if (X=='o' || X=='e')
        {
            who = '!';
        }

        else if (X=='i')
        {

           // std::cout << " stan led to " << digitalRead(LED7)<<std::endl;
            if (digitalRead(LED7)== OFF)
            {
                digitalWrite(LED7,ON);

            }

            else if (digitalRead(LED7)==ON)
            {
                digitalWrite(LED7,OFF);

            }

        }

    }
}

char irda_logic::_get( )
{  char temp;


    if (irda_queue.size() > 0){
        temp = irda_queue.front();

        irda_queue.pop();
    }
    else{

        return 'a';
    }
    return temp;
}

int irda_logic::_size()
{
    return irda_queue.size();
}
