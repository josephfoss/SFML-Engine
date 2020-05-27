#pragma once

class HitboxComponent
{
private:
	sf::Sprite& sprite;
	sf::RectangleShape hitbox;
	float xOffset;
	float yOffset;

public:
	HitboxComponent(sf::Sprite& sprite, float xOffset, float yOffset, float width, float height);
	~HitboxComponent();

	bool checkIntersect(const sf::FloatRect& rect);

	void update();
	void render(sf::RenderTarget& target);
};

