#include <time.h>
 
#include <iostream>
 struct alarm {
        int godzina;
        int minuta;
    };
class budzik
{   public:
//	budzik();
//	~budzik();
    int set_alarm ();
    bool watchdog();
    
	private:
	 void get_time();
    time_t czas;
    struct tm * ptr;
    struct alarm _alarm;
   
 
};

   int budzik::set_alarm()
   {
    std::cin >>   _alarm.godzina ;
    std::cin >>  _alarm.minuta;
        return 0;   
   }
    
 bool budzik::watchdog ()
{
    get_time();
    return true;
}
void budzik::get_time ()
{
    time( & czas );
    ptr = localtime( & czas );
    std::cout << "godzina: " << ptr->tm_hour << std::endl;
        std::cout << "godzina moja : " << _alarm.godzina << std::endl;
  std::cout << "minuta : " << ptr->tm_min << std::endl;
  std::cout << "minuta moja  : " << _alarm.minuta << std::endl;
   std::cout << "dzie " << ptr->tm_wday << std::endl;
   
   if ( ptr->tm_hour ==  _alarm.godzina &&  ptr->tm_min ==  _alarm.minuta )
   {
       std::cout <<  " !!!!!!!!!!!!!!!!!!!!!  alarm wstawaj " <<std::endl;
   }
   
   
}

int main ()
{
budzik _budzik;
_budzik.set_alarm();
_budzik.watchdog();
for ( int i =0 ; i < 100; ++i )
{
std::cout << " iteracja " << i << std::endl;
_budzik.watchdog();
sleep (15);
}
  std::cout << "koniec  " << std::endl;
return 0;
}
