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

const sf::Vector2f& Entity::getPosition() const
{
	if(hitboxComponent)
		return hitboxComponent->getPosition();
	return sprite.getPosition();
}

const sf::Vector2u Entity::getGridPosition(const unsigned gridSize) const
{
	if(hitboxComponent)
		return sf::Vector2u(static_cast<unsigned>(hitboxComponent->getPosition().x) / gridSize, static_cast<unsigned>(hitboxComponent->getPosition().y) / gridSize);

	return sf::Vector2u(static_cast<unsigned>(sprite.getPosition().x) / gridSize, static_cast<unsigned>(sprite.getPosition().y) / gridSize);
}

const sf::FloatRect Entity::getGlobalBounds() const
{
	return hitboxComponent->getGlobalBounds();
}

const sf::FloatRect& Entity::getNextPosition(const float& dt) const
{
	return hitboxComponent->getNextPosition(movementComponent->getVelocity() * dt);
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

MovementComponent& Entity::getMoveComp()
{
	if(movementComponent)
		return *movementComponent;
}

void Entity::stopVelocity()
{
	movementComponent->stopVelocity();
}

void Entity::stopVelocityY()
{
	movementComponent->stopVelocityY();
}

void Entity::stopVelocityX()
{
	movementComponent->stopVelocityX();
}

void Entity::setPosition(const float x, const float y)
{
	if (hitboxComponent)
		hitboxComponent->setPosition(x, y);
	else
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
 
}

void Entity::render(sf::RenderTarget& target)
{

}

