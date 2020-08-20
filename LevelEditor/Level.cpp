#include "Level.h"
#include <iostream>
#include <fstream>

Level::Level()
	:
	m_tileGap(0),
	m_numberOfTiles(nullptr),
	m_collisionTileValues(nullptr),
	m_collisionTilesEnabled(false)
{
}

Level::~Level()
{
	delete[] m_numberOfTiles;
	delete[] m_collisionTileValues;
}

void Level::ReadFromFile(std::ifstream *file)
{
	m_tilesetTexturePath = ReadString(file);

	int tileSizeX;
	int tileSizeY;

	int tileGap;

	int levelSizeX;
	int levelSizeY;

	file->read(reinterpret_cast<char*>(&tileSizeX), sizeof(tileSizeX));
	file->read(reinterpret_cast<char*>(&tileSizeY), sizeof(tileSizeY));

	file->read(reinterpret_cast<char*>(&tileGap), sizeof(tileGap));
	
	file->read(reinterpret_cast<char*>(&levelSizeX), sizeof(levelSizeX));
	file->read(reinterpret_cast<char*>(&levelSizeY), sizeof(levelSizeY));
		
	int* tiles = new int[levelSizeX * levelSizeY];
	
	file->read(reinterpret_cast<char*>(tiles), sizeof(int) * levelSizeX * levelSizeY);
	
	bool* collisionTiles = new bool[levelSizeX * levelSizeY];

	file->read(reinterpret_cast<char*>(collisionTiles), sizeof(bool) * levelSizeX * levelSizeY);

	LoadLevel(m_tilesetTexturePath, sf::Vector2u(tileSizeX, tileSizeY), tiles, levelSizeX, levelSizeY, tileGap, collisionTiles);

	delete[] tiles;
	delete[] collisionTiles;
}

void Level::WriteToFile(std::ofstream *file)
{
	WriteString(file, m_tilesetTexturePath);

	file->write(reinterpret_cast<const char*>(&m_tileSize.x), sizeof(m_tileSize.x));
	file->write(reinterpret_cast<const char*>(&m_tileSize.y), sizeof(m_tileSize.y));

	file->write(reinterpret_cast<const char*>(&m_tileGap), sizeof(m_tileGap));

	file->write(reinterpret_cast<const char*>(&m_levelSize.x), sizeof(m_levelSize.x));
	file->write(reinterpret_cast<const char*>(&m_levelSize.y), sizeof(m_levelSize.y));

	file->write(reinterpret_cast<const char*>(m_numberOfTiles), sizeof(int) * m_levelSize.x * m_levelSize.y);

	file->write(reinterpret_cast<const char*>(m_collisionTileValues), sizeof(bool) * m_levelSize.x * m_levelSize.y);
}

