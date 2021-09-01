#include "PolygonShape.h"

sf::Vector2f Basic::PolygonShape::TranslateRelativePointToGlobal(sf::Vector2f point, const Transform& trans) const
{
	sf::Vector2f scale = trans.getScale();
	float angle = trans.getRotation() * 3.141592653f / 180.0f;
	float colliderAngle = m_Rotation * 3.141592653f / 180.0f;

	point += m_Fixer;

	point = sf::Vector2f(
		point.x * std::cos(colliderAngle) - point.y * std::sin(colliderAngle),
		point.x * std::sin(colliderAngle) + point.y * std::cos(colliderAngle));

	sf::Vector2f global = point + m_Offset;

	global.x *= scale.x;
	global.y *= scale.y;

	global = sf::Vector2f(
		global.x * std::cos(angle) - global.y * std::sin(angle),
		global.x * std::sin(angle) + global.y * std::cos(angle));

	global += trans.getPosition();

	return global;
}

Basic::PolygonShape::PolygonShape()
{
	m_Correct = false;
}

Basic::PolygonShape::PolygonShape(std::initializer_list<sf::Vector2f> list)
{
	m_Correct = false;
	for (auto& it : list)
		m_Vertices.push_back(it);
	UpdatePolygon();
}

Basic::PolygonShape::PolygonShape(const std::vector<sf::Vector2f>& vector)
{
	m_Correct = false;
	m_Vertices = vector;
	UpdatePolygon();
}

void Basic::PolygonShape::SetFillColor(const sf::Color& color)
{
	m_Color = color;
	UpdateColors();
}

sf::Color Basic::PolygonShape::GetFillColor() const
{
	return m_Color;
}

void Basic::PolygonShape::SetOutlineColor(const sf::Color& color)
{

}

sf::Color Basic::PolygonShape::GetOutlineColor() const
{
	return sf::Color::White;
}

void Basic::PolygonShape::SetOffset(sf::Vector2f offset)
{
	UpdateVertexArray(m_Offset, offset);
	m_Offset = offset;
}

sf::Vector2f Basic::PolygonShape::GetOffset() const
{
	return m_Offset;
}

void Basic::PolygonShape::SetRotation(float rotation)
{
	m_Rotation = rotation;

	float colliderAngle = m_Rotation * 3.141592653f / 180.0f;

	for (int i = 0; i < (m_Vertices.size() - 2) * 3; i++)
	{
		auto& curr = m_VertexArray[i].position;
		curr -= m_Offset;
		curr = sf::Vector2f(
			curr.x * std::cos(colliderAngle) - curr.y * std::sin(colliderAngle),
			curr.x * std::sin(colliderAngle) + curr.y * std::cos(colliderAngle));
		curr += m_Offset;
	}
}

float Basic::PolygonShape::GetRotation() const
{
	return m_Rotation;
}

void Basic::PolygonShape::SetOutlineThickness(float thickness)
{
	
}

float Basic::PolygonShape::GetOutlineThickness() const
{
	return 0.0f;
}

Basic::AABB Basic::PolygonShape::GetGlobalAABB(const Transform& transform) const
{
	Basic::AABB aabb;

	aabb.minPoint = sf::Vector2f(FLT_MAX, FLT_MAX);
	aabb.maxPoint = sf::Vector2f(-FLT_MAX, -FLT_MAX);

	for (auto& vertex : m_Vertices)
	{
		sf::Vector2f curr = TranslateRelativePointToGlobal(vertex, transform);

		aabb.minPoint.x = std::min(curr.x, aabb.minPoint.x);
		aabb.minPoint.y = std::min(curr.y, aabb.minPoint.y);

		aabb.maxPoint.x = std::max(curr.x, aabb.maxPoint.x);
		aabb.maxPoint.y = std::max(curr.y, aabb.maxPoint.y);
	}

	return aabb;
}

sf::Vector2f Basic::PolygonShape::GetGlobalPosition(const Transform& transform) const
{
	return TranslateRelativePointToGlobal(m_Centroid, transform);
}

void Basic::PolygonShape::UpdatePolygon()
{
	m_Correct = false;

	if (m_Vertices.size() < 3)
	{
		m_Correct = false;
		return;
	}

	assert(IsSimple(m_Vertices) && "Engine Error: PolygonShape has to be simple! - Sequence of vertices isn't correct.");

	assert(!ContainsColinearEdges(m_Vertices) && "Engine Error: PolygonShape can't include colinear edges(when they are neighbours)!");

	m_Correct = true;

	Triangulate(m_Vertices, m_VertexArray);

	m_Centroid = FindCentroid(m_Vertices);

	m_Fixer = -m_Centroid;

	for (int i = 0; i < (m_Vertices.size() - 2) * 3; i++)
	{
		m_VertexArray[i].position += m_Fixer;
	}
}

void Basic::PolygonShape::UpdateColors()
{
	for (int i = 0; i < (m_Vertices.size() - 2) * 3; i++)
	{
		m_VertexArray[i].color = m_Color;
	}
}

void Basic::PolygonShape::UpdateVertexArray(sf::Vector2f oldOffset, sf::Vector2f newOffset) const
{
	for (int i = 0; i < (m_Vertices.size() - 2) * 3; i++)
	{
		m_VertexArray[i].position -= oldOffset;
		m_VertexArray[i].position += newOffset;
	}
}

