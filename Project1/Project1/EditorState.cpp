#include "stdafx.h"
#include "EditorState.h"

EditorState::EditorState(StateData* stateData) : State(stateData)
{
	initCamera();
	initVariables();
	initTextures();
	initKeybinds();
	initText();
	initTileMap();
	initGui();
}

EditorState::~EditorState()
{
	std::cout << "- EditorState Destroyed.\n";
	delete pausemenu;
	delete tileMap;
	delete textureSelector;
	delete ddmenu;
}

std::string EditorState::getPropModeFromId(int id)
{
	switch (id)
	{
		case 0:
			return " default";
			break;
		case 1:
			return " shopkeeper";
			break;
		case 2:
			return " load dungeon";
			break;
		case 3:
			return " load shop";
			break;
		case 4:
			return " player spawn";
			break;
		case 5:
			return " enemy spawn";
			break;
		case 6:
			return " boss spawn";
			break;
		default:
			return " undefined";
			break;
	}
}

void EditorState::initCamera()
{
	camera.setSize(sf::Vector2f(1500, 900));
	camera.setCenter(1500.f / 2.f, 900.f / 2.f);
}

void EditorState::initVariables()
{
	textureRect = sf::IntRect(0, 0, static_cast<int>(state_data->gridSize), static_cast<int>(state_data->gridSize));
	propTextureRect = sf::IntRect(0, 0, static_cast<int>(state_data->gridSize), static_cast<int>(state_data->gridSize));
	clickable = false;
	breakable = false;
	collision = false;
	id = 0;
	type = TileType::DEFAULT;
	zoomlevel = 1.f;
	cameraSpeed = 250.f;
	currentLayer = 0.f;
}

void EditorState::initTextures()
{
	textures["BUTTON"].loadFromFile("Assets/Textures/UI/mm_button.png");
}

