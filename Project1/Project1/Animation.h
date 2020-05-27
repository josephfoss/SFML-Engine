#pragma once

class Animation
{
public:
	sf::Sprite& sprite;
	sf::Texture& spriteSheet;

	sf::IntRect startRect;
	sf::IntRect currentRect;
	sf::IntRect endRect;

	bool done;
	int width;
	int height;
	float timer;
	float animationTimer;
	float frameTotal;

	Animation(sf::Sprite& sprite, sf::Texture& spriteSheet, float animationTimer, int startFrameX, int startFrameY, int xFrameTotal, int yFrameTotal, int width, int height, bool oddShaped);

	const bool& isDone();
	const bool& play(const float& dt);
	void reset();
};