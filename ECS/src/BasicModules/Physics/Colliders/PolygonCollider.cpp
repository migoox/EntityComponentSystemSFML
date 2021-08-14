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
}

void Basic::PolygonCollider::UpdatePolygon()
{
	// by default polygon isn't correctly defined
	m_Correct = false;

	// if vertices count is less than 3, leave polygon as uncorrectly defined
	// it will not be taken into account during collision checks
	if (m_Vertices.size() < 3)
		return;

	assert(IsSimple(m_Vertices), "Engine Error: Polygon has to be simple! - Sequence of vertices isn't correct.");

	assert(ContainsColinearEdges(m_Vertices), "Engine Error: Polygon can't include colinear edges(when they are neighbours)!");

	// functions below works only if polygon is correctly defined

	// polygon is correctly defined
	m_Correct = true;

	// check if it is convex
	m_Convex = IsPolygonConvex(m_Vertices);

	// triangulate given polygon
	Triangulate(m_Vertices, m_Triangles);

	// create relative position basing on shape of the collider
	m_CenterOfGravity = FindCenterOfGravity(m_Vertices);

	// resize global vertices count to fit relative vertices count
	m_GlobalVertices.resize(m_Vertices.size());

	// counting fixer
	m_Fixer = -m_CenterOfGravity;
}

void Basic::PolygonCollider::UpdateGlobalVertices(const Transform& trans) const
{
	float angle = trans.getRotation() * 3.141592653f / 180.0f;
	for (size_t i = 0; i < m_Vertices.size(); i++)
	{
		sf::Vector2f vertex = m_Vertices[i] + m_CenterDisplacement + m_Fixer;

		vertex = sf::Vector2f(vertex.x * std::cos(angle) - vertex.y * std::sin(angle),
			vertex.x * std::sin(angle) + vertex.y * std::cos(angle));
		
		vertex += trans.getPosition();

		m_GlobalVertices[i] = vertex;
	}
}

void Basic::PolygonCollider::UpdateGlobalTriangles(const Transform& trans) const
{
	float angle = trans.getRotation() * 3.141592653f / 180.0f;
	for (size_t i = 0; i < m_Triangles.size(); i++)
	{
		Triangle triangle;

		// vertex A
		triangle.A = m_Triangles[i].A + m_CenterDisplacement + m_Fixer;

		triangle.A = sf::Vector2f(triangle.A.x * std::cos(angle) - triangle.A.y * std::sin(angle),
			triangle.A.x * std::sin(angle) + triangle.A.y * std::cos(angle));

		triangle.A += trans.getPosition();

		// vertex B
		triangle.B = m_Triangles[i].B + m_CenterDisplacement + m_Fixer;

		triangle.B = sf::Vector2f(triangle.B.x * std::cos(angle) - triangle.B.y * std::sin(angle),
			triangle.B.x * std::sin(angle) + triangle.B.y * std::cos(angle));

		triangle.B += trans.getPosition();

		// vertex C
		triangle.C = m_Triangles[i].C + m_CenterDisplacement + m_Fixer;

		triangle.C = sf::Vector2f(triangle.C.x * std::cos(angle) - triangle.C.y * std::sin(angle),
			triangle.C.x * std::sin(angle) + triangle.C.y * std::cos(angle));

		triangle.C += trans.getPosition();

		m_GlobalTriangles[i] = triangle;
	}
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

void Basic::PolygonCollider::Triangulate(const std::vector<sf::Vector2f>& vertices, std::vector<Triangle>& triangles)
{
	// ear clipping algorithm
	// this method expects correct polygon!

	using MathFunctions::Cross;

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
			// get prev, curr and next indexes of array
			size_t prevIndex = Helpers::GetItem<int>(indexVector, i - 1);
			size_t currIndex = indexVector[i];
			size_t nextIndex = Helpers::GetItem<int>(indexVector, i + 1);

			// get points
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
			triangles.push_back({prev, curr, next});
			indexVector.erase(indexVector.begin() + i);
		}
	}

	// add last triangle
	triangles.push_back({vertices[indexVector[0]], vertices[indexVector[1]], vertices[indexVector[2]]});
}

sf::Vector2f Basic::PolygonCollider::FindCenterOfGravity(const std::vector<sf::Vector2f>& vertices)
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

Basic::PolygonCollider::PolygonCollider()
{
	Init();
}

Basic::PolygonCollider::PolygonCollider(std::initializer_list<sf::Vector2f> list)
{
	Init();
	for (auto& it : list)
		m_Vertices.push_back(it);
	UpdatePolygon();
}

Basic::PolygonCollider::PolygonCollider(const std::vector<sf::Vector2f>& vector)
{
	Init();
	m_Vertices = vector;
	UpdatePolygon();
}

void Basic::PolygonCollider::SetVertex(size_t index, sf::Vector2f vertex)
{
	m_Vertices[index] = vertex;
}

void Basic::PolygonCollider::AddVertex(sf::Vector2f vertex)
{
	m_Vertices.push_back(vertex);
	UpdatePolygon();
}

void Basic::PolygonCollider::MoveCollider(sf::Vector2f displacement)
{
	m_CenterDisplacement += displacement;
}

sf::Vector2f Basic::PolygonCollider::GetGlobalCenterOfGravity(const Transform& trans) const
{
	return m_CenterOfGravity + m_CenterDisplacement + trans.getPosition();
}

float Basic::PolygonCollider::GetMomentumOfInertia(const RigidBody& rb) const
{
	float A = 0.0f;
	float B = 0.0f;

	for (size_t i = 0; i < m_Vertices.size() - 1; i++)
		A += m_Vertices[i].x * m_Vertices[i + 1].y - m_Vertices[i + 1].x * m_Vertices[i].y;

	for (size_t i = 0; i < m_Vertices.size() - 1; i++)
	{
		auto& i1 = m_Vertices[i];
		auto& i2 = m_Vertices[i + 1];

		B += (std::pow(i1.x, 2) + std::pow(i1.y, 2) + i1.x * i2.x + i1.y * i2.y + std::pow(i2.y, 2) + std::pow(i2.y, 2)) * (i1.x * i2.y - i2.x * i1.y);
	}

	return B / A / 6 * rb.Mass;
}

const std::vector<sf::Vector2f>& Basic::PolygonCollider::GlobalVertices(const Transform& trans) const
{
	UpdateGlobalVertices(trans);
	return m_GlobalVertices;
}

const std::vector<Basic::Triangle>& Basic::PolygonCollider::GlobalTriangles(const Transform& trans) const
{
	UpdateGlobalTriangles(trans);
	return m_GlobalTriangles;
}