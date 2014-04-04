#ifndef GUI_FRAME_HPP
#define GUI_LAYOUT_HPP

#include "Widget.hpp"

namespace gui
{

class Widget;
class Label;
class Button;

/**
 * Special widget which acts as a container with a specified layout
 */
class Layout: public Widget
{
public:
	enum Type
	{
		Vertical,
		Horizontal,
		Form
	};

	Layout(Type type = Vertical);
	~Layout();

	Layout* addLayout(Type type);

	/**
	 * Add a new widget in the container
	 * The container will take care of widget deallocation
	 * @return added widget
	 */
	Widget* add(Widget* widget, int id = -1);
	Button* addButton(const sf::String& string, int id = -1);
	Label*  addLabel(const sf::String& string);
	Widget* addRow(const sf::String& str, Widget* widget, int id = -1);

protected:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

	// Callbacks ---------------------------------------------------------------
	void onStateChanged(State state) override;
	void onMouseMoved(float x, float y) override;
	void onMousePressed(float x, float y) override;
	void onMouseReleased(float x, float y) override;
	void onMouseWheelMoved(int delta) override;
	void onKeyPressed(sf::Keyboard::Key key) override;
	void onKeyReleased(sf::Keyboard::Key key) override;
	void onTextEntered(sf::Uint32 unicode) override;

	Layout* toLayout() override { return this; }
	bool focusNextWidget();
	bool focusPreviousWidget();

private:
	bool giveFocus(Widget* widget);

	void recomputeSize();

	Widget* push(Widget* widget);

	Widget*   m_first;
	Widget*   m_last;
	Type      m_type;
	Widget*   m_hover;
	Widget*   m_focus;

	struct FormInfo // Layout::Form
	{
		FormInfo():
			label_width(0.f)
		{
		}

		float label_width;
	};
	FormInfo m_form;
};

}

#endif // GUI_LAYOUT_HPP