void EditorState::initKeybinds()
{
	std::ifstream fin("Properties/States/editorstate_keys.ini");

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

void EditorState::initText()
{
	cursorText.setFont(fontSecondary);
	cursorText.setCharacterSize(9);
}

void EditorState::initGui()
{
	pausemenu = new PauseMenu(*window, fontMain, textures);
	textureSelector = new gui::TextureSelector(20, 20, 20, 20, 840, 720, 120, tileMap->getTileSheet(), fontMain);
	propSelector = new gui::TextureSelector(20, 20, 60, 20, 480, 480, 120, propMap->getPropSheet(), fontMain);

	propSelector->isHidden() = true;

	//Button IDs:
	//						0         1            2           3            4
	std::string li[] = { "Shop", "Dungeon 1", "Dungeon 2", "Dungeon 3", "Dungeon 4"};
	ddmenu = new gui::DropDownMenu(650, 500, 200, 50, fontMain, li, 5, 0);

	buttons["SAVE"] =
		new gui::Button(
			//position              dimensions
			sf::Vector2f(40, 800), sf::Vector2f(100, 30),
			//font     text    size
			&fontMain, "SAVE", 75,
			//fontidle               fonthover                fontactive
			sf::Color(194, 10, 19), sf::Color(230, 10, 19), sf::Color(194, 10, 19),
			//idlecolor               hovercolor                activecolor
			sf::Color(200, 185, 185), sf::Color(220, 220, 220), sf::Color(194, 0, 0)
		);

	buttons["LOAD"] =
		new gui::Button(
			//position              dimensions
			sf::Vector2f(40, 735), sf::Vector2f(100, 30),
			//font     text    size
			&fontMain, "LOAD", 75,
			//fontidle               fonthover                fontactive
			sf::Color(194, 10, 19), sf::Color(230, 10, 19), sf::Color(194, 10, 19),
			//idlecolor               hovercolor                activecolor
			sf::Color(200, 185, 185), sf::Color(220, 220, 220), sf::Color(194, 0, 0)
		);

	selectorRect.setSize(sf::Vector2f(state_data->gridSize, state_data->gridSize));
	selectorRect.setFillColor(sf::Color(255, 255, 255, 150));
	selectorRect.setTexture(tileMap->getTileSheet());
	selectorRect.setTextureRect(textureRect);
	selectorRect.setOutlineThickness(1.f);
	selectorRect.setOutlineColor(sf::Color::Green);
}

void EditorState::initTileMap()
{
	tileMap = new TileMap(state_data->gridSize, 50, 50, 5, "Assets/Textures/Tiles/dungeonTileSheet.png");
	propMap = new PropMap(6000, 6000, "Assets/Textures/Tiles/dungeonPropSheet.png");
}

void EditorState::updateButtons()
{

}

void EditorState::updatePauseMenuButtons(const float& dt)
{
	buttons["SAVE"]->update(mousePosWindow);
	buttons["LOAD"]->update(mousePosWindow);

	ddmenu->update(mousePosWindow, dt);

	if (buttons["SAVE"]->isPressed() && getKeyTime())
	{
		switch (ddmenu->getActiveElement()->getID())
		{
			case 1:
				std::cout << "Save Case 1" << std::endl;
				tileMap->saveToFile("Data/tilemaps/shop.tm");
				propMap->saveToFile("Data/tilemaps/shop.pm");
				break;
			case 2:
				std::cout << "Save Case 2" << std::endl;
				tileMap->saveToFile("Data/tilemaps/dungeon1.tm");
				propMap->saveToFile("Data/tilemaps/dungeon1.pm");
				break;
			case 3:
				std::cout << "Save Case 3" << std::endl;
				tileMap->saveToFile("Data/tilemaps/dungeon2.tm");
				propMap->saveToFile("Data/tilemaps/dungeon2.pm");
				break;
			case 4:
				std::cout << "Save Case 4" << std::endl;
				tileMap->saveToFile("Data/tilemaps/dungeon3.tm");
				propMap->saveToFile("Data/tilemaps/dungeon3.pm");
				break;
			case 5:
				std::cout << "Save Case 5" << std::endl;
				tileMap->saveToFile("Data/tilemaps/dungeon4.tm");
				propMap->saveToFile("Data/tilemaps/dungeon4.pm");
				break;
			default:
				break;
		}
	}


	if (buttons["LOAD"]->isPressed() && getKeyTime())
	{
		switch (ddmenu->getActiveElement()->getID())
		{
			case 1:
				std::cout << "Load Case 1" << std::endl;
				tileMap->loadFromFile("Data/tilemaps/shop.tm");
				propMap->loadFromFile("Data/tilemaps/shop.pm", &fontMain);
				break;
			case 2:
				std::cout << "Load Case 2" << std::endl;
				tileMap->loadFromFile("Data/tilemaps/dungeon1.tm");
				propMap->loadFromFile("Data/tilemaps/dungeon1.pm", &fontMain);
				break;
			case 3:
				std::cout << "Load Case 3" << std::endl;
				tileMap->loadFromFile("Data/tilemaps/dungeon2.tm");
				propMap->loadFromFile("Data/tilemaps/dungeon2.pm", &fontMain);
				break;
			case 4:
				std::cout << "Load Case 4" << std::endl;
				tileMap->loadFromFile("Data/tilemaps/dungeon3.tm");
				propMap->loadFromFile("Data/tilemaps/dungeon3.pm", &fontMain);
				break;
			case 5:
				std::cout << "Load Case 5" << std::endl;
				tileMap->loadFromFile("Data/tilemaps/dungeon4.tm");
				propMap->loadFromFile("Data/tilemaps/dungeon4.pm", &fontMain);
				break;
			default:
				break;
		}
	}

	if (pausemenu->getButtons()["EXIT"]->isPressed())
	{
		endState();
	}
}

void EditorState::updateInput(const float& dt)
{
	if (sf::Keyboard::isKeyPressed(stateKeybinds.at("CLOSE")) && getKeyTime())
	{
		if (!paused)
		{
			pauseState();
		}
		else
		{
			unpauseState();
		}
	}
}

void EditorState::updateEditorInput(const float& dt)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
	{
		if ((!textureSelector->hideButton->getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow))) && (!textureSelector->getActive()) && (!propSelector->hideButton->getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow))) && (!propSelector->getActive()))
		{
			if (editorMode == EditorMode::TILES)
			{
				tileMap->addTile(mousePosGrid.x, mousePosGrid.y, currentLayer, textureRect, collision, type);
			}
			else
			{
				if (getKeyTime())
				{
					propMap->addProp(mousePosView.x, mousePosView.y, propTextureRect, clickable, id, &fontMain, breakable);
				}
			}
		}
		else if(textureSelector->getActive())
		{
			textureRect = textureSelector->getTextureRect();
		}
		else if (propSelector->getActive())
		{
			if (selectorRect.getTexture() != propMap->getPropSheet())
			{
				selectorRect.setTexture(propMap->getPropSheet());
			}
			propTextureRect = propSelector->getTextureRect();
		}
	}
	else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
	{
		if ((!textureSelector->hideButton->getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow))) && (!textureSelector->getActive()) && (!propSelector->hideButton->getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow))) && (!propSelector->getActive()))
		{
			(editorMode == EditorMode::TILES ? tileMap->removeTile(mousePosGrid.x, mousePosGrid.y, currentLayer) : propMap->removeProp(mousePosView.x, mousePosView.y));
		}
	}
	if (sf::Keyboard::isKeyPressed(stateKeybinds.at("MOVE_UP")))
	{
		camera.move(0.f, -(cameraSpeed * dt));
	}
	if (sf::Keyboard::isKeyPressed(stateKeybinds.at("MOVE_DOWN")))
	{
		camera.move(0.f, (cameraSpeed * dt));
	}
	if (sf::Keyboard::isKeyPressed(stateKeybinds.at("MOVE_LEFT")))
	{
		camera.move(-(cameraSpeed * dt), 0.f);
	}
	if (sf::Keyboard::isKeyPressed(stateKeybinds.at("MOVE_RIGHT")))
	{
		camera.move((cameraSpeed * dt), 0.f);
	}
	if (sf::Keyboard::isKeyPressed(stateKeybinds.at("ZOOM_OUT")) && getKeyTime())
	{
		if (zoomlevel < 4)
		{
			zoomlevel += .5;
		}

		cursorText.setScale(zoomlevel, zoomlevel);
		camera.setSize(1500 * zoomlevel, 900 * zoomlevel);
	}
	if (sf::Keyboard::isKeyPressed(stateKeybinds.at("ZOOM_IN")) && getKeyTime())
	{
		if (zoomlevel > .5)
		{
			zoomlevel -= .5;
		}

		cursorText.setScale(zoomlevel, zoomlevel);
		camera.setSize(1500 * zoomlevel, 900 * zoomlevel);
	}
	if (sf::Keyboard::isKeyPressed(stateKeybinds.at("CYCLE_LAYER")) && getKeyTime())
	{
		if (currentLayer < tileMap->getNumLayers() - 1)
		{
			currentLayer++;
		}
		else
		{
			currentLayer = 0;
		}
	}
	if (sf::Keyboard::isKeyPressed(stateKeybinds.at("CYCLE_TYPE")) && getKeyTime())
	{
		if (editorMode == EditorMode::TILES)
		{
			if (type < TileType::numOfElements - 1)
			{
				type++;
			}
			else if (type == TileType::numOfElements - 1)
			{
				type = 0;
			}
		}
		else
		{
			breakable = !breakable;
		}

	}
	if (sf::Keyboard::isKeyPressed(stateKeybinds.at("TOGGLE_COLLISION")) && getKeyTime())
	{
		if (editorMode == EditorMode::TILES)
		{
			collision = !collision;
		}
		else
		{
			clickable = !clickable;
		}
	}
	if (sf::Keyboard::isKeyPressed(stateKeybinds.at("INC_ID")) && getKeyTime())
	{
		id++;
		if (id > 0)
			clickable = true;
		else
			clickable = false;
	}
	if (sf::Keyboard::isKeyPressed(stateKeybinds.at("DEC_ID")) && getKeyTime())
	{
		id--;
		if (id > 0)
			clickable = true;
		else
			clickable = false;
	}
}

