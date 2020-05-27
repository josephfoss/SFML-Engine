#pragma once
#include "Animation.h"

class AnimationComponent
{
private:

	sf::Sprite& sprite;
	sf::Texture& spriteSheet;
	std::map<std::string, Animation*> animations;

	Animation* lastAnimation;
	Animation* priorityAnimation;

public:
	AnimationComponent(sf::Sprite& sprite, sf::Texture& spriteSheet);
	~AnimationComponent();

	void addAnimation(std::string key, float animationTimer, int startFrameX, int startFrameY, int xFrameTotal, int yFrameTotal, int width, int height, bool oddShaped);
	Animation getAnimation(std::string key);

	const bool& isAnimationDone(const std::string key);
	const bool& Play(const std::string key, const float& dt, const bool priority = false);
	void checkForReset(const std::string key);
	//void Pause(const std::string key);
	//void Restart(const std::string key);
};

