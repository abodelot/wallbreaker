#include <stdexcept>
#include "Layout.hpp"
#include "Theme.hpp"
#include "Button.hpp"
#include "Label.hpp"

using namespace gui;


Layout::Layout(Type type):
    m_first(nullptr),
    m_last(nullptr),
    m_type(type),
    m_hover(nullptr),
    m_focus(nullptr)
{
}


Layout::~Layout()
{
    // Deallocate all widgets
    const Widget* widget = m_first;
    while (widget != nullptr)
    {
        const Widget* next = widget->m_next;
        delete widget;
        widget = next;
    }
}


Layout* Layout::addLayout(Type type)
{
    Layout* layout = new Layout(type);
    add(layout);
    return layout;
}


Widget* Layout::add(Widget* widget)
{
    sf::Vector2f pos;
    if (m_last != nullptr)
    {
        if (m_type == Layout::Vertical)
            pos.y = m_last->getPosition().y + m_last->getSize().y + Theme::margin;
        else if (m_type == Layout::Horizontal)
            pos.x = m_last->getPosition().x + m_last->getSize().x + Theme::margin;
    }
    widget->setPosition(pos);
    push(widget);
    return widget;
}


Label* Layout::addLabel(const sf::String& string)
{
    Label* label = new Label(string);
    add(label);
    return label;
}


Button* Layout::addButton(const sf::String& label, std::function<void(void)> callback)
{
    Button* button = new Button(label);
    button->setCallback(callback);
    add(button);
    return button;
}


Widget* Layout::addRow(const sf::String& str, Widget* widget)
{
    if (m_type != Layout::Form)
        throw std::logic_error("Layout::Form is required for adding a new row");

    sf::Vector2f pos;
    if (m_last != nullptr)
        pos.y = getSize().y + Theme::margin;

    // Label on the left side
    Label* label = new Label(str);
    label->setPosition(pos);
    push(label);

    // Widget on the right side
    float width = label->getSize().x + Theme::margin;
    if (width > m_form.label_width)
    {
        m_form.label_width = width;
        size_t i = 0;
        for (Widget* w = m_first; w != nullptr; w = w->m_next)
        {
            // Re-align previous widgets
            if (i % 2)
                w->setPosition(width, w->getPosition().y);
            ++i;
        }
    }

    widget->setPosition(
        m_form.label_width,
        // Align bottom of widget with bottom of label
        pos.y + (Theme::font.getGlyphHeight() + Theme::padding - widget->getSize().y));
    push(widget);
    return widget;
}


Widget* Layout::push(Widget* widget)
{
    widget->setParent(this);

    if (m_first == nullptr)
    {
        // Widget is the first element
        m_first = m_last = widget;
    }
    else
    {
        // Append widget to the linked list
        m_last->m_next = widget;
        widget->m_previous = m_last;
        m_last = widget;
    }
    recomputeSize();
    return widget;
}


void Layout::recomputeSize()
{
    float max_width = 0;
    float max_height = 0;
    //float max_visible_width = 0;
    //float max_visible_height = 0;
    for (const Widget* widget = m_first; widget != nullptr; widget = widget->m_next)
    {
        float width = widget->getPosition().x + widget->getSize().x;
        if (width > max_width)
            max_width = width;

        float height = widget->getPosition().y + widget->getSize().y;
        if (height > max_height)
            max_height = height;

        /*float visible_width = widget.getX() + widget.getVisibleWidth();
        if (visible_width > max_visible_width)
            max_visible_width = visible_width;

        float visible_height = widget.getY() + widget.getVisibleHeight();
        if (visible_height > max_visible_height)
            max_visible_height = visible_height;*/
    }

    setSize(max_width, max_height);
    //setVisibleSize(max_visible_width, max_visible_height);
}


void Layout::onStateChanged(State state)
{
    if (state == StateDefault)
    {
        if (m_focus != nullptr)
        {
            m_focus->setState(StateDefault);
            m_focus = nullptr;
        }
    }
}


void Layout::onMouseMoved(const sf::Vector2f& pos)
{
    for (Widget* widget = m_first; widget != nullptr; widget = widget->m_next)
    {
        // Convert mouse position to widget coordinates system
        sf::Vector2f mouse = pos - widget->getPosition();
        if (widget->containsPoint(mouse))
        {
            if (m_hover != widget)
            {
                // A new widget is hovered
                if (m_hover != nullptr)
                    m_hover->setState(StateDefault);

                m_hover = widget;
                // Don't send Hovered state if widget is already focused
                if (m_hover != m_focus)
                {
                    widget->setState(StateHovered);
                }
            }
            else
            {
                // Hover on the same previously hovered widget
                widget->onMouseMoved(mouse);
            }
            return;
        }
    }
    // No widget hovered, remove hovered state
    if (m_hover != nullptr)
    {
        m_hover->onMouseMoved(pos);
        m_hover->setState(m_focus == m_hover ? StateFocused : StateDefault);
        m_hover = nullptr;
    }
}


