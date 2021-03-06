#include "stdafx.h"
#include "Gui.h"

gui::Button::Button(sf::Vector2f position, sf::Vector2f dimensions, sf::Font* font, std::string text, int fontsize, sf::Color fontIdleColor, sf::Color fontHoverColor, sf::Color fontActiveColor, sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor, sf::Color outlineIdleColor, sf::Color outlineHoverColor, sf::Color outlineActiveColor, short unsigned id)
{
	buttonState = IDLE;
	setPosition(position);
	setDimensions(dimensions);
	setId(id);
	setTextSettings(text, font, fontsize, fontIdleColor);
	setButtonColors(idleColor, hoverColor, activeColor);
	setFontColors(fontIdleColor, fontHoverColor, fontActiveColor);
	//setOutlineColors(outlineIdleColor, outlineHoverColor, outlineActiveColor);
}

gui::Button::~Button()
{
}

const bool gui::Button::isPressed() const
{
	return buttonState == ACTIVE;
}

sf::Vector2f gui::Button::getDimesions()
{
	return sf::Vector2f(shape.getSize().x, shape.getSize().y);
}

sf::FloatRect gui::Button::getGlobalBounds()
{
	return shape.getGlobalBounds();
}

sf::Vector2f gui::Button::getPosition()
{
	return shape.getPosition();
}

void gui::Button::update(const sf::Vector2i& mousePosWindow, const sf::Vector2f& mousePosView, bool worldButton)
{
	buttonState = IDLE;

	if (shape.getGlobalBounds().contains((worldButton == false ? static_cast<sf::Vector2f>(mousePosWindow) : mousePosView)))
	{
		buttonState = HOVERED;

		if ((worldButton == false ? sf::Mouse::isButtonPressed(sf::Mouse::Left) : sf::Mouse::isButtonPressed(sf::Mouse::Right)))
		{
			buttonState = ACTIVE;
		}
	}

	switch (buttonState)
	{
	case IDLE:
		shape.setFillColor(idleColor);
		shape.setOutlineColor(outlineIdleColor);
		text.setFillColor(fontIdleColor);
		break;
	case HOVERED:
		shape.setFillColor(hoverColor);
		shape.setOutlineColor(outlineHoverColor);
		text.setFillColor(fontHoverColor);
		break;
	case ACTIVE:
		shape.setFillColor(activeColor);
		shape.setOutlineColor(outlineActiveColor);
		text.setFillColor(fontActiveColor);
		break;
	default:
		break;
	}
}

void gui::Button::render(sf::RenderTarget& target)
{
	target.draw(shape);
	target.draw(text);
}

void gui::Button::setOrigin(sf::Vector2f origin)
{
	shape.setOrigin(origin.x, origin.y);
	text.setOrigin(origin.x, origin.y);
}

void gui::Button::setFillColor(sf::Color color)
{
	shape.setFillColor(color);
}

void gui::Button::setTexture(sf::Texture* texture)
{
	shape.setTexture(texture);
}

//Setters
void gui::Button::setPosition(sf::Vector2f position)
{
	shape.setPosition(position);
	setTextPosition();
}

void gui::Button::setDimensions(sf::Vector2f dimensions)
{
	shape.setSize(dimensions);
}

