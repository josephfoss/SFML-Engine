#pragma once

class HitboxComponent
{
private:
	sf::Sprite& sprite;
	sf::RectangleShape hitbox;
	sf::FloatRect nextPosition;
	float xOffset;
	float yOffset;

public:
	HitboxComponent(sf::Sprite& sprite, float xOffset, float yOffset, float width, float height);
	~HitboxComponent();

	const sf::Vector2f& getPosition() const;
	const sf::FloatRect getGlobalBounds() const;
	const sf::FloatRect& getNextPosition(const sf::Vector2f& velocity);

	void setPosition(const sf::Vector2f& position);
	void setPosition(const float x, const float y);

	bool checkIntersect(const sf::FloatRect& rect);

	void update();
	void render(sf::RenderTarget& target);
};

