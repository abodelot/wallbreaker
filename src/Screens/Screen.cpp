#include "Screen.hpp"


Screen::Screen():
    m_previous(NULL)
{
}


void Screen::setPrevious(Screen* screen)
{
    m_previous = screen;
}


Screen* Screen::getPrevious() const
{
    return m_previous;
}
