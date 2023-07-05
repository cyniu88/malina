#include "lcd_c.h"
#include "../iDom_server_OOP.h"

LCD_c::LCD_c(uint8_t lcd_Addr, uint8_t lcd_cols, uint8_t lcd_rows)
{
    // GPIO chip i2c address
    uint8_t i2c = 0x27;
    // Control line PINs
    uint8_t en = 2;
    uint8_t rw = 1;
    uint8_t rs = 0;

    // Data line PINs
    uint8_t d4 = 4;
    uint8_t d5 = 5;
    uint8_t d6 = 6;
    uint8_t d7 = 7;

    // Backlight PIN
    uint8_t bl = 3;

    // LCD display size (1x16, 2x16, 4x20)
    uint8_t rows = 2;
    uint8_t cols = 16;

    int adapter_nr = 1;
    char filename[20];

    snprintf(filename, 19, "/dev/i2c-%d", adapter_nr);
    log_file_mutex.mutex_lock();
    log_file_cout << DEBUG << "LCD I2C " << filename << " Rows " << rows << " cols " << cols << std::endl;
    log_file_mutex.mutex_unlock();
    //printf("Using i2c device: %s, Rows: %u, Cols: %u\n", filename, rows, cols);
    // LCD display size (1x16, 2x16, 4x20)
    main_lcd = std::make_unique<LiquidCrystal_I2C>(filename, i2c, en, rw, rs, d4, d5, d6, d7, bl, POSITIVE);
    main_lcd->begin(cols, rows);
    main_lcd->noCursor();
    // main_lcd->autoscroll();
    main_lcd->createChar(0, arrowDown);
    main_lcd->createChar(1, arrowUp);
    m_className.append(typeid(this).name());
    addToMap(m_className, this);
}

LCD_c::~LCD_c()
{
    removeFromMap(m_className);
}

std::string LCD_c::getData() const
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

void LCD_c::printString(const std::string &row1, const std::string &row2, bool clear)
{
    if (clear == true)
    {
        main_lcd->clear();
        m_printed.clear();
    }

    main_lcd->backlight();
    main_lcd->setCursor(0, 0);
    main_lcd->print(row1.c_str());
    main_lcd->setCursor(0, 1);
    main_lcd->print(row2.c_str());

    m_printed.append(row1);
    m_printed.append(" ");
    m_printed.append(row2);
}

void LCD_c::noBacklight()
{
    main_lcd->clear();
    main_lcd->noBacklight();
    m_printed.clear();
}
void LCD_c::clear()
{
    main_lcd->clear();
    m_printed.clear();
}

void LCD_c::scrollLeft()
{
    main_lcd->scrollDisplayLeft();
}

void LCD_c::scrollRight()
{
    main_lcd->scrollDisplayRight();
}
