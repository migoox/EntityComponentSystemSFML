#pragma once
#include "ShapeDrawing.h"
#include "../../Maths/Maths.h"

namespace Basic {
	class LineShape : public ShapeDrawing
	{
	private:
		sf::Vector2f m_LocalPointA;
		sf::Vector2f m_LocalPointB;

		float m_Thickness;
		
		sf::Vector2f m_Offset;
		float m_Rotation;

		sf::Color m_Color;

		sf::VertexArray m_VertexArray;

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
		{
			// draw the shape
			target.draw(m_VertexArray, states);
		}

		sf::Vector2f TranslateRelativePointToGlobal(sf::Vector2f point, const Transform& trans) const;

		void UpdateVertexArray();

		void UpdateColors();

	public:
		LineShape();
		LineShape(float distance, float thickness);

		void SetFillColor(const sf::Color& color) override;
		sf::Color GetFillColor() const override;

		void SetOutlineColor(const sf::Color& color) override {}
		sf::Color GetOutlineColor() const override { return sf::Color::White; }

		// the offset of centroid point from game object's transform point
		void SetOffset(sf::Vector2f offset) override;
		sf::Vector2f GetOffset() const;

		void SetRotation(float rotation) override;
		float GetRotation() const override;

		void SetOutlineThickness(float thickness) override { }
		float GetOutlineThickness() const override { return 0.0f; }

		AABB GetGlobalAABB(const Transform& transform) const override;

		sf::Vector2f GetGlobalPosition(const Transform& transform) const override;
	};
}