#include "stdafx.h"
#include "Player.h"

Player::Player(float x, float y, sf::Texture& spriteSheet)
{
	createHitboxComponent(sprite, 40.f, 60.f, 33, 60);
	createMovementComponent(250.f, 90.f, 50.f);
	createAnimationComponent(spriteSheet);

	initializeVariables();
	initializeAnimations();

	setOrigin(sprite.getGlobalBounds().width / 2.f, sprite.getGlobalBounds().height / 2.f);
	setPosition(x, y);

	loadPlayerStats("Data/player/save.dat");
}

Player::~Player()
{
	savePlayerStats("Data/player/save.dat");
}

PlayerStats& Player::getStats()
{
	return stats;
}

void Player::initializeVariables()
{
	attacking = false;
}

void Player::initializeAnimations()
{
	//creating animations from strips of the player sprite sheet.
	animationComponent->addAnimation("IDLE_DOWN", 10.f, 0, 0, 15, 0, 99, 101, 0);
	animationComponent->addAnimation("IDLE_DLEFT", 10.f, 0, 1, 15, 1, 99, 101, 0);
	animationComponent->addAnimation("IDLE_LEFT", 10.f, 0, 2, 15, 2, 99, 101, 0);
	animationComponent->addAnimation("IDLE_ULEFT", 10.f, 0, 3, 15, 3, 99, 101, 0);
	animationComponent->addAnimation("IDLE_UP", 10.f, 0, 4, 15, 4, 99, 101, 0);
	animationComponent->addAnimation("IDLE_DRIGHT", 10.f, 0, 7, 15, 7, 99, 101, 0);
	animationComponent->addAnimation("IDLE_RIGHT", 10.f, 0, 6, 15, 6, 99, 101, 0);
	animationComponent->addAnimation("IDLE_URIGHT", 10.f, 0, 5, 15, 5, 99, 101, 0);

	animationComponent->addAnimation("WALK_DOWN", 10.f, 0, 8, 9, 8, 99, 101, 0);
	animationComponent->addAnimation("WALK_DLEFT", 10.f, 0, 9, 9, 9, 99, 101, 0);
	animationComponent->addAnimation("WALK_LEFT", 10.f, 0, 10, 9, 10, 99, 101, 0);
	animationComponent->addAnimation("WALK_ULEFT", 10.f, 0, 11, 9, 11, 99, 101, 0);
	animationComponent->addAnimation("WALK_UP", 10.f, 0, 12, 9, 12, 99, 101, 0);
	animationComponent->addAnimation("WALK_URIGHT", 10.f, 0, 13, 9, 13, 99, 101, 0);
	animationComponent->addAnimation("WALK_RIGHT", 10.f, 0, 14, 9, 14, 99, 101, 0);
	animationComponent->addAnimation("WALK_DRIGHT", 10.f, 0, 15, 9, 15, 99, 101, 0);

	animationComponent->addAnimation("ATTACK_DOWN", 7.f, 0, (0 * 116) + 1627, (11 * 179), (0 * 116) + 1627, 179, 116, 1);
	animationComponent->addAnimation("ATTACK_DLEFT", 7.f, 0, (1 * 116) + 1627, (11 * 179), (1 * 116) + 1627, 179, 116, 1);
	animationComponent->addAnimation("ATTACK_LEFT", 7.f, 0, (2 * 116) + 1627, (11 * 179), (2 * 116) + 1627, 179, 116, 1);
	animationComponent->addAnimation("ATTACK_ULEFT", 7.f, 0, (3 * 116) + 1627, (11 * 179), (3 * 116) + 1627, 179, 116, 1);
	animationComponent->addAnimation("ATTACK_UP", 7.f, 0, (4 * 116) + 1627, (11 * 179), (4 * 116) + 1627, 179, 116, 1);
	animationComponent->addAnimation("ATTACK_URIGHT", 7.f, 0, (5 * 116) + 1627, (11 * 179), (5 * 116) + 1627, 179, 116, 1);
	animationComponent->addAnimation("ATTACK_RIGHT", 7.f, 0, (6 * 116) + 1627, (11 * 179), (6 * 116) + 1627, 179, 116, 1);
	animationComponent->addAnimation("ATTACK_DRIGHT", 7.f, 0, (7 * 116) + 1627, (11 * 179), (7 * 116) + 1627, 179, 116, 1);
}

