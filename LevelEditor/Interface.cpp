#include <iostream>

#include "Interface.h"
#include "LevelEditor.h"

Interface::Interface (int width, int height, LevelEditor *levelEditor)
	:
	m_levelEditor(levelEditor),
	m_mode(Mode::EDIT)
{
	m_buttons.push_back(new InterfaceButtons(0.0f, 0.0f, InterfaceButtons::ButtonAction::NEWLEVEL, this, levelEditor));
	m_buttons.push_back(new InterfaceButtons(160.0f, 0.0f, InterfaceButtons::ButtonAction::OPENLEVEL, this, levelEditor));
	m_buttons.push_back(new InterfaceButtons(330.0f, 0.0f, InterfaceButtons::ButtonAction::SAVELEVEL, this, levelEditor));
	m_buttons.push_back(new InterfaceButtons(1355.0f, 71.0f, InterfaceButtons::ButtonAction::COLLISION, this, levelEditor));

	m_menuLines[0].setSize(sf::Vector2f(1600.0f, 2.0f));
	m_menuLines[0].setPosition(0.0f, 40.0f);
	m_menuLines[0].setFillColor(sf::Color::Black);

	m_menuLines[1].setSize(sf::Vector2f(2.0f, 858.0f));
	m_menuLines[1].setPosition(1350.0f, 42.0f);
	m_menuLines[1].setFillColor(sf::Color::Black);

	m_tilesetBackground[0].setPosition(1352.0f, 42.0f);
	m_tilesetBackground[0].setSize(sf::Vector2f(248.0f, 858.0f));
	m_tilesetBackground[0].setFillColor(sf::Color(190.0f, 190.0f, 190.0f));

	m_tilesetBackground[1].setPosition(1354.95f, 110.6f);
	m_tilesetBackground[1].setSize(sf::Vector2f(241.5f, 781.0f));
	m_tilesetBackground[1].setFillColor(sf::Color(240.0f, 240.0f, 240.0f));

	m_emptyTileTexture.loadFromFile("Resources/Interface/BlankTile.png");
	
	int tileID = 0;

	float x = 1361.6f;
	
	for (int i = 0; i < 4; ++i)
	{
		float y = 119.2;

		for (int j = 0; j < 13; ++j)
		{
			m_tiles.push_back(new Tile(x, y, &m_emptyTileTexture, tileID++, this));

			y += 59.6;
		}

		x += 59.6f;
	}

	m_currentTile.setTexture(&m_emptyTileTexture);
	m_currentTile.setSize(sf::Vector2f(65.0f, 65.0f));
	m_currentTile.setPosition(1505.0f, 46.0f);

	m_buttons[buttonID(ButtonID::SAVELEVEL)]->DisableButton();
}

Interface::~Interface()
{
	for (auto iterator = m_buttons.begin(); iterator != m_buttons.end(); ++iterator)
	{
		delete *iterator;
	}
}

void Interface::LoadTileset(const std::string &tileset, float tileWidth, float tileHeight, int tileGap)
{
	m_tileSize.x = tileWidth;
	m_tileSize.y = tileHeight;
	m_tileGap = tileGap;

	m_tilesetTexture.loadFromFile(tileset);
	
	int tilesetWidth = m_tilesetTexture.getSize().x;
	int tilesetHeight = m_tilesetTexture.getSize().y;

	for (int i = 0; i < 52; ++i)
	{
		m_tiles[i]->SetTilesetTexture(&m_tilesetTexture);
	}

	SetTileset();
}

void Interface::ProcessMouse(float x, float y)
{
	sf::Vector2f position(x, y);

	position = getTransform().getInverse().transformPoint(position);

	for (auto iterator : m_buttons)
	{
		iterator->ProcessMouse(position.x, position.y);
	}
}

void Interface::ProcessMouseClicked(float x, float y, int buttonID)
{
	sf::Vector2f position(x, y);

	position = getTransform().getInverse().transformPoint(position);

	for (auto iterator : m_buttons)
	{
		if (iterator->IsEnabled())
		{
			iterator->ProcessMouseClick(position.x, position.y, buttonID);
		}
	}

	if (m_mode == EDIT)
	{
		for (auto iterator : m_tiles)
		{
			iterator->ProcessMouseClick(position.x, position.y, buttonID);
		}
	}
	else if (m_mode == COLLSION)
	{
	}
}

void Interface::ChangeMode(Mode mode)
{
	if (m_mode != mode)
	{
		m_mode = mode;

		if (mode == EDIT)
		{
			m_levelEditor->ChangeMode(Mode::EDIT);

			m_buttons[buttonID(ButtonID::COLLISION)]->EnableButton();

			m_tilesetBackground[1].setFillColor(sf::Color(240.0f, 240.0f, 240.0f));
		}
		else if (mode == COLLSION)
		{
			m_levelEditor->ChangeMode(Mode::COLLSION);

			m_buttons[buttonID(ButtonID::COLLISION)]->DisableButton();

			m_tilesetBackground[1].setFillColor(sf::Color(240.0f, 0.0f, 00.0f));
		}
	}
}

void Interface::ChangeCurrentTile(int tileValue, sf::IntRect textureRect)
{

	m_levelEditor->ChangeTileValue(tileValue);

	m_currentTile.setTexture(&m_tilesetTexture);
	m_currentTile.setTextureRect(textureRect);

}

void Interface::ResetButtons()
{
	for (auto iterator : m_buttons)
	{
		iterator->ResetButtons();
	}
}

void Interface::LevelOpened()
{
	m_buttons[buttonID(ButtonID::SAVELEVEL)]->EnableButton();
}

Interface::Mode Interface::ReturnMode()
{
	return m_mode;
}

void Interface::draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates) const
{
	renderStates.transform *= getTransform();

	renderTarget.draw(m_tilesetBackground[0], renderStates);
	renderTarget.draw(m_tilesetBackground[1], renderStates);

	for (int i = 0; i < 2; ++i)
	{
		renderTarget.draw(m_menuLines[i], renderStates); 
	}

	for (auto iterator : m_buttons)
	{
		renderTarget.draw(*iterator, renderStates);
	}

	for (auto button : m_tiles)
	{
		renderTarget.draw(*button, renderStates);
	}

	renderTarget.draw(m_currentTile, renderStates);
}

void Interface::SetTileset()
{
	int numberOfTiles = (m_tilesetTexture.getSize().x / m_tileSize.x) * (m_tilesetTexture.getSize().y / m_tileSize.y);

	for(int i = 0; i < 52; ++i)
	{
		if (i >= numberOfTiles)
		{
			m_tiles[i]->SetBlankTileTexture();
		}
		else
		{
			int x = (i % (m_tilesetTexture.getSize().x / m_tileSize.x)) * (m_tileSize.x + m_tileGap);
			int y = (i / (m_tilesetTexture.getSize().x / m_tileSize.x)) * (m_tileSize.y + m_tileGap);

			m_tiles[i]->SetTileRect(sf::IntRect(x, y, m_tileSize.x, m_tileSize.y));
			m_tiles[i]->SetTileTexture();
		}
	}
}

int Interface::buttonID(ButtonID buttonID)
{
	return static_cast<std::underlying_type_t<ButtonID>>(buttonID);
}