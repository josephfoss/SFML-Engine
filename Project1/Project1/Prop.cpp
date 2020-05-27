#include "stdafx.h"
#include "Prop.h"


Prop::Prop(unsigned x, unsigned y, float size, sf::Texture& texture, const sf::IntRect& rect, bool clickable, int id, sf::Font* font, bool breakable)
{
	button = nullptr;
	hitbox = nullptr; 

	shape.setPosition(x, y);
	shape.setSize(sf::Vector2f(size, size));
	shape.setTexture(&texture);
	shape.setTextureRect(rect);

	this->id = id;
	this->clickable = clickable;
	this->breakable = breakable;

	if (clickable == true)
	{
		button =
			new gui::Button(
				//position              dimensions
				sf::Vector2f(x, y), sf::Vector2f(size, size),
				//font     text    size
				font, "", 75,
				//fontidle               fonthover                fontactive
				sf::Color(194, 10, 19), sf::Color(230, 10, 19), sf::Color(194, 10, 19),
				//idlecolor               hovercolor                activecolor
				sf::Color::Transparent, sf::Color::Transparent, sf::Color::Transparent
			);
	}
	if (breakable)
	{
		//TODO
	}
}


Prop::~Prop()
{
	delete button;
	delete hitbox;
}

bool Prop::isClickable()
{
	return clickable;
}

bool Prop::isBreakable()
{
	return breakable;
}

int Prop::getID()
{
	return id;
}

sf::Vector2f Prop::getPosition()
{
	return shape.getPosition();
}

gui::Button* Prop::getPropButton()
{
	if (button)
	{
		return button;
	}
}

HitboxComponent* Prop::getPropHitbox()
{
	if (hitbox)
	{
		return hitbox;
	}
}

gui::Button* Prop::getButton()
{
	return button;
}

const std::string Prop::getAsString() const
{
	std::stringstream ss;

	ss << shape.getTextureRect().left << " " << shape.getTextureRect().top << " " << clickable << " " << id << " " << breakable;

	return ss.str();
}

void Prop::update(const sf::Vector2f& mousePosView)
{
	if (button)
	{
		button->update(sf::Vector2i(0,0), mousePosView, true);
	}
	if (hitbox)
	{
		hitbox->update();
	}
}

void Prop::render(sf::RenderTarget& target)
{
	target.draw(shape);
}
