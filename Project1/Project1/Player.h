#pragma once
#include "Entity.h"

struct PlayerStats
{
	int maxHealth;
	int currentHealth;
	int strength;
	int defense;
	int currency;

	PlayerStats()
	{
		maxHealth = 100;
		currentHealth = 100;
		strength = 1;
		defense = 1;
		currency = 0;
	}
};

class Player :
	public Entity
{
private:
	void initializeVariables();
	void initializeAnimations();

	void loadPlayerStats(std::string path);
	void savePlayerStats(std::string path);

public:
	Player(float x, float y, sf::Texture& spriteSheet);
	virtual ~Player();
	PlayerStats stats;


	PlayerStats& getStats();

	int angleDirection;
	bool attacking;
	bool& setAttacking();

	sf::Vector2f getVelocity();
	sf::Vector2f getPositionInWorld();
	std::string getAnimationFromAngle(std::string);
	sf::Vector2f getPos();

	void updateAngleDirection();


	void resetOrigin();
	void setOrigin(float x, float y);

	void updateInput();
	void updateAnimations(const float& dt);

	virtual void render(sf::RenderTarget& target);
	virtual void update(const float& dt);
};

