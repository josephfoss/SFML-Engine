#include "stdafx.h"
#include "Animation.h"

Animation::Animation(sf::Sprite& sprite, sf::Texture& spriteSheet, float animationTimer, int startFrameX, int startFrameY, int xFrameTotal, int yFrameTotal, int width, int height, bool oddShaped)
	: sprite(sprite), spriteSheet(spriteSheet), animationTimer(animationTimer), timer(0.f), done(false), frameTotal(xFrameTotal), width(width), height(height)
{
	if (oddShaped == false)
	{
		this->startRect = sf::IntRect(startFrameX * width, startFrameY * height, width, height);
		this->currentRect = this->startRect;
		this->endRect = sf::IntRect(xFrameTotal * width, yFrameTotal * height, width, height);
	}
	else
	{
		this->startRect = sf::IntRect(startFrameX, startFrameY, width, height);
		this->currentRect = this->startRect;
		this->endRect = sf::IntRect(xFrameTotal, yFrameTotal, width, height);
	}

	this->sprite.setTexture(this->spriteSheet, true);
	this->sprite.setTextureRect(this->startRect);
}

const bool& Animation::isDone()
{
	return done;
}

const bool& Animation::play(const float& dt)
{
	done = false;
	//increments the timer.
	timer += 100.f * dt;
	//changing the animationTimer variable changes the time between frames.
	if (timer >= animationTimer)
	{
		//restarts the timer.
		timer = 0.f;

		if (currentRect != endRect)
		{
			//goes to the next frame in the animation.
			currentRect.left += width;
		}
		else
		{
			//restarts the animation.
			currentRect.left = startRect.left;
			done = true;
		}
		//sets the texture to the currentRect
		this->sprite.setTextureRect(currentRect);
	}
	return done;
}

void Animation::reset()
{
	timer = animationTimer;
	currentRect = startRect;
}