#include "stdafx.h"
#include "HitboxComponent.h"



HitboxComponent::HitboxComponent(sf::Sprite& sprite, float xOffset, float yOffset, float width, float height)
	:sprite(sprite), xOffset(xOffset), yOffset(yOffset)
{
	nextPosition.left = 0.f;
	nextPosition.top = 0.f;
	nextPosition.width = width;
	nextPosition.height = height;

	hitbox.setPosition(this->sprite.getPosition().x + xOffset, this->sprite.getPosition().y + yOffset);
	this->hitbox.setSize(sf::Vector2f(width, height));
	this->hitbox.setFillColor(sf::Color::Transparent);
	this->hitbox.setOutlineThickness(-1.f);
	this->hitbox.setOutlineColor(sf::Color::Green);
}


HitboxComponent::~HitboxComponent()
{
}

const sf::Vector2f& HitboxComponent::getPosition() const
{
	return hitbox.getPosition();
}

const sf::FloatRect HitboxComponent::getGlobalBounds() const
{
	return hitbox.getGlobalBounds();
}

const sf::FloatRect& HitboxComponent::getNextPosition(const sf::Vector2f& velocity)
{
	nextPosition.left = hitbox.getPosition().x + velocity.x;
	nextPosition.top = hitbox.getPosition().y + velocity.y;

	return this->nextPosition;
}

void HitboxComponent::setPosition(const sf::Vector2f& position)
{
	hitbox.setPosition(position);
	sprite.setPosition(position.x - xOffset, position.y - yOffset);
}

void HitboxComponent::setPosition(const float x, const float y)
{
	hitbox.setPosition(x, y);
	sprite.setPosition(x - xOffset, y - yOffset);
}

bool HitboxComponent::checkIntersect(const sf::FloatRect& rect)
{
	return hitbox.getGlobalBounds().intersects(rect);
}

void HitboxComponent::update()
{
	hitbox.setPosition(sprite.getPosition().x + xOffset, sprite.getPosition().y + yOffset);
}

void HitboxComponent::render(sf::RenderTarget& target)
{
	target.draw(hitbox);
}
