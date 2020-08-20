#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>

class Interface;

class LevelEditor;

class InterfaceButtons : public sf::Transformable, public sf::Drawable
{
public:
	enum class ButtonAction { NEWLEVEL, OPENLEVEL, SAVELEVEL, COLLISION, BLANKTILE };

	InterfaceButtons(float x, float y, ButtonAction buttonAction, Interface *interface, LevelEditor *levelEditor);

	void ProcessMouse(float x, float y);
	void ProcessMouseClick(float x, float y, int buttonID);
	void UpdateMouseOver(sf::Time elapsedTime);

	void EnableButton();
	void DisableButton();

	void ResetButtons();

	bool IsEnabled() const;

private:
	Interface *m_interface;
	LevelEditor* m_levelEditor;

	sf::Texture m_enabledButtonTexture;
	sf::Texture m_disabledButtonTexture;
	sf::Sprite m_sprite;

	ButtonAction m_buttonAction;
	sf::Time m_timeClicked;

	bool m_mouseClicked;
	bool m_isMouseOverButton;
	bool m_isEnabled;

	void draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates) const override;
};
#endif