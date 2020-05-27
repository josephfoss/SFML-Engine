#include "stdafx.h"
#include "MainMenuState.h"

MainMenuState::MainMenuState(StateData* stateData) : State(stateData)
{
	initKeybinds();
	initButtons();
	initTextures();
	initAudio();
}


MainMenuState::~MainMenuState()
{
	std::cout << "- MainMenuState Destroyed.\n";

	//deleting button pointers.
	auto i = buttons.begin();
	for (i = buttons.begin(); i != buttons.end(); i++)
	{
		delete i->second;
	}

	//deleting texture pointers.
	auto j = textures.begin();
	for (j = textures.begin(); j != textures.end(); j++)
	{
		delete j->second;
	}

	delete ddm;
}

void MainMenuState::initVariables()
{

}

void MainMenuState::initTextures()
{
	//logo
	textures["MM_LOGO"] = new sf::Texture();
	textures["MM_LOGO"]->loadFromFile("Assets/Textures/UI/mm_logo.png");
	logo.setTexture(textures["MM_LOGO"]);
	logo.setSize(sf::Vector2f(500, 503));
	logo.setPosition(sf::Vector2f((window->getSize().x - logo.getGlobalBounds().width) / 2.f, 20));

	//background
	textures["MM_BACKGROUND"] = new sf::Texture();
	textures["MM_BACKGROUND"]->loadFromFile("Assets/Textures/UI/mm_bg.png");
	background.setTexture(textures["MM_BACKGROUND"]); 
	background.setSize(sf::Vector2f(1500, 900));

	//play & exit button
	textures["MM_BUTTONS"] = new sf::Texture();
	textures["MM_BUTTONS"]->loadFromFile("Assets/Textures/UI/mm_button.png");
	buttons["PLAY_BUTTON"]->setTexture(textures["MM_BUTTONS"]); 
	buttons["EXIT_BUTTON"]->setTexture(textures["MM_BUTTONS"]);

	//settings button
	textures["MM_SETTINGSBUTTON"] = new sf::Texture();
	textures["MM_SETTINGSBUTTON"]->loadFromFile("Assets/Textures/UI/mm_settingsbutton.png");
	buttons["SETTINGS_BUTTON"]->setTexture(textures["MM_SETTINGSBUTTON"]);

	//settings button
	textures["MM_EDITORBUTTON"] = new sf::Texture();
	textures["MM_EDITORBUTTON"]->loadFromFile("Assets/Textures/UI/mm_editorbutton.png");
	buttons["EDITOR_BUTTON"]->setTexture(textures["MM_EDITORBUTTON"]);
}

void MainMenuState::initKeybinds()
{

	std::ifstream fin("Properties/States/mainmenustate_keys.ini");

	if (fin.is_open())
	{
		std::string key = "";
		std::string keyVal = "";

		while (!fin.eof())
		{
			fin >> key >> keyVal;
			this->stateKeybinds[key] = this->keybinds->at(keyVal);
		}
	}


	fin.close();
}

void MainMenuState::updateInput(const float& dt)
{

}

