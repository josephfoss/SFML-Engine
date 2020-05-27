#include "stdafx.h"
#include "AnimationComponent.h"

AnimationComponent::AnimationComponent(sf::Sprite& sprite, sf::Texture& spriteSheet)
	:sprite(sprite), spriteSheet(spriteSheet), lastAnimation(NULL), priorityAnimation(NULL)
{

}

AnimationComponent::~AnimationComponent()
{
	for (auto &i : animations)
	{
		delete i.second;
	}
}

void AnimationComponent::addAnimation(std::string key, float animationTimer, int startFrameX, int startFrameY, int xFrameTotal, int yFrameTotal, int width, int height, bool oddShaped)
{
	animations[key] = new Animation(this->sprite, this->spriteSheet, animationTimer, startFrameX, startFrameY, xFrameTotal, yFrameTotal, width, height, oddShaped);
	//Playing the animation as its added fixes a bug where the sprite will jump around on the first play.
	//not sure why this happens but it seems like the animation needs to be played to initialize correctly
	Play(key, 1);
}

Animation AnimationComponent::getAnimation(std::string key)
{
	return *animations[key];
}

const bool& AnimationComponent::isAnimationDone(const std::string key)
{
	return animations[key]->isDone();
}

const bool& AnimationComponent::Play(const std::string key, const float& dt, const bool priority)
{
	if (priority)
	{
		priorityAnimation = animations[key];
	}

	if (priorityAnimation)
	{
		if (priorityAnimation == animations[key])
		{
			checkForReset(key);

			//sets the priority animation to null once it plays once.
			if (animations[key]->play(dt))
			{
				priorityAnimation = NULL;
			}
		}
	}
	else 
	{
		checkForReset(key);
		animations[key]->play(dt);
	}

	return animations[key]->isDone();
}

void AnimationComponent::checkForReset(const std::string key)
{
	if (lastAnimation != animations[key])
	{
		if (lastAnimation == NULL)
		{
			lastAnimation = animations[key];
		}
		else
		{
			lastAnimation->reset();
			lastAnimation = animations[key];
		}
	}
}
