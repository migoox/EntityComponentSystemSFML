#pragma once
#include "../StateSystem/State.h"
#include <SFML/Graphics.hpp>

namespace Framework {
	class Scene : public State
	{
	protected:

	public:

		virtual void UpdateEvents(const sf::Event& event) {};
		virtual void Render(sf::RenderTarget& target) {};
	};
}