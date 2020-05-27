#include "stdafx.h"
#include "Entity.h"



void Entity::initializeVariables()
{
	hitboxComponent = NULL;
	movementComponent = NULL;
	animationComponent = NULL;
}

Entity::Entity()
{
	initializeVariables();
}

Entity::~Entity()
{
	delete hitboxComponent;
	delete movementComponent;
	delete animationComponent;
}

void Entity::setScale(float scaleX, float scaleY)
{
	this->scaleX = scaleX;
	this->scaleY = scaleY;
	sprite.setScale(sf::Vector2f(scaleX, scaleY));
}

void Entity::createHitboxComponent(sf::Sprite& sprite, float xOffset, float yOffset, float width, float height)
{
	hitboxComponent = new HitboxComponent(sprite, xOffset, yOffset, width, height);
}

void Entity::createMovementComponent(const float maxVelocity, const float acceleration, const float deceleration)
{
	movementComponent = new MovementComponent(sprite, maxVelocity, acceleration, deceleration);
}

void Entity::createAnimationComponent(sf::Texture& spriteSheet)
{
	animationComponent = new AnimationComponent(sprite, spriteSheet);
}

void Entity::setPosition(const float x, const float y)
{
	sprite.setPosition(x, y);
}

void Entity::move(const float vx, const float vy, const float& dt)
{
	if (movementComponent)
	{
		movementComponent->move(vx, vy, dt);
	}
}

void Entity::update(const float& dt)
{
	if (movementComponent)
	{
		movementComponent->update(dt);
	}
}

void Entity::render(sf::RenderTarget& target)
{
	target.draw(sprite);
}

