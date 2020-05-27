#pragma once

class MovementComponent
{
private:
	sf::Sprite& sprite;
	sf::Vector2f velocity = { 0,0 };
	sf::Vector2f positionInWorld = { 0,0 };

	float maxVelocity;
	float acceleration;
	float deceleration;

public:

	MovementComponent(sf::Sprite& sprite, float maxVelocity, float acceleration, float deceleration);
	~MovementComponent();

	const sf::Vector2f& getVelocity() const;
	sf::Vector2f getPosInWorld();

	void move(const float x, const float y, const float& dt);
	void update(const float& dt);

};

