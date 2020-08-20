#ifndef GUI_H
#define GUI_H

#include "Button.h"
#include "Tile.h"

#include <SFML/Graphics.hpp>

class LevelEditor;

class Interface : public sf::Transformable, public sf::Drawable
{
public:
	enum class ButtonID { NEWLEVEL, OPENLEVEL, SAVELEVEL, COLLISION };

	enum Mode { EDIT, COLLSION };

	Interface(int width, int height, LevelEditor *levelEditor);
	~Interface();

	void LoadTileset(const std::string &tileset, float tileWidth, float tileHeight, int tileGap);

	void ProcessMouse(float x, float y);
	void ProcessMouseClicked(float x, float y, int buttonID);
		
	void ChangeMode(Mode mode);
	void ChangeCurrentTile(int tileValue, sf::IntRect textureRect);

	void ResetButtons();

	void LevelOpened();

	Interface::Mode ReturnMode();

private:
	LevelEditor* m_levelEditor;

	Mode m_mode;
	
	std::vector<InterfaceButtons*> m_buttons;
	std::vector<Tile*> m_tiles;
	static int buttonID(ButtonID buttonID);

	sf::Texture m_tilesetTexture;
	sf::Vector2u m_tileSize;
	int m_tileGap;

	sf::Texture m_emptyTileTexture;

	sf::RectangleShape m_currentTile;

	sf::RectangleShape m_menuLines[2];
	sf::RectangleShape m_tilesetBackground[2];

	void SetTileset();

	void draw(sf::RenderTarget &renderTargets, sf::RenderStates renderStates) const override;
};
#endif