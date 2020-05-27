#pragma once
#include "Gui.h"
#include "HitboxComponent.h"

class Prop
{
private:
	sf::RectangleShape shape;
	gui::Button* button;
	HitboxComponent* hitbox;

	int id;
	bool clickable;
	bool breakable;

public:
	Prop(unsigned x, unsigned y, float size, sf::Texture& texture, const sf::IntRect& rect, bool clickable = false, int id = -1, sf::Font* font = nullptr, bool breakable = false);
	~Prop();

	bool isClickable();
	bool isBreakable();

	int getID();
	sf::Vector2f getPosition();
	gui::Button* getPropButton();
	HitboxComponent* getPropHitbox();
	gui::Button* getButton();
	const std::string getAsString() const;


	void update(const sf::Vector2f& mousePosView);
	void render(sf::RenderTarget& target);
};