void gui::Button::setTextSettings(std::string text, sf::Font* font, int fontsize, sf::Color fontColor)
{
	this->text.setString(text);
	this->text.setFont(*font);
	this->text.setCharacterSize(fontsize);
	this->text.setFillColor(fontColor);
	this->text.setPosition(
		shape.getPosition().x + (shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		shape.getPosition().y + (shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height / 1.5f);
}

void gui::Button::setText(std::string text)
{
	this->text.setString(text);
}

void gui::Button::setTextPosition()
{
	this->text.setPosition(
		shape.getPosition().x + (shape.getGlobalBounds().width / 2.f) - this->text.getGlobalBounds().width / 2.f,
		shape.getPosition().y + (shape.getGlobalBounds().height / 2.f) - this->text.getGlobalBounds().height / 1.5f);
}

void gui::Button::setId(short unsigned id)
{
	this->id = id;
}

void gui::Button::setFontColors(sf::Color fontIdleColor, sf::Color fontHoverColor, sf::Color fontActiveColor)
{
	this->fontIdleColor = fontIdleColor;
	this->fontHoverColor = fontHoverColor;
	this->fontActiveColor = fontActiveColor;
}

void gui::Button::setButtonColors(sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor)
{
	this->idleColor = idleColor;
	this->hoverColor = hoverColor;
	this->activeColor = activeColor;
}

void gui::Button::setOutlineColors(sf::Color outlineIdleColor, sf::Color outlineHoverColor, sf::Color outlineActiveColor)
{
	shape.setOutlineThickness(1);

	this->outlineIdleColor = outlineIdleColor;
	this->outlineHoverColor = outlineHoverColor;
	this->outlineActiveColor = outlineActiveColor;
}

const short unsigned& gui::Button::getID() const
{
	return id;
}

const std::string gui::Button::getText() const
{
	return text.getString();
}


//----------------------------------------
//
//           Drop Down Menu
//
//----------------------------------------

gui::DropDownMenu::DropDownMenu(float x, float y, float width, float height, sf::Font& font, std::string list[], unsigned numOfElements, unsigned defaultIndex)
{
	showList = false;
	keyTime = 0.f;
	keyTimeMax = 2.f;

	activeElement = 
		new gui::Button(
			//position              dimensions
			sf::Vector2f(x, y), sf::Vector2f(width, height),
			//font     text    size
			&font, list[defaultIndex], 30,
			//fontidle               fonthover                fontactive
			sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 200), sf::Color(20, 20, 20, 50),
			//idlecolor               hovercolor                activecolor
			sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
			//outlineidle               outlinehover                   outlineactive
			sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 255), sf::Color(255, 255, 255, 255),
			1
		);

	for (size_t i = 0; i < numOfElements; i++)
	{
		elements.push_back(
			new gui::Button(
				//position              dimensions
				sf::Vector2f(x, y + ((i + 1) * height)), sf::Vector2f(width, height),
				//font     text    size
				&font, list[i], 30,
				//fontidle               fonthover                fontactive
				sf::Color(255, 255, 255, 150), sf::Color(255, 255, 255, 255), sf::Color(20, 20, 20, 50),
				//idlecolor               hovercolor                activecolor
				sf::Color(70, 70, 70, 200), sf::Color(150, 150, 150, 200), sf::Color(20, 20, 20, 200),
				//outlineidle               outlinehover                   outlineactive
				sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0), sf::Color(255, 255, 255, 0),
				//id
				i + 1
			));
	}


}

gui::DropDownMenu::~DropDownMenu()
{
	delete activeElement;
	for (int i = 0; i < elements.size(); i++)
	{
		delete elements[i];
	}
}

bool gui::DropDownMenu::getKeyTime()
{
	if (keyTime >= keyTimeMax)
	{
		keyTime = 0.f;
		return true;
	}
	else
	{
		return false;
	}
}

gui::Button* gui::DropDownMenu::getActiveElement()
{
	return activeElement;
}

void gui::DropDownMenu::updateKeyTime(const float& dt)
{
	if (keyTime < keyTimeMax)
	{
		keyTime += 10.f * dt;
	}
}

void gui::DropDownMenu::update(const sf::Vector2i& mousePosWindow, const float& dt)
{
	updateKeyTime(dt);
	activeElement->update(mousePosWindow);

	if (activeElement->isPressed() && getKeyTime())
	{
		showList = !showList;
	}

	if (showList)
	{
		for (auto& i : elements)
		{
			i->update(mousePosWindow);

			if (i->isPressed() && getKeyTime())
			{
				showList = false;
				activeElement->setId(i->getID());
				activeElement->setText(i->getText());
			}
		}
	}
}

void gui::DropDownMenu::render(sf::RenderTarget& target)
{
	activeElement->render(target);

	if (showList)
	{
		for (auto& i : elements)
		{
			i->render(target);
		}
	}
}



//----------------------------------------
//
//           Texture Selector
//
//----------------------------------------