bool Level::LoadLevel(std::string &tilesetTexturePath, sf::Vector2u tileSize, int* tiles, int width, int height, int tileGap, bool* collisionTiles)
{
	m_tilesetTexturePath = tilesetTexturePath;

	m_tileSize = tileSize;
	m_tileGap = tileGap;

	m_levelSize.x = width;
	m_levelSize.y = height;

	m_tilesetTexture.loadFromFile(tilesetTexturePath);

	m_tileVertices.setPrimitiveType(sf::Quads);
	m_tileVertices.resize(width * height * 5);

	for (unsigned int i = 0; i < width; ++i)
	{
		for (unsigned int j = 0; j < height; ++j)
		{
			int tile = tiles[i + j * width];

			int xOffset = tile % (m_tilesetTexture.getSize().x / tileSize.x);
			int yOffset = tile / (m_tilesetTexture.getSize().x / tileSize.x);

			sf::Vertex* quad = &m_tileVertices[(i + j * width) * 4];

			quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
			quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
			quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
			quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

			quad[0].texCoords = sf::Vector2f(xOffset * (tileSize.x + tileGap), yOffset * (tileSize.y + tileGap));
			quad[1].texCoords = sf::Vector2f(xOffset * (tileSize.x + tileGap) + tileSize.x, yOffset * (tileSize.y + tileGap));
			quad[2].texCoords = sf::Vector2f(xOffset * (tileSize.x + tileGap) + tileSize.x, yOffset * (tileSize.y + tileGap) + tileSize.y);
			quad[3].texCoords = sf::Vector2f(xOffset * (tileSize.x + tileGap), yOffset * (tileSize.y + tileGap) + tileSize.y);
		}
	}

	m_numberOfTiles = new int[width * height];

	for (int i = 0; i < width * height; ++i)
	{
		m_numberOfTiles[i] = tiles[i];
	}

	m_collisionTileValues = new bool[width * height];

	for (int i = 0; i < width * height; ++i)
	{
		m_collisionTileValues[i] = collisionTiles[i];
	}

	m_collisionVertices.setPrimitiveType(sf::Quads);
	m_collisionVertices.resize(width * height * 5);

	for (unsigned int i = 0; i < width; ++i)
	{
		for (unsigned int j = 0; j < height; ++j)
		{
			sf::Vertex* quad = &m_collisionVertices[(i + j * width) * 4];

			quad[0].position = sf::Vector2f(i * tileSize.x, j * tileSize.y);
			quad[1].position = sf::Vector2f((i + 1) * tileSize.x, j * tileSize.y);
			quad[2].position = sf::Vector2f((i + 1) * tileSize.x, (j + 1) * tileSize.y);
			quad[3].position = sf::Vector2f(i * tileSize.x, (j + 1) * tileSize.y);

			if (m_collisionTileValues[i + j * width])
			{
				quad[0].color = sf::Color(255, 0, 0, 127);
				quad[1].color = sf::Color(255, 0, 0, 127);
				quad[2].color = sf::Color(255, 0, 0, 127);
				quad[3].color = sf::Color(255, 0, 0, 127);
			}
			else
			{
				quad[0].color = sf::Color::Transparent;
				quad[1].color = sf::Color::Transparent;
				quad[2].color = sf::Color::Transparent;
				quad[3].color = sf::Color::Transparent;
			}
		}
	}

	return true;
}

void Level::PlaceTileAtMousePosition(float x, float y, int value)
{
	int levelX; 
	int levelY;

	levelX = x / m_tileSize.x;
	levelY = y / m_tileSize.y;

	if (levelX < 0 || levelX >= m_levelSize.x || levelY < 0 || levelY >= m_levelSize.y)
	{
		return;
	}

	m_numberOfTiles[levelX + levelY * m_levelSize.x] = value;

	UpdateTileVertices(levelX, levelY);
}

void Level::PlaceTilesInArea(sf::FloatRect rect, int value)
{
	int levelX1; 
	int levelY1;

	int levelX2;
	int levelY2;

	levelX1 = rect.left / m_tileSize.x;
	levelY1 = rect.top / m_tileSize.y;

	levelX2 = (rect.left + rect.width) / m_tileSize.x;
	levelY2 = (rect.top + rect.height) / m_tileSize.y;

	if (levelX1 < 0 || levelX1 >= m_levelSize.x || levelY1 < 0 || levelY1 >= m_levelSize.y)
	{
		return;
	}

	if (levelX2 < 0 || levelX2 >= m_levelSize.x || levelY2 < 0 || levelY2 >= m_levelSize.y)
	{
		return;
	}

	for (int i = levelX1; i <= levelX2; ++i)
	{
		for (int j = levelY1; j <= levelY2; ++j)
		{
			m_numberOfTiles[i + j * m_levelSize.x] = value;

			UpdateTileVertices(i, j);
		}
	}
}

void Level::PlaceCollisionTileAtMousePosition(float x, float y, bool collision)
{
	int levelX;
	int levelY;

	levelX = x / m_tileSize.x;
	levelY = y / m_tileSize.y;

	if (levelX < 0 || levelX >= m_levelSize.x || levelY < 0 || levelY >= m_levelSize.y)
	{
		return;
	}

	m_collisionTileValues[levelX + levelY * m_levelSize.x] = collision;

	UpdateCollisionTileVertices(levelX, levelY, collision);
}