void MainMenuState::initButtons()
{
	//creating Play button.
	buttons["PLAY_BUTTON"] =
		new gui::Button(
			//position              dimensions
			sf::Vector2f(0, 0), sf::Vector2f(350, 170),
			//font     text    size
			&fontMain, "Play", 75,
			//fontidle               fonthover                fontactive
			sf::Color(194, 10, 19), sf::Color(230, 10, 19), sf::Color(194, 10, 19),
			//idlecolor               hovercolor                activecolor
			sf::Color(200, 185, 185), sf::Color(220, 220, 220), sf::Color(194, 0, 0)
		);

	//centering Play button.
	buttons["PLAY_BUTTON"]->setPosition(sf::Vector2f(
		(window->getSize().x - buttons["PLAY_BUTTON"]->getGlobalBounds().width) / 2.f, 520));

	//creating Exit button
	buttons["EXIT_BUTTON"] =
		new gui::Button(
			//position              dimensions
			sf::Vector2f(0, 0), sf::Vector2f(350, 170),
			//font     text    size
			&fontMain, "Quit", 75,
			//fontidle               fonthover                fontactive
			sf::Color(194, 10, 19), sf::Color(230, 10, 19), sf::Color(194, 10, 19),
			//idlecolor               hovercolor                activecolor
			sf::Color(200, 185, 185), sf::Color(220, 220, 220), sf::Color(194, 0, 0)
		);
	//centering Exit button.
	buttons["EXIT_BUTTON"]->setPosition(sf::Vector2f(
		(window->getSize().x - buttons["EXIT_BUTTON"]->getGlobalBounds().width) / 2.f, 700));

	//creating Settings button
	buttons["SETTINGS_BUTTON"] =
		new gui::Button(
			//position              dimensions
			sf::Vector2f(0, 0), sf::Vector2f(94, 90),
			//font     text    size
			&fontMain, "", 75,
			//fontidle               fonthover                fontactive
			sf::Color(194, 10, 19), sf::Color(230, 10, 19), sf::Color(194, 10, 19),
			//idlecolor               hovercolor                activecolor
			sf::Color(200, 185, 185), sf::Color(220, 220, 220), sf::Color(194, 0, 0)
		);
	//centering Settings button.
	buttons["SETTINGS_BUTTON"]->setPosition(sf::Vector2f(
		1359, 780));

	//creating Settings button
	buttons["EDITOR_BUTTON"] =
		new gui::Button(
			//position              dimensions
			sf::Vector2f(0, 0), sf::Vector2f(94, 90),
			//font     text    size
			&fontMain, "", 75,
			//fontidle               fonthover                fontactive
			sf::Color(194, 10, 19), sf::Color(194, 10, 19), sf::Color(194, 10, 19),
			//idlecolor               hovercolor                activecolor
			sf::Color(200, 185, 185), sf::Color(220, 220, 220), sf::Color(194, 0, 0)
		);
	//centering Settings button.
	buttons["EDITOR_BUTTON"]->setPosition(sf::Vector2f(
		1359, 675));

	std::string li[] = {"First", "Second", "Third", "Fourth"};
	ddm = new gui::DropDownMenu(100, 100, 200, 50, fontMain, li, 4, 0);

}

void MainMenuState::initAudio()
{
	if (!ambiance.openFromFile("Assets/Audio/mm_amb.wav"))
	{
		throw("Could not load audio.");
	}

	ambiance.setLoop(true);
	ambiance.play();
}

void MainMenuState::renderButtons(sf::RenderTarget& target)
{
	for (auto &i : this->buttons)
	{
		i.second->render(target);
	}
}

void MainMenuState::updateButtons()
{
	for (auto &i : this->buttons)
	{
		i.second->update(mousePosWindow);
	}

	if (buttons["PLAY_BUTTON"]->isPressed())
	{
		ambiance.stop();
		this->states->push(new GameState(state_data));
	}

	if (buttons["EXIT_BUTTON"]->isPressed())
	{
		endState();
	}

	if (buttons["EDITOR_BUTTON"]->isPressed())
	{
		ambiance.stop();
		this->states->push(new EditorState(state_data));
	}
}

void MainMenuState::updateAudio()
{
	if (ambiance.getStatus() != sf::Music::Playing)
	{
		ambiance.setLoop(true);
		ambiance.play();
	}
}

void MainMenuState::update(const float& dt)
{
	sf::View view = window->getDefaultView();
	window->setView(view);

	updateAudio();
	updateMousePositions();
	updateInput(dt);
	updateButtons();

	ddm->update(mousePosWindow, dt);
}

void MainMenuState::render(sf::RenderTarget* target)
{
	if (!target)
		target = window;

	target->draw(background);
	target->draw(logo);
	ddm->render(*target);
	renderButtons(*target);
}