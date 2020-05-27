#include "stdafx.h"
#include "HitboxComponent.h"



HitboxComponent::HitboxComponent(sf::Sprite& sprite, float xOffset, float yOffset, float width, float height)
	:sprite(sprite), xOffset(xOffset), yOffset(yOffset)
{
	hitbox.setPosition(sprite.getPosition().x + xOffset, sprite.getPosition().y + yOffset);
	hitbox.setSize(sf::Vector2f(width, height));
	hitbox.setFillColor(sf::Color::Transparent);
	hitbox.setOutlineThickness(1.f);
	hitbox.setOutlineColor(sf::Color::Green);
}


HitboxComponent::~HitboxComponent()
{
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
;}