void Level::PlaceCollisionTilesInArea(sf::FloatRect rect, bool collision)
{
	int levelX1;
	int levelY1;

	int levelX2;
	int levelY2;

	levelX1 = rect.left / m_tileSize.x;
	levelY1 = rect.top / m_tileSize.y;

	levelX2 = (rect.left + rect.width) / m_tileSize.x;
	levelY2 = (rect.top + rect.height) / m_tileSize.y;

	if (levelX1 < 0 || levelX1 >= m_levelSize.x || levelY1 < 0 || levelY1 >= m_levelSize.y)
	{
		return;
	}

	if (levelX2 < 0 || levelX2 >= m_levelSize.x || levelY2 < 0 || levelY2 >= m_levelSize.y)
	{
		return;
	}

	for (int i = levelX1; i <= levelX2; ++i)
	{
		for (int j = levelY1; j <= levelY2; ++j)
		{
			m_collisionTileValues[i + j * m_levelSize.x] = collision;

			UpdateCollisionTileVertices(i, j, collision);
		}
	}
}

sf::Vector2u& Level::ReturnTileSize() 
{
	return m_tileSize;
}

sf::Vector2u& Level::ReturnLevelSize() 
{
	return m_levelSize;
}

std::string& Level::ReturnTexturePath() 
{
	return m_tilesetTexturePath;
}

int Level::ReturnTileGap() 
{
	return m_tileGap;
}

void Level::EnableCollisionTiles()
{
	m_collisionTilesEnabled = true;
}

void Level::DisableCollisionTiles()
{
	m_collisionTilesEnabled = false;
}

std::string Level::ReadString(std::ifstream *file)
{
	std::string string;

	int length;

	file->read(reinterpret_cast<char*>(&length), sizeof(length));

	if (length > 0)
	{
		char* buffer = new char[length];

		file->read(buffer, length);

		string.append(buffer, length);

		delete[] buffer;
	}
	return string;
}

void Level::WriteString(std::ofstream *file, std::string &string)
{
	int length = string.size();

	file->write(reinterpret_cast<const char*>(&length), sizeof(length));

	file->write(string.c_str(), length);
}

void Level::UpdateTileVertices(int x, int y)
{
	int tile = m_numberOfTiles[x + y * m_levelSize.x];

	int xOffset = tile % (m_tilesetTexture.getSize().x / m_tileSize.x);
	int yOffset = tile / (m_tilesetTexture.getSize().x / m_tileSize.x);

	sf::Vertex* quad = &m_tileVertices[(x + y * m_levelSize.x) * 4];

	quad[0].texCoords = sf::Vector2f(xOffset * (m_tileSize.x + m_tileGap), yOffset * (m_tileSize.y + m_tileGap));
	quad[1].texCoords = sf::Vector2f(xOffset * (m_tileSize.x + m_tileGap) + m_tileSize.x, yOffset * (m_tileSize.y + m_tileGap));
	quad[2].texCoords = sf::Vector2f(xOffset * (m_tileSize.x + m_tileGap) + m_tileSize.x, yOffset * (m_tileSize.y + m_tileGap) + m_tileSize.y);
	quad[3].texCoords = sf::Vector2f(xOffset * (m_tileSize.x + m_tileGap), yOffset * (m_tileSize.y + m_tileGap) + m_tileSize.y);
}

void Level::UpdateCollisionTileVertices(int x, int y, bool collision)
{
	sf::Vertex* quad = &m_collisionVertices[(x + y * m_levelSize.x) * 4];

	if (collision)
	{
		quad[0].color = sf::Color(255, 0, 0, 127);
		quad[1].color = sf::Color(255, 0, 0, 127);
		quad[2].color = sf::Color(255, 0, 0, 127);
		quad[3].color = sf::Color(255, 0, 0, 127);
	}
	else
	{
		quad[0].color = sf::Color::Transparent;
		quad[1].color = sf::Color::Transparent;
		quad[2].color = sf::Color::Transparent;
		quad[3].color = sf::Color::Transparent;
	}
}

void Level::draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates) const
{
	renderStates.transform *= getTransform();
	renderStates.texture = &m_tilesetTexture;

	renderTarget.draw(m_tileVertices, renderStates);

	if (m_collisionTilesEnabled)
	{
		renderTarget.draw(m_collisionVertices, renderStates);
	}
}