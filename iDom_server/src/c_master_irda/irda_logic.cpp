#include "irda_logic.h"
#include "../functions/master_LCD.h"
irda_logic::irda_logic()
{
    who='!';
    pinMode(LED7, OUTPUT); // LED  na wyjscie  GPIO
    digitalWrite(LED7,OFF);
}
//irda_logic::who='!';

void irda_logic::_add(char X)
{

    if (who=='!')
    {
        if (X!='m')
        {
            char_queue._add(X);
        }
        else {
            who = 'm';
        }
    }
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
