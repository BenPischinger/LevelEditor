#include <iostream>
#include <fstream>

#include "LevelEditor.h"

int main()
{
	LevelEditor levelEditor(1600, 900);
	levelEditor.Initialise();

	return 0;
}

LevelEditor::LevelEditor(unsigned int width, unsigned int height)
	:
	m_window(sf::VideoMode(width, height), "2D Level Editor"),

	m_interface(width, height, this),

	m_mode(Interface::Mode::EDIT),

	m_currentTileValue(-1),

	m_showTileSelector(false),
	m_mouseIsDragged(false)
{
	m_window.setFramerateLimit(144);

	m_view.setSize(1350, 900);
	m_view.setViewport(sf::FloatRect(0.0f, 0.045f, 0.845f, 1.0f));

	m_interfaceView = m_window.getView();

	m_tileSelector.setFillColor(sf::Color(0, 255, 0, 127));
}

LevelEditor::~LevelEditor()
{

}

void LevelEditor::Initialise()
{
	sf::Clock clock;

	while(m_window.isOpen())
	{
		ProcessEvents();
		Render();
	}
}

void LevelEditor::NewLevel()
{
	std::string tileset;
	int *tiles;
	
	int tileWidth;
	int tileHeight;
	int tileGap;

	int levelWidth;
	int levelHeight;

	bool *isCollisionTile;

	std::cout << "New Level \nIncluded tileset is named: \"tileset1.png\" \nName of the tileset file (including file extension): ";

	std::cin >> tileset;

	while ((std::cout << "Included tileset width: 128 \nTile width in pixels: ") && !(std::cin >> tileWidth))
	{
		std::cout << "Invalid width input. \nPlease reenter." << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	while ((std::cout << "Included tileset height: 128 \nTile height in pixels:  ") && !(std::cin >> tileHeight))
	{
		std::cout << "Invalid height input. \nPlease reenter." << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	while ((std::cout << "Included tileset gap: 5 \nGap between tiles in pixels: ") && !(std::cin >> tileGap))
	{
		std::cout << "Invalid gap input. \nPlease reenter." << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	while ((std::cout << "Recommended level widht: 10 - 20 \nLevel width in tiles: ") && !(std::cin >> levelWidth))
	{
		std::cout << "Invalid Level width input. \nPlease reenter." << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	while ((std::cout << "Recommended level height: 10 - 20 \nLevel height in tiles: ") && !(std::cin >> levelHeight))
	{
		std::cout << "Invalid Level height input. \nPlease reenter." << std::endl;
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	tiles = new int[levelWidth * levelHeight];

	for (int i = 0; i < levelWidth * levelHeight; ++i)
	{
		tiles[i] = 0;
	}

	isCollisionTile = new bool[levelWidth * levelHeight];

	for (int i = 0; i < levelWidth * levelHeight; ++i)
	{
		isCollisionTile[i] = false;
	}

	if (m_level.LoadLevel("Resources/Game/Tilesets/" + tileset, sf::Vector2u(tileWidth, tileHeight), tiles, levelWidth, levelHeight, tileGap, isCollisionTile))
	{
		std::cout << "Level created successfully." << std::endl;

		m_tileSelector.setSize(sf::Vector2f(tileWidth, tileHeight));

		m_tileSize.x = tileWidth;
		m_tileSize.y = tileHeight;

		m_levelSize.x = levelWidth;
		m_levelSize.y = levelHeight;

		m_interface.LevelOpened();

		m_interface.LoadTileset("Resources/Game/Tilesets/" + tileset, tileWidth, tileHeight, tileGap);
	}
	else
	{
		std::cout << "Level could not be created." << std::endl;
	}

	delete[] tiles;
	delete[] isCollisionTile;
}

void LevelEditor::OpenLevel()
{
	std::string levelName;

	std::cout << "Load existing level. \nIncluded example levels are: \nexample1 \nexample2 \nexample3 \nName of the level file: ";
	std::cin >> levelName;

	levelName.append(".lvl");

	std::ifstream levelFile(std::string("Resources/Game/Levels/") + levelName, std::ios::binary);

	if (levelFile.is_open())
	{
		m_level.ReadFromFile(&levelFile);

		std::cout << levelName << " loaded succesfully." << std::endl;

		m_tileSize.x = m_level.ReturnTileSize().x;
		m_tileSize.y = m_level.ReturnTileSize().y;

		m_levelSize.x = m_level.ReturnLevelSize().x;
		m_levelSize.y = m_level.ReturnLevelSize().y;

		m_tileSelector.setSize(sf::Vector2f(m_tileSize.x, m_tileSize.y));

		m_interface.LoadTileset(m_level.ReturnTexturePath(), m_tileSize.x, m_tileSize.y, m_level.ReturnTileGap());

		m_interface.LevelOpened();
	}
	else
	{
		std::cerr << levelName << " could not be loaded." << std::endl;
	}
}

void LevelEditor::SaveLevel()
{

	std::string levelName;

	std::cout << "Save level. \nEnter level name: ";
	std::cin >> levelName;

	levelName.append(".lvl");

	std::ofstream levelFile(std::string("Resources/Game/Levels/") + levelName, std::ios::binary);

	if (levelFile.is_open())
	{
		m_level.WriteToFile(&levelFile);
		std::cout << levelName << " saved successfully" << std::endl;
	}
	else
	{
		std::cerr << levelName << " could not be saved" << std::endl;
	}
}

void LevelEditor::ChangeMode(Interface::Mode mode)
{
	if (m_mode != mode)
	{
		m_mode = mode;

		if (mode == Interface::Mode::EDIT)
		{
			m_level.DisableCollisionTiles();
			m_tileSelector.setFillColor(sf::Color(0, 255, 0, 127));
		}
		else if (mode == Interface::Mode::COLLSION)
		{
			m_level.EnableCollisionTiles();
			m_tileSelector.setFillColor(sf::Color(255, 0, 0, 127));
		}
	}
}

void LevelEditor::ChangeTileValue(int tileValue)
{
	m_currentTileValue = tileValue;
}

void LevelEditor::Render()
{
	m_window.clear(sf::Color(240, 240, 240, 255));

	m_window.setView(m_view);
	m_window.draw(m_level);

	if (m_showTileSelector)
	{
		m_window.draw(m_tileSelector);
	}

	m_window.setView(m_interfaceView);

	m_window.draw(m_interface);

	m_window.display();
}

void LevelEditor::ProcessEvents()
{
	sf::Event event;

	while (m_window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			m_window.close();
			break;

		case sf::Event::MouseMoved:
			ProcessMouse(event);
			break;

		case sf::Event::MouseButtonPressed:
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				m_leftMouseButtonPressed = true;

				ProcessMouseButtonPressed(event);
			}
			else if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
			{
				m_leftMouseButtonPressed = false;

				ProcessMouseButtonPressed(event);
			}
			break;

		case sf::Event::MouseButtonReleased:
			ProcessMouseButtonReleased(event);
			break;

		case sf::Event::MouseWheelScrolled:
			ProcessMouseWheelScrolled(event);
			break;

		case sf::Event::KeyPressed:
			ProcessKeyboard(event);
			break;
		}
	}
}

void LevelEditor::ProcessMouse(sf::Event& event)
{

	if (IsAboveInterfaceView(event.mouseMove.x, event.mouseMove.y))
	{
		sf::Vector2f position = m_window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), m_interfaceView);

		m_interface.ProcessMouse(position.x, position.y);

		m_showTileSelector = false;
	}

	else
	{
		m_interface.ResetButtons();

		if (IsAboveLevelView(event.mouseMove.x, event.mouseMove.y))
		{
			if (m_mouseIsDragged)
			{
				sf::Vector2f position = m_window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), m_view);

				sf::FloatRect rectangle;

				rectangle.left = std::min(m_mouseDragStart.x, position.x);
				rectangle.top = std::min(m_mouseDragStart.y, position.y);
				rectangle.width = std::abs(m_mouseDragStart.x - position.x);
				rectangle.height = std::abs(m_mouseDragStart.y - position.y);

				if (rectangle.left < 0)
				{
					rectangle.width = rectangle.left + rectangle.width;
					rectangle.left = 0;
				}
				else if ((rectangle.left + rectangle.width) > (m_levelSize.x * m_tileSize.x))
				{
					rectangle.width = (m_levelSize.x * m_tileSize.x) - rectangle.left - 1;
				}

				if (rectangle.top < 0)
				{
					rectangle.height = rectangle.top + rectangle.height;
					rectangle.top = 0;
				}
				else if ((rectangle.top + rectangle.height) > (m_levelSize.y * m_tileSize.y))
				{
					rectangle.height = (m_levelSize.y * m_tileSize.y) - rectangle.top - 1;
				}

				int levelX1;
				int levelY1;

				int levelX2;
				int levelY2;

				levelX1 = rectangle.left / m_tileSize.x;
				levelY1 = rectangle.top / m_tileSize.y;

				levelX2 = (rectangle.left + rectangle.width) / m_tileSize.x;
				levelY2 = (rectangle.top + rectangle.height) / m_tileSize.y;

				m_tileSelector.setPosition(levelX1 * m_tileSize.x, levelY1 * m_tileSize.x);
				m_tileSelector.setSize(sf::Vector2f((1 + levelX2 - levelX1) * m_tileSize.x, (1 + levelY2 - levelY1) * m_tileSize.y));

				m_showTileSelector = true;
			}
			else 
			{
				sf::Vector2f position = m_window.mapPixelToCoords(sf::Vector2i(event.mouseMove.x, event.mouseMove.y), m_view);

				if (IsAboveLevel(position.x, position.y))
				{
					int levelX;
					int levelY;

					levelX = position.x / m_tileSize.x;
					levelY = position.y / m_tileSize.y;

					if (levelX < 0 || levelX >= m_levelSize.x || levelY < 0 || levelY >= m_levelSize.y)
					{
						return;
					}

					m_showTileSelector = true;

					m_tileSelector.setPosition(levelX * m_tileSize.x, levelY * m_tileSize.y);
					m_tileSelector.setSize(sf::Vector2f(m_tileSize.x, m_tileSize.y));
				}
				else
					m_showTileSelector = false;
			}
		}
	}
}