gui::TextureSelector::TextureSelector(float x, float y, float bX, float bY, float width, float height, float gridSize, const sf::Texture* textureSheet, sf::Font& font)
{
	active = false;
	hidden = false;
	keyTime = 0.f;
	keyTimeMax = 2.f;
	float xoffset = 0.f;
	float yoffset = 30.f;
	this->gridSize = gridSize;

	bounds.setSize(sf::Vector2f(width, height));
	bounds.setPosition(x + xoffset, y + yoffset);
	bounds.setFillColor(sf::Color(50, 50, 50, 200));
	bounds.setOutlineThickness(1.f);
	bounds.setOutlineColor(sf::Color(255, 255, 255, 170));

	sheet.setTexture(*textureSheet);
	sheet.setPosition(x + xoffset, y + yoffset);

	if (this->sheet.getGlobalBounds().width > bounds.getGlobalBounds().width)
	{
		sheet.setTextureRect(sf::IntRect(0, 0, static_cast<int>(bounds.getGlobalBounds().width), static_cast<int>(sheet.getGlobalBounds().height)));
	}
	if (this->sheet.getGlobalBounds().height > bounds.getGlobalBounds().height)
	{
		sheet.setTextureRect(sf::IntRect(0, 0, static_cast<int>(sheet.getGlobalBounds().width), static_cast<int>(bounds.getGlobalBounds().height)));
	}

	textureRect.width = static_cast<int>(gridSize);
	textureRect.height = static_cast<int>(gridSize);

	selector.setPosition(x + xoffset, y + yoffset);
	selector.setSize(sf::Vector2f(gridSize, gridSize));
	selector.setFillColor(sf::Color::Transparent);
	selector.setOutlineThickness(1.f);
	selector.setOutlineColor(sf::Color::Red);

	hideButton =
		new gui::Button(
			//position              dimensions
			sf::Vector2f(bX, bY), sf::Vector2f(25, 25),
			//font     text    size
			&font, "", 30,
			//fontidle               fonthover                fontactive
			sf::Color(194, 10, 19), sf::Color(230, 10, 19), sf::Color(194, 10, 19),
			//idlecolor               hovercolor                activecolor
			sf::Color(80, 80, 80, 150), sf::Color(100, 100, 100, 200), sf::Color(115, 115, 115, 199)
		);
}

gui::TextureSelector::~TextureSelector()
{
	delete hideButton;
}

bool& gui::TextureSelector::getActive()
{
	return active;
}

const sf::IntRect gui::TextureSelector::getTextureRect() const
{
	return textureRect;
}

bool& gui::TextureSelector::isHidden()
{
	return hidden;
}

bool gui::TextureSelector::getKeyTime()
{
	if (keyTime >= keyTimeMax)
	{
		keyTime = 0.f;
		return true;
	}
	else
	{
		return false;
	}
}

void gui::TextureSelector::updateKeyTime(const float& dt)
{
	if (keyTime < keyTimeMax)
	{
		keyTime += 10.f * dt;
	}
}

void gui::TextureSelector::update(const sf::Vector2i& mousePosWindow, const float& dt)
{
	updateKeyTime(dt);
	hideButton->update(mousePosWindow);

	if (!hidden)
	{
		if (bounds.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow)))
		{
			active = true;
		}
		else
		{
			active = false;
		}

		if (active)
		{
			mousePosGrid.x = (mousePosWindow.x - static_cast<int>(bounds.getPosition().x)) / static_cast<unsigned>(gridSize);
			mousePosGrid.y = (mousePosWindow.y - static_cast<int>(bounds.getPosition().y)) / static_cast<unsigned>(gridSize);

			selector.setPosition(bounds.getPosition().x + mousePosGrid.x * gridSize, bounds.getPosition().y + mousePosGrid.y * gridSize);

			textureRect.left = static_cast<int>(selector.getPosition().x - bounds.getPosition().x);
			textureRect.top = static_cast<int>(selector.getPosition().y - bounds.getPosition().y);
		}
	}
}

void gui::TextureSelector::render(sf::RenderTarget& target)
{
	if (!hidden)
	{
		target.draw(bounds);
		target.draw(sheet);
		if (active)
		{
			target.draw(selector);
		}
	}
	hideButton->render(target);
}