void EditorState::updateGui(const float& dt)
{
	textureSelector->update(mousePosWindow, dt);
	propSelector->update(mousePosWindow, dt);

	if (textureSelector->hideButton->isPressed() && getKeyTime())
	{
		if (editorMode == EditorMode::PROPS)
		{
			if (selectorRect.getTexture() != tileMap->getTileSheet())
			{
				selectorRect.setTexture(tileMap->getTileSheet());
				selectorRect.setPosition(mousePosGrid.x * state_data->gridSize, mousePosGrid.y * state_data->gridSize);
				selectorRect.setTextureRect(textureRect);
			}

			editorMode = EditorMode::TILES;
			propSelector->isHidden() = true;
		}

		textureSelector->isHidden() = !textureSelector->isHidden();
	}

	if (propSelector->hideButton->isPressed() && getKeyTime())
	{
		if (editorMode == EditorMode::TILES)
		{
			if (selectorRect.getTexture() != propMap->getPropSheet())
			{
				selectorRect.setTexture(propMap->getPropSheet());
				selectorRect.setPosition(mousePosView.x, mousePosView.y);
				selectorRect.setTextureRect(propTextureRect);

			}

			editorMode = EditorMode::PROPS;
			textureSelector->isHidden() = true;
		}

		propSelector->isHidden() = !propSelector->isHidden();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && getKeyTime())
	{
		if (editorMode == EditorMode::TILES)
		{
			textureSelector->isHidden() = !textureSelector->isHidden();
			textureSelector->getActive() = false;
			selectorRect.setPosition(mousePosGrid.x * state_data->gridSize, mousePosGrid.y * state_data->gridSize);
			selectorRect.setTextureRect(textureRect);
		}
		if (editorMode == EditorMode::PROPS)
		{
			propSelector->isHidden() = !propSelector->isHidden();
			propSelector->getActive() = false;
			selectorRect.setPosition(mousePosView.x, mousePosView.y);
			selectorRect.setTextureRect(propTextureRect);
		}
	}

	if ((!textureSelector->hideButton->getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow))) && (!textureSelector->getActive()))
	{
		if (editorMode == EditorMode::TILES)
		{
			selectorRect.setPosition(mousePosGrid.x * state_data->gridSize, mousePosGrid.y * state_data->gridSize);
			selectorRect.setTextureRect(textureRect);
		}
		if (editorMode == EditorMode::PROPS)
		{
			selectorRect.setPosition(mousePosView.x, mousePosView.y);
			selectorRect.setTextureRect(propTextureRect);
		}
	}

	if (editorMode == EditorMode::TILES)
	{
		std::stringstream ss;
		cursorText.setPosition(mousePosView.x + 50, mousePosView.y);
		ss << "Tile Mode: \n(" << mousePosView.x << ", " << mousePosView.y << ")\n" << "(" << mousePosGrid.x << ", " << mousePosGrid.y << ")\n" << textureRect.left << " " << textureRect.top << "\nColl: " << (collision == false ? "False" : "True") << " \nType: " << Tile::getTileType(type) << "\nLayer: " << currentLayer << "\n";
		cursorText.setString(ss.str());
	}
	else
	{
		std::stringstream ss;
		cursorText.setPosition(mousePosView.x + 50, mousePosView.y);
		ss << "Prop Mode: \n(" << mousePosView.x << ", " << mousePosView.y << ")\n" << textureRect.left << " " << textureRect.top << "\nClickable: " << (clickable == false ? "False" : "True") << " \nBreakable: " << (breakable == false ? "False\nID:" : "True\nID:") << getPropModeFromId(id);
		cursorText.setString(ss.str());
	}

}

