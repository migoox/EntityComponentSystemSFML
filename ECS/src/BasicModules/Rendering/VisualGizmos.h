#pragma once
#include <SFML/Graphics.hpp>
#include <array>

#include "Components/Line.h"
#include "Components/CircleShape.h"
#include "Components/VertexArray.h"
#include "Components/RectangleShape.h"
#include "../Physics/Colliders/ColliderItem.h"

namespace Basic {
	class VisualGizmos
	{
	private:
		static VisualGizmos s_Instance;

		std::vector<Line> m_Lines;
		std::vector<CircleShape> m_CircleShapes;
		std::vector<VertexArray> m_VertexArrays;
		std::vector<RectangleShape> m_RectangleShapes;

		std::vector<Line> m_TempLines;
		std::vector<CircleShape> m_TempCircleShapes;
		std::vector<VertexArray> m_TempVertexArrays;
		std::vector<RectangleShape> m_TempRectangleShapes;

	private:
		VisualGizmos();

		void IDraw(Line line);
		void IDraw(CircleShape circle);
		void IDraw(VertexArray verArr);
		void IDraw(RectangleShape rect);

		void IDrawOnce(Line line);
		void IDrawOnce(CircleShape circle);
		void IDrawOnce(VertexArray verArr);
		void IDrawOnce(RectangleShape rect);

		void IClearLines();
		void IClearCircleShapes();
		void IClearVertexArrays();
		void IClearRectangleShapes();
		void IClearDrawableElements();
		void IClear();

		void IRender(sf::RenderTarget& target);
	public:
		static void Draw(Line line);
		static void Draw(CircleShape circle);
		static void Draw(VertexArray verArr);
		static void Draw(RectangleShape rect);

		static void DrawOnce(Line line);
		static void DrawOnce(CircleShape circle);
		static void DrawOnce(VertexArray verArr);
		static void DrawOnce(RectangleShape rect);

		static void ClearLines();
		static void ClearCircleShapes();
		static void ClearVertexArrays();
		static void ClearRectangleShapes();
		static void Clear();

		static void Render(sf::RenderTarget& target);
	};
}