void Player::loadPlayerStats(std::string path)
{
	std::ifstream fin;

	fin.open(path);

	if (fin.is_open())
	{
		fin >> stats.maxHealth >> stats.strength >> stats.defense >> stats.currency;
		stats.currentHealth = stats.maxHealth;
	}
}

void Player::savePlayerStats(std::string path)
{
	std::ofstream fout;

	fout.open(path);

	if (fout.is_open())
	{
		fout << stats.maxHealth << " " << stats.strength << " " << stats.defense << " " << stats.currency;
	}
}

void Player::resetOrigin()
{
	if (sprite.getOrigin().x != 0 || sprite.getOrigin().y != 0)
	{
		sprite.setOrigin(0, 0);
	}
}

void Player::setOrigin(float x, float y)
{
	if (sprite.getOrigin().x != x || sprite.getOrigin().y != y)
	{
		sprite.setOrigin(x, y);
	}
}

bool& Player::setAttacking()
{
	return attacking;
}

sf::Vector2f Player::getVelocity()
{
	return movementComponent->getVelocity();
}

sf::Vector2f Player::getPositionInWorld()
{
	return movementComponent->getPosInWorld();
}

void Player::updateAngleDirection()
{
	//takes the players x and y velocity to find the direction of movement, then divides by 45
	//to turn 360 possible values into 8.
	//angle direction is an integer to truncate to a whole number.
	angleDirection = (atan2f(movementComponent->getVelocity().y, movementComponent->getVelocity().x) * 180 / 3.14159265359) / 45;
}

sf::Vector2f Player::getPos()
{
	return sf::Vector2f(sprite.getPosition().x, sprite.getPosition().y);
}


std::string Player::getAnimationFromAngle(std::string type)
{
	switch (angleDirection)
	{
		case 2:
			return type + "_DOWN";
		case 3:
			return type + "_DLEFT";
		case 4:
			return type + "_LEFT";
		case -3:
			return type + "_ULEFT";
		case -2:
			return type + "_UP";
		case -1:
			return type + "_URIGHT";
		case 0:
			return type + "_RIGHT";
		case 1:
			return type + "_DRIGHT";
		default:
			break;
	}
}

void Player::updateInput()
{

}

void Player::updateAnimations(const float& dt)
{
	if (getVelocity() != sf::Vector2f(0, 0) && !attacking)
	{
		if (sprite.getOrigin().x != 0 || sprite.getOrigin().y != 0)
		{
			sprite.setOrigin(0, 0);
		}
		animationComponent->Play(getAnimationFromAngle("WALK"), dt);
	}
	else if (getVelocity() == sf::Vector2f(0, 0) && !attacking)
	{
		sprite.setOrigin(0, 0);
		animationComponent->Play(getAnimationFromAngle("IDLE"), dt);
	}
	if (attacking)
	{
		animationComponent->Play(getAnimationFromAngle("ATTACK"), dt, true);

		if (!animationComponent->isAnimationDone(getAnimationFromAngle("ATTACK")))
		{
			sprite.setOrigin(35, 4);
		}
		if (animationComponent->isAnimationDone(getAnimationFromAngle("ATTACK")))
		{
			attacking = false;
		}
	}
}

void Player::render(sf::RenderTarget& target)
{
	target.draw(sprite);
	//hitboxComponent->render(target);
}

void Player::update(const float& dt)
{
	movementComponent->update(dt);
	updateInput();
	updateAnimations(dt);
	hitboxComponent->update();
}
