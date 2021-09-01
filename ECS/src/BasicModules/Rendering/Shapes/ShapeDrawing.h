#pragma once
#include <SFML/Graphics.hpp>
#include "../../Physics/Colliders/ColliderItem.h"

namespace Basic {

	class BoxShape;
	class CircleShape;
	class PolygonShape;
	class LineShape;

	class ShapeDrawing : public sf::Drawable
	{
	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const  = 0;

	public:
		virtual void SetFillColor(const sf::Color& color) = 0;
		virtual sf::Color GetFillColor() const = 0;

		virtual void SetOutlineColor(const sf::Color& color) = 0;
		virtual sf::Color GetOutlineColor() const = 0;

		// the offset of centroid point from game object's transform point
		virtual void SetOffset(sf::Vector2f offset) = 0;
		virtual sf::Vector2f GetOffset() const = 0;

		virtual void SetRotation(float rotation) = 0;
		virtual float GetRotation() const = 0;

		virtual AABB GetGlobalAABB(const Transform& transform) const = 0;

		virtual void SetOutlineThickness(float thickness) = 0;
		virtual float GetOutlineThickness() const = 0;

		virtual sf::Vector2f GetGlobalPosition(const Transform& transform) const = 0;
	};
}