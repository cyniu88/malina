#include <iostream>
#include <fstream>
#include <time.h>
 struct File
{
 int h;
 int m;
 bool activ;
};
class cyniu_alarm
{
private:
File file [7];
time_t act_time;
struct tm * act_date;
public:

void set_alarm(int _day, int _h, int _m, bool _activ);
void write_to_file();
void read_from_file();
bool check_alarm();
void check_timeout ();
void set_timeout (int min);

};
 bool cyniu_alarm::check_alarm()
 {
	time( & act_time );
    act_date = localtime( & act_time );
	 
    std::cout << "dzien tygodnia " << act_date->tm_wday << " godzina " << act_date->tm_hour  <<" minuta " << act_date->tm_min  <<std::endl;
	if (file[act_date->tm_wday].h == act_date->tm_hour && file[act_date->tm_wday].m == act_date->tm_min && file[act_date->tm_wday].activ == true)
	{
	return true;
	}
	else
	{
	return false;
	}
 }
 void cyniu_alarm::set_alarm(int _day, int _h, int _m, bool _activ)
 {
 
std::cout << " podaj godzinie z dnia: " << i<< std::endl;
  file[_day].h = _h;
  file[_day].m = _m;
  file[_day].activ _activ;
 }
  
 
 
void cyniu_alarm::write_to_file()
{
  std::ofstream ofs("plik.database", std::ios::binary); // otwieramy plik do zapisu binarnego
 
 ofs.write((char*)(file), sizeof(file)); // zapisujemy dane do pliku
 
std::cout << " sizeof daje: " << sizeof(File) <<std::endl;
 ofs.close();
}
 void cyniu_alarm::read_from_file()
 {
 std::ifstream ifs("plik.database", std::ios::binary);  
 ifs.read((char*)file, sizeof(file));
 std::cout << " fileuje " << std::endl;
 for (int i=0; i<7; ++i)
{
std::cout << " file  z dnia: " << i<< std::endl;
 std::cout << file[i].h << " godz " << file[i].m << " minut" <<std::endl;
 }
 
 ifs.close();
 }
int main()
{
  cyniu_alarm alarm1;
  int wybor;
  std::cin >>wybor;
  if (wybor == 1 )
  {
  alarm1.read_from_file();
 }
 else if (wybor ==0 )
 {
 alarm1.set_alarm();
 }
while ( !alarm1.check_alarm())
{
std::cout << " nie ma alarmu " << std::endl;
sleep (60);
}
std::cout << " ALARM!!!!" << std::endl;
 return 0;
}
