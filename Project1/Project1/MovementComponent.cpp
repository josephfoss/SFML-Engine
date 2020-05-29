#include "stdafx.h"
#include "MovementComponent.h"



MovementComponent::MovementComponent(sf::Sprite& sprite, float maxVelocity, float acceleration, float deceleration) 
	: sprite(sprite), maxVelocity(maxVelocity), acceleration(acceleration), deceleration(deceleration)
{
	this->maxVelocity = maxVelocity;
}


MovementComponent::~MovementComponent()
{

}

const sf::Vector2f& MovementComponent::getVelocity() const
{
	return velocity;
}

sf::Vector2f MovementComponent::getPosInWorld()
{
	return positionInWorld;
}

void MovementComponent::stopVelocity()
{
	velocity.x = 0.f;
	velocity.y = 0.f;
}

void MovementComponent::stopVelocityX()
{
	velocity.x = 0.f;
}

void MovementComponent::stopVelocityY()
{
	velocity.y = 0.f;
}

void MovementComponent::move(const float x, const float y, const float& dt)
{
	velocity.x += acceleration * x;

	velocity.y += acceleration * y;
}

void MovementComponent::update(const float& dt)
{
	if (velocity.x > 0.f) // positive x movement
	{
		positionInWorld.x++;
		if (velocity.x > maxVelocity)
			velocity.x = maxVelocity;

		velocity.x -= deceleration;
		if (velocity.x < 0.f)
			velocity.x = 0.f;
	}
	else if(velocity.x < 0.f) // negative x movement
	{
		positionInWorld.x--;
		if (velocity.x < -maxVelocity)
			velocity.x = -maxVelocity;

		velocity.x += deceleration;
		if (velocity.x > 0.f)
			velocity.x = 0.f;
	}

	if (velocity.y > 0.f) // positive y movement
	{
		positionInWorld.y++;
		if (velocity.y > maxVelocity)
			velocity.y = maxVelocity;

		velocity.y -= deceleration;
		if (velocity.y < 0.f)
			velocity.y = 0.f;
	}
	else if (velocity.y < 0.f) // negative y movement
	{
		positionInWorld.y--;
		if (velocity.y < -maxVelocity)
			velocity.y = -maxVelocity;

		velocity.y += deceleration;
		if (velocity.y > 0.f)
			velocity.y = 0.f;
	}

	sprite.move(velocity * dt);
}