//----------------------------------------
//
//                 HUD
//
//----------------------------------------

gui::HUD::HUD(sf::Font* font)
{
	initTextures();
	initGui(font);
}

void gui::HUD::initTextures()
{
	textures["Heart"].loadFromFile("Assets/Textures/UI/heart.png");
	textures["Sword"].loadFromFile("Assets/Textures/UI/sword.png");
	textures["Shield"].loadFromFile("Assets/Textures/UI/shield.png");
	textures["Currency"].loadFromFile("Assets/Textures/UI/currency.png");
}

void gui::HUD::initGui(sf::Font* font)
{
	heart.setSize(sf::Vector2f(50, 50));
	sword.setSize(sf::Vector2f(35, 35));
	shield.setSize(sf::Vector2f(35, 35));
	currency.setSize(sf::Vector2f(35, 35));

	heart.setPosition(21, 13);
	sword.setPosition(21, 800);
	shield.setPosition(21, 850);
	currency.setPosition(1349, 13);

	health.setFont(*font);
	strength.setFont(*font);
	defense.setFont(*font);
	money.setFont(*font);

	health.setCharacterSize(35);
	strength.setCharacterSize(35);
	defense.setCharacterSize(35);
	money.setCharacterSize(35);

	health.setPosition(heart.getPosition().x + 70, heart.getPosition().y + 5);
	strength.setPosition(sword.getPosition().x + 65, sword.getPosition().y - 5);
	defense.setPosition(shield.getPosition().x + 65, shield.getPosition().y - 6);
	money.setPosition(currency.getPosition().x + 58, currency.getPosition().y - 4);

	heart.setTexture(&textures["Heart"]);
	sword.setTexture(&textures["Sword"]);
	shield.setTexture(&textures["Shield"]);
	currency.setTexture(&textures["Currency"]);
}


void gui::HUD::update(PlayerStats stats)
{
	health.setString(std::to_string(stats.currentHealth) + "/" + std::to_string(stats.maxHealth));
	strength.setString(std::to_string(stats.strength));
	defense.setString(std::to_string(stats.defense));
	money.setString(std::to_string(stats.currency));
}

void gui::HUD::render(sf::RenderTarget& target)
{
	target.draw(heart);
	target.draw(sword);
	target.draw(shield);
	target.draw(currency);

	target.draw(health);
	target.draw(strength);
	target.draw(defense);
	target.draw(money);
}

//----------------------------------------
//
//              Shop Menu
//
//----------------------------------------

void gui::ShopMenu::initButtons(sf::Font* font)
{

	buttons["b_exit"] =
		new gui::Button(
			//position              dimensions
			sf::Vector2f(375, 110), sf::Vector2f(43, 40),
			//font     text    size
			font, "", 70,
			//fontidle               fonthover                fontactive
			sf::Color(232, 20, 5), sf::Color(232, 20, 5), sf::Color(232, 20, 5),
			//idlecolor               hovercolor                activecolor
			sf::Color(220, 220, 220), sf::Color(225, 225, 225), sf::Color(194, 0, 0)
		);
	buttons["b_exit"]->setTexture(&textures["EXITBUTTON"]);
	buttons["b_strength"] = 
		new gui::Button(
			//position              dimensions
			sf::Vector2f(579, 254), sf::Vector2f(320, 100),
			//font     text    size
			font, "Strength", 70,
			//fontidle               fonthover                fontactive
			sf::Color(232, 20, 5), sf::Color(232, 20, 5), sf::Color(232, 20, 5),
			//idlecolor               hovercolor                activecolor
			sf::Color(200, 185, 185), sf::Color(220, 220, 220), sf::Color(194, 0, 0)
		);
	buttons["b_strength"]->setTexture(&textures["BUTTON"]);
	buttons["b_defense"] =
		new gui::Button(
			//position              dimensions
			sf::Vector2f(579, 400), sf::Vector2f(320, 100),
			//font     text    size
			font, "Defense", 70,
			//fontidle               fonthover                fontactive
			sf::Color(232, 20, 5), sf::Color(232, 20, 5), sf::Color(232, 20, 5),
			//idlecolor               hovercolor                activecolor
			sf::Color(200, 185, 185), sf::Color(220, 220, 220), sf::Color(194, 0, 0)
		);
	buttons["b_defense"]->setTexture(&textures["BUTTON"]);
	buttons["b_vitality"] =
		new gui::Button(
			//position              dimensions
			sf::Vector2f(579, 546), sf::Vector2f(320, 100),
			//font     text    size
			font, "Vitality", 70,
			//fontidle               fonthover                fontactive
			sf::Color(232, 20, 5), sf::Color(232, 20, 5), sf::Color(232, 20, 5),
			//idlecolor               hovercolor                activecolor
			sf::Color(200, 185, 185), sf::Color(220, 220, 220), sf::Color(194, 0, 0)
		);
	buttons["b_vitality"]->setTexture(&textures["BUTTON"]);
}

