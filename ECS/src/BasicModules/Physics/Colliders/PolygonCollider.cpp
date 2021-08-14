#include "PolygonCollider.h"
bool Basic::Helpers::PointInTriangle(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c, const sf::Vector2f& p)
{
	using MathFunctions::Cross;

	if (Cross(b - a, p - a) <= 0.0f &&
		Cross(c - b, p - b) <= 0.0f &&
		Cross(a - c, p - c) <= 0.0f)
		return true;

	return false;
}

void Basic::PolygonCollider::Init()
{
	m_Correct = false;
	m_Convex = false;
	m_RelativeCenterPosition = sf::Vector2f(0.0f, 0.0f);
	m_GlobalCenterPosition = sf::Vector2f(0.0f, 0.0f);
}

void Basic::PolygonCollider::UpdatePolygon()
{
	m_Correct = false;

	if (IsSimple(m_Vertices))
		return;

	if (ContainsColinearEdges(m_Vertices))
		return;

	m_Convex = IsPolygonConvex(m_Vertices);
	if (m_Convex)
	{
		m_Polygons.push_back({ m_Vertices });
	}
	else
	{
		if (!Triangulate(m_Vertices, m_Polygons))
			return;
	}

	m_Correct = true;
}

bool Basic::PolygonCollider::IsSimple(const std::vector<sf::Vector2f>& vertices)
{
	return false;
}

bool Basic::PolygonCollider::ContainsColinearEdges(const std::vector<sf::Vector2f>& vertices)
{
	return false;
}

bool Basic::PolygonCollider::IsPolygonConvex(std::vector<sf::Vector2f>& vertices)
{
	using MathFunctions::Cross;

	// algorithm is going to check if there is a angle inside of the polygon
	// which is greater than 90 degrees
	// it compares all of the cross products of two vectors 
	// created from neighbours of current point and this point
	// with the first cross product

	// n represents the amount of the vertices
	size_t n = vertices.size();

	// cross procucts
	float crossProduct;
	float firstCrossProduct;

	// neighbours of current point
	sf::Vector2f neighbours[2];

	// getting neighbours
	neighbours[0] = Helpers::GetItem<sf::Vector2f>(vertices, 0 - 1);
	neighbours[1] = Helpers::GetItem<sf::Vector2f>(vertices, 0 + 1);

	// create vectors
	sf::Vector2f n1P = vertices[0] - neighbours[0]; // neighbour[0] -> curr point
	sf::Vector2f n2P = neighbours[1] - vertices[0]; // curr point -> neighbour[1]

	// counting cross product of these vectors
	firstCrossProduct = Cross(n1P, n2P);

	// going through all of the vertices
	for (size_t i = 1; i < n; i++)
	{
		// getting neighbours
		neighbours[0] = Helpers::GetItem<sf::Vector2f>(vertices, i - 1);
		neighbours[1] = Helpers::GetItem<sf::Vector2f>(vertices, i + 1);

		// create vectors
		n1P = vertices[i] - neighbours[0]; // neighbour[0] -> curr point
		n2P = neighbours[1] - vertices[i]; // curr point -> neighbour[1]

		// counting cross product of these vectors
		crossProduct = Cross(n1P, n2P);

		// if first cross product and current cross product has opposite sings
		if (firstCrossProduct * crossProduct < 0)
		{
			return false;
		}
	}
	return true;
}

bool Basic::PolygonCollider::Triangulate(const std::vector<sf::Vector2f>& vertices, std::vector<Polygon>& triangles)
{
	using MathFunctions::Cross;
	
	// if it isn't a polygon return false
	if (vertices.size() < 3) 
		return false;

	// create vector of available indexes
	std::vector<int> indexVector;

	for (size_t i = 0; i < vertices.size(); i++)
	{
		indexVector.push_back(i);
	}

	// prepare total counts of elements
	size_t totalTriangleCount = vertices.size() - 2;
	size_t totalTriangleVerticesCount = totalTriangleCount * 3;

	// main loop
	while (indexVector.size() > 3)
	{
		// iterate trough all available indexes
		for (size_t i = 0; indexVector.size(); i++)
		{
			// gain prev, curr and next index of array
			size_t prevIndex = Helpers::GetItem<int>(indexVector, i - 1);
			size_t currIndex = indexVector[i];
			size_t nextIndex = Helpers::GetItem<int>(indexVector, i + 1);

			// gain points
			auto& prev = vertices[prevIndex];
			auto& curr = vertices[i];
			auto& next = vertices[nextIndex];

			// count vectors
			sf::Vector2f vectorCurrToPrev = prev - curr;
			sf::Vector2f vectorCurrToNext = next - curr;

			// if angle is not convex - skip
			if (Cross(vectorCurrToPrev, vectorCurrToNext) < 0.0f) continue;

			// check if there is a point inside of the triangle
			bool pointInTriangleExists = false;

			for (size_t j = 0; j < indexVector.size(); j++)
			{
				// do not take into account prev, curr and next indexes
				if (j = prevIndex || j == currIndex || j == nextIndex) continue;

				// if there is a point in triangle
				if (Helpers::PointInTriangle(prev, curr, next, vertices[j]))
				{
					// raise the flag and break the loop
					pointInTriangleExists = true;
					break;
				}
			}

			// if there is a point inside of the triangle - skip
			if (pointInTriangleExists) continue;

			// all of the conditions are met, add triangle and erase curr index
			triangles.push_back({ {prev, curr, next} });
			indexVector.erase(indexVector.begin() + i);
		}
	}

	// add last triangle
	triangles.push_back({ {vertices[indexVector[0]], vertices[indexVector[1]], vertices[indexVector[2]]} });

	return false;
}

Basic::PolygonCollider::PolygonCollider()
{
	Init();
}

Basic::PolygonCollider::PolygonCollider(std::initializer_list<sf::Vector2f> list)
{
	Init();
	if (list.size() >= 3)
	{
		for (auto& it : list)
		{
			m_Vertices.push_back(it);
		}
		UpdatePolygon();
	}
}
