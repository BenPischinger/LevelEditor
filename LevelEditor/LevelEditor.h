#pragma
#include "Interface.h"
#include "Level.h"

#include <SFML/Graphics.hpp>

class LevelEditor
{
public:
	int main();

	LevelEditor(unsigned int width, unsigned int height);
	~LevelEditor();

	void Initialise();

	void NewLevel();
	void OpenLevel();
	void SaveLevel();

	void ChangeMode(Interface::Mode mode);
	void ChangeTileValue(int tileValue);
	
private:
	sf::RenderWindow m_window;
	sf::View m_view;

	Level m_level;
	sf::Vector2u m_levelSize;

	int m_currentTileValue;
	sf::Vector2u m_tileSize;
	sf::RectangleShape m_tileSelector;
	bool m_showTileSelector;

	sf::Vector2f m_mouseDragStart;
	sf::Vector2f m_mouseDragEnd;

	bool m_mouseIsDragged; 
	bool m_leftMouseButtonPressed;

	bool m_collision;
	
	Interface m_interface;
	sf::View m_interfaceView;

	Interface::Mode m_mode;
	
	void Render();

	void ProcessEvents();
	void ProcessMouse(sf::Event &event);
	void ProcessMouseButtonPressed(sf::Event &event);
	void ProcessMouseButtonReleased(sf::Event &event);
	void ProcessMouseWheelScrolled(sf::Event &event);
	void ProcessKeyboard(sf::Event &event);

	bool IsAboveLevel(float x, float y);
	bool IsAboveLevelView(int x, int y);
	bool IsAboveInterfaceView(int x, int y);
};