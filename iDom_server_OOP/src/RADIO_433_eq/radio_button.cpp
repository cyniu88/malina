#include "radio_433_eq.h"
/*
 * Najpierw możliwe do uzyskania funkcje załączania

    Załączenie chwilowe (momentary) - przekaźnik jest załączony tak długo jak długo naciskany jest klawisz pilota - nadawany kod
    Przełącznik (toggle) - przekaźnik jest załączany /wyłączany przy kolejnych naciśnięciach tego samego klawisza - nadajemy ten sam kod
    Włącz/wyłącz (latched) - przekaźnik jest załączany jednym kodem a wyłączany innym
    Włącznik czasowy 5 sek - po naciśnięciu pilota następuje załączenie przekaźnika na czas 5 sek
    Włącznik czasowy 10 sek - załączenie na czas 10 sek
    Włącznik czasowy 15 sek - załączenie na czas 15 sek

Programowanie
Niby opisane jest to na stronach dostawców - ale nie do końca - więc pełen opis programowania. Zalecane rozpoczęcie od punktu 8 - KASOWANIE

    Załączenie chwilowe (momentary) - naciskamy 1x przycisk programowania (potwierdzenie naciśnięcia krótkim błyskiem LED). LED po chwili włącza się na stałe na 8-10 sek. W tym czasie układ gotowy jest na przyjęcie nowego kodu. Wysłany kod zapamiętany jest w pamięci a potwierdzeniem tego jest podwójne "mrugnięcie" LEDa. LED gaśnie . Koniec procedury.
    Przełącznik (toggle) - naciskamy 2 x przycisk programowania. Dalej jak w pkt. 1
    Włącz/wyłącz (latched) - naciskamy 3 x przycisk programowania (potwierdzenie naciśnięcia krótkim błyskiem LED). LED po chwili włącza się na stałe na 8-10 sek. W tym czasie układ gotowy jest na przyjęcie nowego kodu. Wysłany kod zapamiętany jest w pamięci a potwierdzeniem tego jest podwójne "mrugnięcie" LEDa. LED pozostaje włączony Układ czeka na odbiór drugiego kodu. Po jego wysłaniu jest znowu podwójne "mrugnięcie" LEDa. LED gaśnie. Koniec procedury.
    Włącznik czasowy 5 sek - naciskamy 4 x przycisk programowania. Dalej jak w pkt. 1
    Włącznik czasowy 10 sek - naciskamy 5 x przycisk programowania. Dalej jak w pkt. 1
    Włącznik czasowy 15 sek - naciskamy 6 x przycisk programowania. Dalej jak w pkt. 1
    Włącznik czasowy 15 sek - naciskamy 7 x przycisk programowania. Dalej jak w pkt. 1
     KASOWANIE - usuwanie z pamięci wszystkich kodów. Nacisnąć 8 x przycisk. Każde naciśniecie sygnalizowane jest błyśnięciem. Po 8 naciśnięciu następują trzy mignięcia i pamięć kodów jest wyzerowana. Można też skasować pamięć naciskając przycisk raz przez ok 8 sek . Po puszczeniu przycisku LED zapali się na ok 3-4 sek i zgaśnie. Efekt działania ten sam.

Wszystkie te funkcje działają równolegle bo układ może zapamiętać do 50 kodów! Dodatkowo w trybie włącznika czasowego można skrócić czas załączenia poprzez wysłanie kodu WYŁĄCZ z wcześniej ustawionej funkcji latched.
 */
RADIO_BUTTON::RADIO_BUTTON(thread_data *my_data, const RADIO_EQ_CONFIG& cfg, RADIO_EQ_TYPE type)
{
    //puts("RADIO_BUTTON::RADIO_BUTTON()");
    RADIO_EQ::m_my_data = my_data;
    RADIO_EQ::m_type = type;
    RADIO_EQ::m_config = cfg;
}

RADIO_BUTTON::~RADIO_BUTTON()
{
    //puts("RADIO_BUTTON::~RADIO_BUTTON()");
}

STATE RADIO_BUTTON::getState()
{
    return m_state;
}

void RADIO_BUTTON::setState(STATE s)
{
    m_state = s;
}

std::string RADIO_BUTTON::getName()
{
    return RADIO_EQ::m_config.name;
}

std::string RADIO_BUTTON::getID()
{
    return RADIO_EQ::m_config.ID;
}
