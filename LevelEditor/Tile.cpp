#include "Tile.h"
#include "Interface.h"

Tile::Tile(float x, float y, sf::Texture *blankTileTexture, int tileID, Interface *interface)
	:
	m_interface(interface),
	m_selectedTileID(tileID),
	m_showSelectedTile(false)
{
	setPosition(x, y);
	m_rectangleShape.setSize(sf::Vector2f(50.f, 50.f));
	m_rectangleShape.setTexture(blankTileTexture);

	m_blankTileTexture = blankTileTexture;
}

void Tile::ProcessMouseClick(float x, float y, int buttonID) const
{
	sf::Vector2f v = getTransform().getInverse().transformPoint(x, y);

	if (m_rectangleShape.getGlobalBounds().contains(v))
	{
		if (buttonID == 0)
		{
			m_interface->ChangeCurrentTile(m_selectedTileID, m_rectangleShape.getTextureRect());
		}
	}
}

void Tile::SetTilesetTexture(sf::Texture *tilesetTexture)
{
	m_tilesetTexture = tilesetTexture;
}

void Tile::SetTileTexture()
{
	m_rectangleShape.setTexture(m_tilesetTexture);

	m_rectangleShape.setTextureRect(m_textureRectangle);

	m_showSelectedTile = true;
}

void Tile::SetBlankTileTexture()
{
	m_rectangleShape.setTexture(m_blankTileTexture, true);

	m_showSelectedTile = false;
}

void Tile::SetTileRect(const sf::IntRect& rectangle)
{
	m_textureRectangle = rectangle;
}

void Tile::draw(sf::RenderTarget& renderTarget, sf::RenderStates renderStates) const
{
	renderStates.transform *= getTransform();
	renderTarget.draw(m_rectangleShape, renderStates);
}