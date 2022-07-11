#include "lcd_c.h"

LCD_c::LCD_c(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows ) : main_lcd (lcd_Addr,lcd_cols,lcd_rows)
{
    main_lcd.init();
    main_lcd.noCursor();
    main_lcd.autoscroll();
    main_lcd.createChar(0, arrowDown);
    main_lcd.createChar(1, arrowUp);
    m_className.append(typeid(this).name());
    addToMap(m_className,this);
}

LCD_c::~LCD_c()
{
    removeFromMap(m_className);
}


std::string LCD_c::getData()
{
    std::stringstream ss;
    ss << "m_printed:\t" << m_printed << std::endl;
    return ss.str();
}

std::string LCD_c::dump() const
{
    std::stringstream ret;
    ret << "m_printed:\t" << m_printed << std::endl;
    return ret.str();
}


void LCD_c::printString(const std::string &row1, const std::string &row2, bool clear )
{
    if ( clear == true)
    {
        main_lcd.clear();
        m_printed.clear();
    }
    main_lcd.backlight();
    main_lcd.setCursor(0, 0);
    main_lcd.printstr(row1.c_str());
    main_lcd.setCursor(0, 1);
    main_lcd.printstr(row2.c_str());

    m_printed.append(row1);
    m_printed.append(" ");
    m_printed.append(row2);
}

void LCD_c::noBacklight()
{
    main_lcd.clear();
    main_lcd.noBacklight();
    m_printed.clear();
}
void LCD_c::clear()
{
    main_lcd.clear();
    m_printed.clear();
}
