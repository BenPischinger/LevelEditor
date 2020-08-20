#ifndef TILE_BUTTON_H
#define TILE_BUTTON_H

#include "Button.h"

#include <SFML/Graphics.hpp>

class Interface;

class Tile : public sf::Transformable, public sf::Drawable
{
public:

	Tile(float x, float y, sf::Texture *blankTileTexture, int tileID, Interface *interface);

	void ProcessMouseClick(float x, float y, int buttonID) const;

	void SetTilesetTexture(sf::Texture *tilesetTexture);
	void SetTileRect(const sf::IntRect &rectangle);
	void SetTileTexture();
	void SetBlankTileTexture();
	
private:
	Interface* m_interface;

	sf::Texture *m_tilesetTexture;
	sf::Texture *m_blankTileTexture;
	
	sf::IntRect m_textureRectangle;
	sf::RectangleShape m_rectangleShape;

	sf::Time m_clickedTime;

	int m_selectedTileID;

	bool m_mouseClicked;
	bool m_showSelectedTile;

	void draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates) const override;
};
#endif