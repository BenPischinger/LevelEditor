#include <iostream>

#include "Button.h"
#include "Interface.h"
#include "LevelEditor.h"

InterfaceButtons::InterfaceButtons(float x, float y, ButtonAction buttonAction, Interface *interface, LevelEditor *levelEditor)
	:
	m_interface(interface),
	m_levelEditor(levelEditor),
	m_buttonAction(buttonAction),
	m_mouseClicked(false),
	m_isMouseOverButton(false),
	m_isEnabled(true)
{
	setPosition(x, y);

	std::string enabledButtonTexture = "Resources/Interface/";

	std::string disabledButtonTexture = "Resources/Interface/";

	switch(buttonAction)
	{
	case ButtonAction::NEWLEVEL:
		enabledButtonTexture.append("NewLevelButton.png");
		disabledButtonTexture.append("NewLevelButton.png");
		break;
	case ButtonAction::OPENLEVEL:
		enabledButtonTexture.append("OpenLevelButton.png");
		disabledButtonTexture.append("OpenLevelButton.png");
		break;
	case ButtonAction::SAVELEVEL:
		enabledButtonTexture.append("SaveLevelButtonEnabled.png");
		disabledButtonTexture.append("SaveLevelButtonDisabled.png");
		break;
	case ButtonAction::COLLISION:
		enabledButtonTexture.append("CollisionDisabled.png");
		disabledButtonTexture.append("CollisionEnabled.png");
		break;
	case ButtonAction::BLANKTILE:
		enabledButtonTexture.append("BlankTile.png");
		break;
	}

	m_enabledButtonTexture.loadFromFile(enabledButtonTexture);

	m_disabledButtonTexture.loadFromFile(disabledButtonTexture);

	m_sprite.setTexture(m_enabledButtonTexture);
}

void InterfaceButtons::ProcessMouse(float x, float y)
{
	sf::Vector2f vector = getTransform().getInverse().transformPoint(x, y);

	if (m_sprite.getGlobalBounds().contains(vector) && m_isEnabled)
	{
		m_sprite.setColor(sf::Color(240, 240, 240, 127));

		m_isMouseOverButton = true;
	}
	else
	{
		m_sprite.setColor(sf::Color::White);

		m_isMouseOverButton = false;
	}
}

void InterfaceButtons::ProcessMouseClick(float x, float y, int buttonID)
{
	sf::Vector2f vector = getTransform().getInverse().transformPoint(x, y);

	if (m_sprite.getGlobalBounds().contains(vector))
	{
		if(buttonID == 0)
		{
			m_mouseClicked = true;

			m_timeClicked = sf::seconds(0.18f);

			m_sprite.setColor(sf::Color(240, 240, 240, 127));

			switch (m_buttonAction)
			{
			case ButtonAction::NEWLEVEL:
				m_levelEditor->NewLevel();
				break;
			case ButtonAction::OPENLEVEL:
				m_levelEditor->OpenLevel();
				break;
			case ButtonAction::SAVELEVEL:
				m_levelEditor->SaveLevel();
				break;
			case ButtonAction::COLLISION:
				if (m_interface->ReturnMode() == Interface::Mode::EDIT)
				{
					m_interface->ChangeMode(Interface::Mode::COLLSION);
				}
				else
				{
					m_interface->ChangeMode(Interface::Mode::EDIT);
				}
				break;
			case ButtonAction::BLANKTILE:
				break;
			}
		}
	}
}

void InterfaceButtons::UpdateMouseOver(sf::Time elapsedTime)
{
	if (m_mouseClicked)
	{
		m_timeClicked -= elapsedTime;

		if (m_timeClicked <= sf::Time::Zero)
		{
			m_mouseClicked = false;

			if (m_isMouseOverButton && m_isEnabled)
			{
				m_sprite.setColor(sf::Color(240, 240, 240, 127));
			}
			else
			{
				m_sprite.setColor(sf::Color::White);
			}
		}
	}
}

void InterfaceButtons::EnableButton()
{
	m_isEnabled = true;

	m_sprite.setTexture(m_enabledButtonTexture);
}

void InterfaceButtons::DisableButton()
{
	if (m_interface->ReturnMode() == Interface::Mode::COLLSION)
	{
		m_sprite.setTexture(m_disabledButtonTexture);
	}
	else
	{
		m_isEnabled = false;

		m_sprite.setTexture(m_disabledButtonTexture);
	}
}

void InterfaceButtons::ResetButtons()
{
	m_sprite.setColor(sf::Color::White);
}

bool InterfaceButtons::IsEnabled() const
{
	return m_isEnabled;
}

void InterfaceButtons::draw(sf::RenderTarget &renderTarget, sf::RenderStates renderStates) const
{
	renderStates.transform *= getTransform();

	renderTarget.draw(m_sprite, renderStates);
}