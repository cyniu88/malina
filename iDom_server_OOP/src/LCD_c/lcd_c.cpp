#include "lcd_c.h"

LCD_c::LCD_c(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows ) : main_lcd (lcd_Addr,lcd_cols,lcd_rows),
    m_lcd_state(0),m_row1("  Dzien dobry!"),m_row2("  Milego Dnia")
{
    main_lcd.init();
    main_lcd.noCursor();
    m_className.append(typeid(this).name());
    addToMap(m_className,this);
}

LCD_c::~LCD_c()
{
    removeFromMap(m_className);
}

void LCD_c::checkState()
{
    if (m_print_song_state > 1 )
    {
        --m_print_song_state;
    }
    if (m_print_song_state == 0 ){
        if (m_lcd_state == -1)
        {
            return;
        }
        if (m_lcd_state == 0 )
        {
            if (m_play_Y_N == false)
            {
                noBacklight(); // jesli nic nie jest wysetlane print_song_state == 0 i nic nie jest grane
                // play_Y_N == fale a lcd_state =0 to gas ekran bo szkoda pradu
            }

            if (m_rePrint == 0 )
            {
                printRadioName(true,0,0,m_radioName);
                m_lcd_state = 14;
            }
            else
            {
                --m_rePrint;
            }
            return;
        }
        if (m_lcd_state == 100) // blokuje caly wysetlacz nie wysetla piosenek
        {
            return;
        }
        if (m_lcd_state == 1 )
        {
            --m_lcd_state;
            song_printstr(); // wysetli nazwe piosenki po zliczeniu countera
        }
        else if (m_lcd_state > 1)
        {
            --m_lcd_state;
        }
    }
}

std::string LCD_c::getData()
{
    std::stringstream ss;
    ss << "play_Y_N \t" << m_play_Y_N << std::endl;
    ss << "lcd_state \t" << m_lcd_state << std::endl;
    ss << " print_song_state\t" << m_print_song_state << std::endl;
    ss << "rePrint\t" << m_rePrint << std::endl;
    ss << "row1\t" << m_row1 << std::endl;
    ss << "row2\t" << m_row2 << std::endl;
    ss << "radioName\t" << m_radioName << std::endl;
    return ss.str();
}

std::string LCD_c::dump() const
{
    std::stringstream ret;

    ret << "play_Y_N \t" << m_play_Y_N << std::endl;
    ret << "lcd_state \t" << m_lcd_state << std::endl;
    ret << " print_song_state\t" << m_print_song_state << std::endl;
    ret << "rePrint\t" << m_rePrint << std::endl;
    ret << "row1\t" << m_row1 << std::endl;
    ret << "row2\t" << m_row2 << std::endl;
    ret << "radioName\t" << m_radioName << std::endl;
   // ret << "status\t" << LCD_c::m_status << std::endl;
    return ret.str();
}

void LCD_c::set_lcd_STATE (int i)
{
    m_lcd_state = i;
}

void LCD_c::set_print_song_state (int i)
{
    m_print_song_state = i;
}

void LCD_c::printSongName (const std::string& songName){

    std::size_t pos = songName.find(" - ");

    m_row2 = songName.substr(pos+3); //tytul
    m_row1 = songName.substr(0,pos); //autor
    if (m_row1 != m_row2){
        if(m_row1.size() < 14)
        {
            m_row1 += " -"; // doda pauze na koncu nazwy autora
        }
        else
        {
            if (m_row2.size() < 16){
                m_row2.insert(0, "- "); // doda pauze na poczatku utworu ale tyko jak bedzie rizu auto i tytul
            }
            else{
                m_row1 = m_row1.substr(0,18);
                m_row2 = m_row2.substr(0,18);
            }
        }
    }
    else
    {
        m_row2 = "  ";
        return;
    }

    if (m_lcd_state == 0 && m_play_Y_N == true ){
        song_printstr();
    }
}

void LCD_c::song_printstr(){
    if (m_lcd_state != 0 || m_play_Y_N == false ){
        return;
    }
    if (m_print_song_state != 0 )
    {
        return;
    }

    main_lcd.clear();
    main_lcd.backlight();

    main_lcd.setCursor(0, 0);
    main_lcd.printstr(m_row1.c_str());
    main_lcd.setCursor(0, 1);
    main_lcd.printstr(m_row2.c_str());
}

void LCD_c::printRadioName(bool clear, int col, int row, const std::string &st){
    m_radioName = st;
    std::regex reg1("\\[(.*?)\\]");
    std::smatch res1;
    if (regex_search(m_radioName,res1,reg1)){
        m_radioName = res1[1];
    }
    m_rePrint = 30;
    if (m_print_song_state != 0 || m_play_Y_N == false )
    {
        return;
    }
    if ( clear == true)
    {
        main_lcd.clear();
    }
    main_lcd.backlight();
    main_lcd.setCursor(col, row);
    main_lcd.printstr(m_radioName.c_str());
}

void LCD_c::printString(bool clear,int col,int row , const std::string& str)
{
    if ( clear==true)
    {
        main_lcd.clear();
    }
    main_lcd.backlight();
    main_lcd.setCursor(col, row);
    main_lcd.printstr(str.c_str());
}

void LCD_c::noBacklight()
{
    main_lcd.clear();
    main_lcd.noBacklight();
}
void LCD_c::clear()
{
    main_lcd.clear();
}


void LCD_c::printVolume (int vol)
{
    if (m_print_song_state != 0 || m_lcd_state == -1 )
    {
        return;
    }
    std::string tmp = "   vol ";
    tmp += std::to_string(vol);
    tmp += " %";
    main_lcd.backlight();
    main_lcd.clear();
    main_lcd.printstr(tmp.c_str());
    m_lcd_state = 10;
}