void gui::ShopMenu::initText(sf::Font* font)
{
	text["currentStrength"].setFont(*font);
	text["currentStrength"].setPosition(sf::Vector2f(502, 282));
	text["currentStrength"].setCharacterSize(35);

	text["currentDefense"].setFont(*font);
	text["currentDefense"].setPosition(sf::Vector2f(502, 429));
	text["currentDefense"].setCharacterSize(35);

	text["currentVitality"].setFont(*font);
	text["currentVitality"].setPosition(sf::Vector2f(490, 572));
	text["currentVitality"].setCharacterSize(35);

	text["strCost"].setFont(*font);
	text["strCost"].setPosition(sf::Vector2f(930, 282));
	text["strCost"].setCharacterSize(35);

	text["defCost"].setFont(*font);
	text["defCost"].setPosition(sf::Vector2f(930, 429));
	text["defCost"].setCharacterSize(35);

	text["vitCost"].setFont(*font);
	text["vitCost"].setPosition(sf::Vector2f(930, 572));
	text["vitCost"].setCharacterSize(35);
}

void gui::ShopMenu::initShapes()
{
	//background
	shapes["bg_shape"].setSize(sf::Vector2f(1032, 845));
	shapes["bg_shape"].setTexture(&textures["BACKGROUND"]);
	shapes["bg_shape"].setPosition(234, 0);
}

void gui::ShopMenu::initTextures()
{
	textures["BACKGROUND"].loadFromFile("Assets/Textures/UI/shop_bg.png");
	textures["BUTTON"].loadFromFile("Assets/Textures/UI/shop_button.png");
	textures["EXITBUTTON"].loadFromFile("Assets/Textures/UI/shop_exit.png");
}

gui::ShopMenu::ShopMenu(sf::Font* font)
{
	hidden = true;
	initTextures();
	initButtons(font);
	initShapes();
	initText(font);
}

std::map<std::string, gui::Button*>& gui::ShopMenu::getButtons()
{
	return buttons;
}

bool& gui::ShopMenu::getHide()
{
	return hidden;
}

void gui::ShopMenu::update(const float& dt, const sf::Vector2i& mousePosWindow, PlayerStats stats)
{
	if (!hidden)
	{
		for (auto& i : buttons)
		{
			i.second->update(mousePosWindow);
		}

		text["currentVitality"].setString(std::to_string(stats.maxHealth));
		text["currentStrength"].setString(std::to_string(stats.strength));
		text["currentDefense"].setString(std::to_string(stats.defense));

		text["strCost"].setString(std::to_string((int)(stats.strength * 3) * 100));
		text["defCost"].setString(std::to_string((int)(stats.defense * 3) * 100));
		text["vitCost"].setString(std::to_string((int)(stats.maxHealth) * 3));
	}
}


void gui::ShopMenu::render(sf::RenderTarget& target)
{
	if (!hidden)
	{
		for (auto& i : shapes)
		{
			target.draw(i.second);
		}
		for (auto& i : buttons)
		{
			if (i.second)
			{
				i.second->render(target);
			}
		}
		for (auto& i : text)
		{
			target.draw(i.second);
		}
	}
}
