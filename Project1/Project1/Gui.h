#pragma once
#include "Player.h"

enum button_states {IDLE = 0, HOVERED, ACTIVE};

namespace gui
{
	class Button
	{
	private:
		sf::RectangleShape shape;
		sf::Font* font;
		sf::Text text;

		sf::Color fontIdleColor;
		sf::Color fontHoverColor;
		sf::Color fontActiveColor;

		sf::Color idleColor;
		sf::Color hoverColor;
		sf::Color activeColor;

		sf::Color outlineIdleColor;
		sf::Color outlineHoverColor;
		sf::Color outlineActiveColor;

		short unsigned buttonState;
		short unsigned id;
	public:
		Button(sf::Vector2f position, sf::Vector2f dimensions, sf::Font* font, std::string text, int fontsize, 
			   sf::Color fontIdleColor, sf::Color fontHoverColor, sf::Color fontActiveColor, 
			   sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor, 
			   sf::Color outlineIdleColor = sf::Color::Transparent, sf::Color outlineHoverColor = sf::Color::Transparent, sf::Color outlineActiveColor = sf::Color::Transparent,
			   short unsigned id = 0);
		~Button();

		const bool isPressed() const;

		sf::Vector2f getDimesions();
		sf::FloatRect getGlobalBounds();
		sf::Vector2f getPosition();

		void setOrigin(sf::Vector2f origin);
		void setFillColor(sf::Color color);
		void setTexture(sf::Texture*);
		void setPosition(sf::Vector2f position);
		void setDimensions(sf::Vector2f dimensions);
		void setTextSettings(std::string text, sf::Font*, int fontsize, sf::Color fontColor);
		void setText(std::string text);
		void setTextPosition();
		void setId(short unsigned id);
		void setFontColors(sf::Color fontIdleColor, sf::Color fontHoverColor, sf::Color fontActiveColor);
		void setButtonColors(sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor);
		void setOutlineColors(sf::Color outlineIdleColor, sf::Color outlineHoverColor, sf::Color outlineActiveColor);

		const short unsigned& getID() const;
		const std::string getText() const;

		void update(const sf::Vector2i& mousePosWindow, const sf::Vector2f& mousePosView = {0,0}, bool worldButton = false);
		void render(sf::RenderTarget& target);
	};

	class DropDownMenu
	{
		private:
			sf::Font* font;
			gui::Button* activeElement;
			std::vector<gui::Button*> elements;

			float keyTime;
			float keyTimeMax;
			bool showList;
		public:
			DropDownMenu(float x, float y, float width, float height, sf::Font& font, std::string list[], unsigned numOfElements, unsigned defaultIndex);
			~DropDownMenu();

			bool getKeyTime();
			gui::Button* getActiveElement();

			void updateKeyTime(const float& dt);
			void update(const sf::Vector2i& mousePosWindow, const float& dt);
			void render(sf::RenderTarget& target);
	};

	class TextureSelector
	{
	private:
		sf::RectangleShape bounds;
		sf::RectangleShape selector;
		sf::Vector2u mousePosGrid;
		sf::IntRect textureRect;
		sf::Sprite sheet;

		bool getKeyTime();
		void updateKeyTime(const float& dt);

		float keyTime;
		float keyTimeMax;
		float gridSize;
		bool active;
		bool hidden;

	public:
		TextureSelector(float x, float y, float bX, float bY, float width, float height, float gridSize, const sf::Texture* textureSheet, sf::Font& font);
		~TextureSelector();

		Button* hideButton;
		bool& getActive();
		const sf::IntRect getTextureRect() const;
		bool& isHidden();

		void update(const sf::Vector2i& mousePosWindow, const float& dt);
		void render(sf::RenderTarget& target);
	};

	class HUD
	{
	private:
		sf::Text health;
		sf::Text strength;
		sf::Text defense;
		sf::Text money;

		std::map<std::string, sf::Texture> textures;

		sf::RectangleShape heart;
		sf::RectangleShape currency;
		sf::RectangleShape shield;
		sf::RectangleShape sword;

		void initTextures();
		void initGui(sf::Font* font);
	public:
		HUD(sf::Font* font);

		void update(PlayerStats stats);
		void render(sf::RenderTarget& target);
	};
}

