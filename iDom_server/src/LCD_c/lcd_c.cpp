#include "lcd_c.h"

LCD_c::LCD_c(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows ) : main_lcd (lcd_Addr,lcd_cols,lcd_rows),
    lcd_state(0),row1("  Dzien dobry!"),row2("  Milego Dnia")
{
    main_lcd.init();
    main_lcd.noCursor();
}

void LCD_c::checkState()
{
    if (lcd_state == 0 || lcd_state == -1)  // -1   blokuje  caly wysetlacz  nie wysetla piosenek
    {
        return;
    }
    if (lcd_state == 100  )  //    blokuje  caly wysetlacz  nie wysetla piosenek
    {
        return;
    }
    if (lcd_state == 1 )
    {
        --lcd_state;
        song_printstr();  // wysetli  nazwe piosenki  po zliczeniu countera
    }
    else if (lcd_state > 1)
    {
        --lcd_state;
    }
}
void LCD_c::set_lcd_STATE (int i)
{
    lcd_state = i;
}

void LCD_c::printSongName (std::string songName){

    std::size_t pos = songName.find(" - ");

    row2 = songName.substr(pos+3);  //tytul
    row1 = songName.substr(0,pos);  //autor
    if (row1!=row2){
        if(row1.size()<14)
        {
            row1+=" -";   // doda pauze na koncu nazwy autora
        }
        else
        {
            if (row2.size()<14){
                row2.insert(0,"- "); // doda pauze  na poczatku utworu  ale  tyko jak bedzie rizu auto i tytul
            }
        }
    }
    else
    {
        row2="  ";
    }
    if (lcd_state == 0 ){
        song_printstr();
    }
}

void LCD_c::song_printstr(){
    main_lcd.clear();
    main_lcd.backlight();

    main_lcd.setCursor(0, 0);
    main_lcd.printstr(row1.c_str());
    main_lcd.setCursor(0, 1);
    main_lcd.printstr(row2.c_str());
}
void LCD_c::printString(bool clear,int col,int row , std::string str){
    if ( clear==true){main_lcd.clear();}
    main_lcd.backlight();
    main_lcd.setCursor(col, row);
    main_lcd.printstr(str.c_str());

}

void LCD_c::noBacklight(){
    main_lcd.clear();
    main_lcd.noBacklight();
}
void LCD_c::clear()
{
    main_lcd.clear();
}
void LCD_c::printVolume (int vol)
{   std::string tmp ="   vol  "  ;
    tmp+=intToStr(vol);
    tmp+=" %";
    main_lcd.backlight();
    main_lcd.clear();
    main_lcd.printstr(  tmp.c_str() );
    lcd_state=10;

}
std::string LCD_c::intToStr(int n)
{
    std::string tmp;
    if(n < 0) {
        tmp = "-";
        n = -n;
    }
    if(n > 9)
        tmp += intToStr(n / 10);
    tmp += n % 10 + 48;
    return tmp;
}
