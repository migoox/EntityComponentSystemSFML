#pragma once
#include "../StateSystem/State.h"
#include <SFML/Graphics.hpp>

namespace Framework {
	class Scene : public State
	{
	protected:
		sf::RenderWindow* m_Window;

	public:
		Scene(sf::RenderWindow* window) : m_Window(window) {}

		virtual void UpdateEvents(const sf::Event& event) {};
		virtual void Render() {};
	};
}