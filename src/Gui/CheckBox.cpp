#include "CheckBox.hpp"

using namespace gui;

CheckBox::CheckBox(bool checked):
	Widget()
{
	m_box.setSize(Theme::getBaseLine(), Theme::getBaseLine());
	float size = Theme::getFont().getGlyphHeight() - Theme::PADDING;
	m_box.item().setSize(size);
	m_box.adjustItem();

	check(checked);

	setSize(m_box.getSize());
}


bool CheckBox::isChecked() const
{
	return m_checked;
}


void CheckBox::check(bool checked)
{
	m_checked = checked;
	m_box.item().setColor(m_checked ? Theme::TEXT_COLOR : sf::Color::Transparent);
}


void CheckBox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	transformStates(states);
	target.draw(m_box, states);
}

// callbacks -------------------------------------------------------------------

void CheckBox::onStateChanged(State state)
{
	m_box.applyState(state);
}


void CheckBox::onMouseReleased(float x, float y)
{
	if (containsPoint({x, y}))
	{
		check(!m_checked);
		triggerCallback();
	}
}


void CheckBox::onKeyPressed(sf::Keyboard::Key key)
{
	if (key == sf::Keyboard::Space)
	{
		check(!m_checked);
		triggerCallback();
	}
}
