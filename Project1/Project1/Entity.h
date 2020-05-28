#pragma once

#include "HitboxComponent.h"
#include "MovementComponent.h"
#include "AnimationComponent.h"

class Entity
{
private:
	void initializeVariables();
protected:
	sf::Sprite sprite;

	float scaleX;
	float scaleY;

	HitboxComponent* hitboxComponent;
	MovementComponent* movementComponent;
	AnimationComponent* animationComponent;

public:
	Entity();
	virtual ~Entity();

	virtual const sf::Vector2f& getPosition() const;
	virtual const sf::Vector2u getGridPosition(const unsigned gridSize) const;
	virtual const sf::FloatRect getGlobalBounds() const;
	virtual const sf::FloatRect& getNextPosition(const float& dt) const;

	void setScale(float scaleX, float scaleY);

	//Components
	void createHitboxComponent(sf::Sprite& sprite, float xOffset, float yOffset, float width, float height);
	void createMovementComponent(const float maxVelocity, const float acceleration, const float deceleration);
	void createAnimationComponent(sf::Texture& spriteSheet);

	MovementComponent& getMoveComp();

	void stopVelocity();
	void stopVelocityY();
	void stopVelocityX();

	//Functions
	virtual void setPosition(const float x, const float y);
	virtual void move(const float x, const float y, const float& dt);

	virtual void update(const float& dt) = 0;
	virtual void render(sf::RenderTarget& target) = 0;
};