void LevelEditor::ProcessMouseButtonPressed(sf::Event& event)
{
	m_mouseIsDragged = false;

	if (IsAboveLevelView(event.mouseButton.x, event.mouseButton.y))
	{
		
		sf::Vector2f position = m_window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), m_view);

		if (!IsAboveLevel(position.x, position.y))
		{
			return;
		}

		m_mouseDragStart.x = position.x;
		m_mouseDragStart.y = position.y;

		m_mouseIsDragged = true;

	}
	
	else if (IsAboveInterfaceView(event.mouseButton.x, event.mouseButton.y))
	{
		sf::Vector2f position = m_window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), m_interfaceView);

		m_interface.ProcessMouseClicked(position.x, position.y, event.mouseButton.button);
	}
}

void LevelEditor::ProcessMouseButtonReleased(sf::Event& event)
{
	if (IsAboveLevelView(event.mouseButton.x, event.mouseButton.y) && m_mouseIsDragged)
	{
		sf::Vector2f position = m_window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y), m_view);

		if (IsAboveLevel(position.x, position.y))
		{
			m_mouseDragEnd.x = position.x;
			m_mouseDragEnd.y = position.y;

			if (m_mouseDragStart == m_mouseDragEnd)
			{
				if (m_mode == Interface::Mode::EDIT && m_currentTileValue >= 0)
				{
					if (m_leftMouseButtonPressed)
					{
						m_level.PlaceTileAtMousePosition(position.x, position.y, m_currentTileValue);
					}
					else
					{
						m_level.PlaceTileAtMousePosition(position.x, position.y, 0);
					}
				}

				else if (m_mode == Interface::Mode::COLLSION)
				{
					if (m_leftMouseButtonPressed)
					{
						m_level.PlaceCollisionTileAtMousePosition(position.x, position.y, true);
					}

					else 
					{
						m_level.PlaceCollisionTileAtMousePosition(position.x, position.y, false);
					}
				}
			}
			else
			{
				sf::FloatRect rectangle;

				rectangle.left = std::min(m_mouseDragStart.x, m_mouseDragEnd.x);
				rectangle.top = std::min(m_mouseDragStart.y, m_mouseDragEnd.y);
				rectangle.width = std::abs(m_mouseDragStart.x - m_mouseDragEnd.x);
				rectangle.height = std::abs(m_mouseDragStart.y - m_mouseDragEnd.y);

				if (m_mode == Interface::Mode::EDIT && m_currentTileValue >= 0)
				{
					if (m_leftMouseButtonPressed)
					{
						m_level.PlaceTilesInArea(rectangle, m_currentTileValue);
					}
					else
					{
						m_level.PlaceTilesInArea(rectangle, 0);
					}
				}

				else if (m_mode == Interface::Mode::COLLSION)
				{
					if (m_leftMouseButtonPressed)
					{
						m_level.PlaceCollisionTilesInArea(rectangle, true);
					}
					else 
					{

						m_level.PlaceCollisionTilesInArea(rectangle, false);
					}
				}
			}

			int levelX;
			int levelY;

			levelX = position.x / m_tileSize.x;
			levelY = position.y / m_tileSize.y;

			if (levelX >= m_levelSize.x || levelY >= m_levelSize.y)
			{
				return;
			}

			m_tileSelector.setPosition(levelX * m_tileSize.x, levelY * m_tileSize.y);
			m_tileSelector.setSize(sf::Vector2f(m_tileSize.x, m_tileSize.y));
		}
		else
		{
			m_showTileSelector = false;

			m_mouseDragEnd.x = position.x;
			m_mouseDragEnd.y = position.y;

			if (m_mouseDragStart != m_mouseDragEnd)
			{
				sf::FloatRect rectangle;

				rectangle.left = std::min(m_mouseDragStart.x, m_mouseDragEnd.x);
				rectangle.top = std::min(m_mouseDragStart.y, m_mouseDragEnd.y);
				rectangle.width = std::abs(m_mouseDragStart.x - m_mouseDragEnd.x);
				rectangle.height = std::abs(m_mouseDragStart.y - m_mouseDragEnd.y);

				if (rectangle.left < 0)
				{
					rectangle.width = rectangle.left + rectangle.width;
					rectangle.left = 0;
				}
				else if ((rectangle.left + rectangle.width) > (m_levelSize.x * m_tileSize.x))
				{
					rectangle.width = (m_levelSize.x * m_tileSize.x) - rectangle.left - 1;
				}

				if (rectangle.top < 0)
				{
					rectangle.height = rectangle.top + rectangle.height;
					rectangle.top = 0;
				}
				else if ((rectangle.top + rectangle.height) > (m_levelSize.y * m_tileSize.y))
				{
					rectangle.height = (m_levelSize.y * m_tileSize.y) - rectangle.top - 1;
				}

				if (m_mode == Interface::Mode::EDIT && m_currentTileValue >= 0)
				{
					if (m_leftMouseButtonPressed)
					{
						m_level.PlaceTilesInArea(rectangle, m_currentTileValue);
					}
					else
					{
						m_level.PlaceTilesInArea(rectangle, 0);
					}
				}
				else if (m_mode == Interface::Mode::COLLSION)
				{
					if (m_leftMouseButtonPressed)
					{
						m_level.PlaceCollisionTilesInArea(rectangle, true);
					}

					else 
					{
						m_level.PlaceCollisionTilesInArea(rectangle, false);
					}
				}
			}
		}

	}
	m_mouseIsDragged = false;
}

