#pragma once
#include <vector>

#include "ShapeDrawing.h"
#include "../../Physics/Colliders/PolygonCollider.h"

namespace Basic {
	class PolygonShape : public ShapeDrawing
	{
	private:

		sf::Vector2f m_Fixer;

		sf::Vector2f m_Offset;
		float m_Rotation = 0.0f;

		sf::Vector2f m_Centroid;

		sf::Color m_Color;

		bool m_Correct;

		mutable sf::VertexArray m_VertexArray;
		std::vector<sf::Vector2f> m_Vertices;

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
		{
			// draw the shape
			target.draw(m_VertexArray, states);
		}

		void UpdatePolygon();

		void UpdateColors();

		void UpdateVertexArray(sf::Vector2f oldOffset, sf::Vector2f newOffset) const;

		bool IsSimple(const std::vector<sf::Vector2f>& vertices);

		bool ContainsColinearEdges(const std::vector<sf::Vector2f>& vertices);

		void Triangulate(const std::vector<sf::Vector2f>& vertices, sf::VertexArray& triangles);

		sf::Vector2f FindCentroid(const std::vector<sf::Vector2f>& vertices) const;

		sf::Vector2f TranslateRelativePointToGlobal(sf::Vector2f point, const Transform& trans) const;

	public:
		PolygonShape();

		PolygonShape(std::initializer_list<sf::Vector2f> list);

		PolygonShape(const std::vector<sf::Vector2f>& vector);

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