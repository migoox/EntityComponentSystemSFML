#pragma once
#include <SFML/Graphics.hpp>
#include "../Shapes/ShapeDrawing.h"

namespace Basic {
	struct Shape
	{
		ShapeDrawing* Drawing;
		Shape()
		{
			Drawing = nullptr;
		}

		Shape(ShapeDrawing* drawing) : Drawing(drawing) { }

		Shape(const Shape& other) = default;

		Shape& operator=(const Shape& other) = default;

		Shape(Shape&& other) noexcept
		{
			Drawing = other.Drawing;
			other.Drawing = nullptr;
		}

		Shape& operator=(Shape&& other) noexcept
		{
			Drawing = other.Drawing;
			other.Drawing = nullptr;

			return *this;
		}

		ShapeDrawing* operator->() const
		{
			return Drawing;
		}

		~Shape()
		{
			if (Drawing != nullptr)
			{
				delete Drawing;
			}
		}
	};
}