void LevelEditor::ProcessMouseWheelScrolled(sf::Event& event)
{
	if (event.mouseWheelScroll.delta < 0)
	{
		for (int i = 0; i > event.mouseWheelScroll.delta; --i)
		{
			m_view.zoom(1.05f);
		}
	}
	else if (event.mouseWheelScroll.delta > 0)
	{
		for (int i = 0; i < event.mouseWheelScroll.delta; ++i)
		{
			m_view.zoom(0.95f);
		}
	}
}

void LevelEditor::ProcessKeyboard(sf::Event& event)
{
	switch (event.key.code)
	{
		case sf::Keyboard::W:
			m_view.move(0, -50);
			break;

		case sf::Keyboard::A:
			m_view.move(-50, 0);
			break;

		case sf::Keyboard::S:
			m_view.move(0, 50);
			break;

		case sf::Keyboard::D:
			m_view.move(50, 0);
			break;

		case sf::Keyboard::Space:
			if (m_interface.ReturnMode() == Interface::Mode::EDIT)
			{
				m_interface.ChangeMode(Interface::Mode::COLLSION);
			}
			else
			{
				m_interface.ChangeMode(Interface::Mode::EDIT);
			}
			break;
	}
}

bool LevelEditor::IsAboveLevel(float x, float y)
{
	if (x < 0 || x >= (m_levelSize.x * m_tileSize.x) || y < 0 || y >= (m_levelSize.y * m_tileSize.y))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool LevelEditor::IsAboveLevelView(int x, int y)
{
	if (x < 1350 && y > 42)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool LevelEditor::IsAboveInterfaceView(int x, int y)
{
	if (x > 1350 || y < 42)
	{
		return true;
	}
	else
	{
		return false;
	}
}