void Layout::onMousePressed(const sf::Vector2f& pos)
{
    if (m_hover != nullptr)
    {
        // Clicked widget takes focus
        if (m_focus != m_hover)
        {
            giveFocus(m_hover);
        }
        // Convert mouse position to widget coordinates system
        m_hover->onMousePressed(pos - m_hover->getPosition());
    }
    // User didn't click on a widget, remove focus state
    else if (m_focus != nullptr)
    {
        m_focus->setState(StateDefault);
        m_focus = nullptr;
    }
}


void Layout::onMouseReleased(const sf::Vector2f& pos)
{
    if (m_focus != nullptr)
    {
        // Convert mouse position to widget coordinates system
        sf::Vector2f mouse = pos - m_focus->getPosition();
        if (m_focus->containsPoint(mouse))
        {
            m_focus->onMouseReleased(mouse);
        }
    }
}


void Layout::onMouseWheelMoved(int delta)
{
    if (m_focus != nullptr)
    {
        m_focus->onMouseWheelMoved(delta);
    }
}


void Layout::onKeyPressed(sf::Keyboard::Key key)
{
    if (key == sf::Keyboard::Down)
    {
        if (!focusNextWidget())
        {
            // Try to focus first widget if possible
            focusNextWidget();
        }
    }
    else if (key == sf::Keyboard::Up)
    {
        if (!focusPreviousWidget())
            focusPreviousWidget();
    }
    else if (m_focus != nullptr)
    {
        m_focus->onKeyPressed(key);
    }
}


void Layout::onKeyReleased(sf::Keyboard::Key key)
{
    if (m_focus != nullptr)
    {
        m_focus->onKeyReleased(key);
    }
}


void Layout::onTextEntered(sf::Uint32 unicode)
{
    if (m_focus != nullptr)
    {
        m_focus->onTextEntered(unicode);
    }
}


void Layout::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    transformStates(states);

    for (const Widget* widget = m_first; widget != nullptr; widget = widget->m_next)
    {
        target.draw(*widget, states);
    }
}


bool Layout::giveFocus(Widget* widget)
{
    if (widget != nullptr)
    {
        // If another widget was already focused, remove focus
        if (m_focus != nullptr)
        {
            m_focus->setState(StateDefault);
            m_focus = nullptr;
        }
        // Apply focus to widget
        if (widget->isSelectable())
        {
            m_focus = widget;
            widget->setState(StateFocused);
            return true;
        }
    }
    return false;
}


bool Layout::focusPreviousWidget()
{
    // If a subframe is already focused
    if (m_focus != nullptr && m_focus->toLayout() != nullptr)
    {
        if (m_focus->toLayout()->focusPreviousWidget())
            return true;
    }

    Widget* start = m_focus != nullptr ? m_focus->m_previous : m_last;
    for (Widget* widget = start; widget != nullptr; widget = widget->m_previous)
    {
        if (widget != nullptr)
        {
            Layout* container = widget->toLayout();
            if (container != nullptr)
            {
                if (container->focusPreviousWidget())
                {
                    giveFocus(container);
                    return true;
                }
            }
            else if (giveFocus(widget))
            {
                return true;
            }
        }
    }

    if (m_focus != nullptr)
        m_focus->setState(StateDefault);
    m_focus = nullptr;
    return false;
}


bool Layout::focusNextWidget()
{
    // If a subframe is already focused
    if (m_focus != nullptr && m_focus->toLayout() != nullptr)
    {
        if (m_focus->toLayout()->focusNextWidget())
            return true;
    }

    Widget* start = m_focus != nullptr ? m_focus->m_next : m_first;
    for (Widget* widget = start; widget != nullptr; widget = widget->m_next)
    {
        if (widget != nullptr)
        {
            Layout* container = widget->toLayout();
            if (container != nullptr)
            {
                if (container->focusNextWidget())
                {
                    giveFocus(container);
                    return true;
                }
            }
            else if (giveFocus(widget))
            {
                return true;
            }
        }
    }

    if (m_focus != nullptr)
        m_focus->setState(StateDefault);
    m_focus = nullptr;
    return false;
}
