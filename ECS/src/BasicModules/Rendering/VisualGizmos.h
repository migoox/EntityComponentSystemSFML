#pragma once
#include <SFML/Graphics.hpp>
#include <array>

#include "Other/Line.h"
#include "../Physics/Colliders/ColliderItem.h"

namespace Basic {
	class VisualGizmos
	{
	private:
		static VisualGizmos s_Instance;

		std::vector<Line> m_Lines;
		std::vector<sf::CircleShape> m_CircleShapes;
		std::vector<sf::VertexArray> m_VertexArrays;
		std::vector<sf::RectangleShape> m_RectangleShapes;

		std::vector<Line> m_TempLines;
		std::vector<sf::CircleShape> m_TempCircleShapes;
		std::vector<sf::VertexArray> m_TempVertexArrays;
		std::vector<sf::RectangleShape> m_TempRectangleShapes;

	private:
		VisualGizmos();

		void IDraw(Line line);
		void IDraw(sf::CircleShape circle);
		void IDraw(sf::VertexArray verArr);
		void IDraw(sf::RectangleShape rect);

		void IDrawOnce(Line line);
		void IDrawOnce(sf::CircleShape circle);
		void IDrawOnce(sf::VertexArray verArr);
		void IDrawOnce(sf::RectangleShape rect);

		void IClearLines();
		void IClearCircleShapes();
		void IClearVertexArrays();
		void IClearRectangleShapes();
		void IClearDrawableElements();
		void IClear();

		void IRender(sf::RenderTarget& target);
	public:
		static void Draw(Line line);
		static void Draw(sf::CircleShape circle);
		static void Draw(sf::VertexArray verArr);
		static void Draw(sf::RectangleShape rect);

		static void DrawOnce(Line line);
		static void DrawOnce(sf::CircleShape circle);
		static void DrawOnce(sf::VertexArray verArr);
		static void DrawOnce(sf::RectangleShape rect);

		static void ClearLines();
		static void ClearCircleShapes();
		static void ClearVertexArrays();
		static void ClearRectangleShapes();
		static void Clear();

		static void Render(sf::RenderTarget& target);
	};
}