void EditorState::update(const float& dt)
{
	updateMousePositions(&camera);
	updateKeyTime(dt);
	updateInput(dt);

	if (!paused) //unpaused updates
	{
		updateGui(dt);
		updateEditorInput(dt);
		updateButtons();
	}
	else //pause menu
	{
		pausemenu->update(mousePosWindow);
		updatePauseMenuButtons(dt);
	}
}

void EditorState::renderGui(sf::RenderTarget& target)
{
	if ((!textureSelector->hideButton->getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow))) && (!textureSelector->getActive()) && (!propSelector->hideButton->getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePosWindow))) && (!propSelector->getActive()))
	{
		//selector rectangle drawn through camera view
		target.setView(camera);
		target.draw(selectorRect);
	}

	//texturselector rendered with default view
	target.setView(window->getDefaultView());
	textureSelector->render(target);
	propSelector->render(target);

	//cursortext drawn through camera view
	target.setView(camera);
	target.draw(cursorText);

	if (paused) // renders pause menu
	{
		//pausemenu rendered through default view
		target.setView(window->getDefaultView());
		pausemenu->render(target);
		ddmenu->render(target);
		buttons["SAVE"]->render(target);
		buttons["LOAD"]->render(target);
	}
}

void EditorState::render(sf::RenderTarget* target)
{
	if (!target)
		target = window;

	target->setView(camera);

	tileMap->render(*target, 0);
	tileMap->render(*target, 1);
	tileMap->render(*target, 2);
	propMap->render(*target, true);
	tileMap->render(*target, 3);
	tileMap->render(*target, 4);

	target->setView(window->getDefaultView());
	renderGui(*target);
}

