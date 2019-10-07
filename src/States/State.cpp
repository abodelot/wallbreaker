#include "State.hpp"


State::State():
    m_previous(nullptr)
{
}


void State::setPrevious(State* state)
{
    m_previous = state;
}


State* State::getPrevious() const
{
    return m_previous;
}
