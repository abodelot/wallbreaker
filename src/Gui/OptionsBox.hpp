#ifndef GUI_OPTIONSBOX_HPP
#define GUI_OPTIONSBOX_HPP

#include "Widget.hpp"
#include "BitmapText.hpp"
#include "Utils/Arrow.hpp"
#include "Utils/Box.hpp"

namespace gui
{

/**
 * Widget for selecting one item from a list
 * Triggers an event when selected item changes
 */
template <class T>
class OptionsBox: public Widget
{
public:
    OptionsBox();

    /**
     * Append a new item in the list
     * @param label: displayed label when selected
     * @param value: value associated
     * @param select: if true, new item becomes the default selection
     */
    void addItem(const sf::String& label, const T& value, bool select = false);

    /**
     * Make an item the current one
     * @param item_index: position of the item in the list
     */
    void selectItem(size_t item_index);

    /**
     * Get the value of the selected item
     * @return associated value
     */
    const T& getSelectedValue() const;

    /**
     * Get the index of the selected item
     */
    size_t getSelectedIndex() const;

    /**
     * Select next item in the list
     */
    void selectNext();

    /**
     * Select previous item in the list
     */
    void selectPrevious();

    // callbacks ---------------------------------------------------------------

    void onStateChanged(State state) override;
    void onMouseMoved(const sf::Vector2f& pos) override;
    void onMousePressed(const sf::Vector2f& pos) override;
    void onMouseReleased(const sf::Vector2f& pos) override;
    void onKeyPressed(sf::Keyboard::Key key) override;
    void onKeyReleased(sf::Keyboard::Key key) override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void updateArrow(Box<Arrow>& arrow, const sf::Vector2f& pos);

    void resizeContentBox(float width);

    struct Item
    {
        Item(const sf::String& string, const T& value);

        sf::String label;
        T value;
    };

    typedef std::vector<Item> ItemVector;
    ItemVector         m_items;
    size_t             m_currentIndex;

    // Visual components
    Box<BitmapText>    m_box;
    Box<Arrow>         m_arrowLeft;
    Box<Arrow>         m_arrowRight;
};

}

#include "OptionsBox.inl"

#endif // GUI_OPTIONSBOX_HPP
