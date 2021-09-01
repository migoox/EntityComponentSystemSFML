#pragma once
#include "ShapeDrawing.h"

namespace Basic {
	class BoxShape : public ShapeDrawing
	{
	private:
		sf::RectangleShape m_SFMLRect;

		sf::Vector2f m_Offset;
		float m_Rotation;
		
		std::array<sf::Vector2f, 4> m_Vertices;

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
		{
			// draw the shape
			target.draw(m_SFMLRect, states);
		}

		sf::Vector2f TranslateRelativePointToGlobal(sf::Vector2f point, const Transform& trans) const;

	public:
		BoxShape();
		BoxShape(sf::Vector2f size);

		void SetFillColor(const sf::Color& color) override;
		sf::Color GetFillColor() const override;

		void SetOutlineColor(const sf::Color& color) override;
		sf::Color GetOutlineColor() const override;

		// the offset of centroid point from game object's transform point
		void SetOffset(sf::Vector2f offset) override;
		sf::Vector2f GetOffset() const;

		void SetRotation(float rotation) override;
		float GetRotation() const override;

		void SetOutlineThickness(float thickness) override;
		float GetOutlineThickness() const override;

		AABB GetGlobalAABB(const Transform& transform) const override;

		sf::Vector2f GetGlobalPosition(const Transform& transform) const override;
	};
}