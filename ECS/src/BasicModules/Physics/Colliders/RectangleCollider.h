#pragma once
#include <SFML/Graphics.hpp>
#include "PolygonCollider.h"
#include "../CollisionDetectionAlgorithms.h"

namespace Basic {
	class RectangleCollider : public PolygonCollider
	{
	private:
		sf::Vector2f m_Size = sf::Vector2f(0.0f, 0.0f);

	public:
		RectangleCollider();
		RectangleCollider(sf::Vector2f size);
		RectangleCollider(sf::Vector2f size, sf::Vector2f displacement);

		void DrawOnceOnVisualGizmos(const Transform& trans) const;

		const std::vector<std::array<sf::Vector2f, 3>>& GlobalTriangles(const Transform& trans) const = delete;

		void Set(sf::Vector2f size, sf::Vector2f displacement);
		void Set(sf::Vector2f size);
	};
}