bool Basic::PolygonShape::IsSimple(const std::vector<sf::Vector2f>& vertices)
{
	std::vector<std::pair<sf::Vector2f, sf::Vector2f>> lines;
	lines.reserve(vertices.size());

	for (size_t i = 0; i < vertices.size(); i++)
		lines.push_back(
			std::pair<sf::Vector2f, sf::Vector2f>
			(
				vertices[i],
				PolygonHelpers::GetItem<sf::Vector2f>(vertices, i + 1)
				)
		);

	for (size_t i = 0; i < lines.size(); i++)
	{
		for (size_t j = i + 2; j < lines.size(); j++)
		{
			// prevent comparing neighbours
			if (i + 1 == j) continue;
			if (i == 0 && j == lines.size() - 1) continue;

			// check intersections
			if (PolygonHelpers::LinesIntersect(lines[i].first, lines[i].second,
				lines[j].first, lines[j].second))
				return false;
		}
	}

	return true;
}

bool Basic::PolygonShape::ContainsColinearEdges(const std::vector<sf::Vector2f>& vertices)
{
	using Maths::Cross;

	// going through all of the vertices
	for (int i = 0; i < vertices.size(); i++)
	{
		// get points
		auto& prev = PolygonHelpers::GetItem<sf::Vector2f>(vertices, i - 1);
		auto& curr = PolygonHelpers::GetItem<sf::Vector2f>(vertices, i);
		auto& next = PolygonHelpers::GetItem<sf::Vector2f>(vertices, i + 1);

		// count vectors
		sf::Vector2f vectorCurrToPrev = prev - curr;
		sf::Vector2f vectorCurrToNext = next - curr;

		// vectors are collinear
		if (Cross(vectorCurrToPrev, vectorCurrToNext) == 0.0f)
			return true;
	}

	return false;
}

void Basic::PolygonShape::Triangulate(const std::vector<sf::Vector2f>& vertices, sf::VertexArray& triangles)
{
	// ear clipping algorithm
	// this method expects correct polygon!
	using Maths::Cross;

	// clear triangles
	triangles.clear();
	triangles.setPrimitiveType(sf::PrimitiveType::Triangles);
	triangles.resize(3 * (vertices.size() - 2));

	int currTriangleIndex = 0;

	// create vector of available indexes
	std::vector<int> indexVector;

	for (int i = 0; i < vertices.size(); i++)
	{
		indexVector.push_back(i);
	}

	// main loop
	while (indexVector.size() > 3)
	{
		// iterate trough all available indexes
		for (int i = 0; i < indexVector.size(); i++)
		{
			// get prev, curr and next indexes of array
			size_t prevIndex = PolygonHelpers::GetItem<int>(indexVector, i - 1);
			size_t currIndex = indexVector[i];
			size_t nextIndex = PolygonHelpers::GetItem<int>(indexVector, i + 1);

			// get points
			auto& prev = vertices[prevIndex];
			auto& curr = vertices[i];
			auto& next = vertices[nextIndex];

			// count vectors
			sf::Vector2f vectorCurrToPrev = prev - curr;
			sf::Vector2f vectorCurrToNext = next - curr;

			// if angle is not convex - skip
			if (Cross(vectorCurrToPrev, vectorCurrToNext) > 0.0f) continue;

			// check if there is a point inside of the triangle
			bool pointInTriangleExists = false;

			for (size_t j = 0; j < vertices.size(); j++)
			{
				// do not take into account prev, curr and next indexes
				if (j == prevIndex || j == currIndex || j == nextIndex) continue;

				// if there is a point in triangle
				if (PolygonHelpers::PointInTriangle(prev, curr, next, vertices[j]))
				{
					// raise the flag and break the loop
					pointInTriangleExists = true;
					break;
				}
			}

			// if there is a point inside of the triangle - skip
			if (pointInTriangleExists) continue;

			// all of the conditions are met, add triangle and erase curr index
			triangles[currTriangleIndex * 3] = prev;
			triangles[currTriangleIndex * 3 + 1] = curr;
			triangles[currTriangleIndex * 3 + 2] = next;

			indexVector.erase(indexVector.begin() + i);
			break;
		}
		currTriangleIndex++;
	}
	// add last triangle
	triangles[currTriangleIndex * 3] = vertices[indexVector[0]];
	triangles[currTriangleIndex * 3 + 1] = vertices[indexVector[1]];
	triangles[currTriangleIndex * 3 + 2] = vertices[indexVector[2]];
}

sf::Vector2f Basic::PolygonShape::FindCentroid(const std::vector<sf::Vector2f>& vertices) const
{
	// based on maths covered by wiki:
	// https://en.wikipedia.org/wiki/Centroid (of a polyogn) 
	// formula on wiki:
	// X = SUM[(Xi + Xi+1) * (Xi * Yi+1 - Xi+1 * Yi)] / 6 / A
	// Y = SUM[(Yi + Yi + 1) * (Xi * Yi + 1 - Xi + 1 * Yi)] / 6 / A
	// where A = SUM((Xi * Yi+1 - Xi+1 * Yi)) / 2

	float A = 0.0f;
	sf::Vector2f centroid = sf::Vector2f(0.0f, 0.0f);

	// count A parameter
	for (size_t i = 0; i < vertices.size() - 1; i++)
		A += vertices[i].x * vertices[i + 1].y - vertices[i + 1].x * vertices[i].y;

	A *= 0.5f;

	// countnig centroid
	for (size_t i = 0; i < vertices.size() - 1; i++)
	{
		float parameter = (vertices[i].x * vertices[i + 1].y - vertices[i + 1].x * vertices[i].y);
		centroid.x += (vertices[i].x + vertices[i + 1].x) * parameter;
		centroid.y += (vertices[i].y + vertices[i + 1].y) * parameter;
	}

	centroid.x = centroid.x / 6 / A;
	centroid.y = centroid.y / 6 / A;

	return centroid;
}
