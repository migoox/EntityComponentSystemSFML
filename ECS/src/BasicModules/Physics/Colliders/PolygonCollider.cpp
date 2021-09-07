#include "PolygonCollider.h"
#include <iostream>
int Basic::PolygonHelpers::FixIndex(int index, int size)
{
	if (index >= size)
		return index % size;
	else if (index < 0)
		return index % size + size;
	else
		return index;
}

bool Basic::PolygonHelpers::PointInTriangle(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c, const sf::Vector2f& p)
{
	using Maths::Cross;

	if (Cross(b - a, p - a) <= 0.0f &&
		Cross(c - b, p - b) <= 0.0f &&
		Cross(a - c, p - c) <= 0.0f)
		return true;

	return false;
}

bool Basic::PolygonHelpers::LinesIntersect(const sf::Vector2f& a, const sf::Vector2f& b, const sf::Vector2f& c, const sf::Vector2f& d)
{
	if (a == c && b == d)
		return true;

	float denominator = ((b.x - a.x) * (d.y - c.y)) - ((b.y - a.y) * (d.x - c.x));
	float numerator1 = ((a.y - c.y) * (d.x - c.x)) - ((a.x - c.x) * (d.y - c.y));
	float numerator2 = ((a.y - c.y) * (b.x - a.x)) - ((a.x - c.x) * (b.y - a.y));

	// Detect coincident lines (has a problem, read below)
	if (denominator == 0) return numerator1 == 0 && numerator2 == 0;

	float r = numerator1 / denominator;
	float s = numerator2 / denominator;

	return (r >= 0 && r <= 1) && (s >= 0 && s <= 1);
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

	assert(IsSimple(m_Vertices) && "Engine Error: Polygon has to be simple! - Sequence of vertices isn't correct.");

	assert(!ContainsColinearEdges(m_Vertices) && "Engine Error: Polygon can't include colinear edges(when they are neighbours)!");

	// functions below works only if polygon is correctly defined

	// polygon is correctly defined
	m_Correct = true;

	// check if it is convex
	m_Convex = IsPolygonConvex(m_Vertices);

	// triangulate given polygon
	Triangulate(m_Vertices, m_Triangles);

	// create relative position basing on shape of the collider
	m_CenterOfGravity = FindCenterOfGravity(m_Vertices);

	// resize global vertices and global triangles count to fit relative vertices count
	m_GlobalVertices.resize(m_Vertices.size());
	m_GlobalTriangles.resize(m_Vertices.size() - 2);

	// counting fixer
	m_Fixer = -m_CenterOfGravity;

	m_FurthestPointFromCentroidIndex = FindFurthestPointFromCentroidIndex();
}

void Basic::PolygonCollider::UpdateGlobalVertices(const Transform& trans) const
{
	for (size_t i = 0; i < m_Vertices.size(); i++)
		m_GlobalVertices[i] = TranslateRelativePointToGlobal(m_Vertices[i], trans);
}

void Basic::PolygonCollider::UpdateGlobalTriangles(const Transform& trans) const
{
	float angle = trans.getRotation() * 3.141592653f / 180.0f;
	for (size_t i = 0; i < m_Triangles.size(); i++)
	{
		m_GlobalTriangles[i] = {
			TranslateRelativePointToGlobal(m_Triangles[i][0], trans),
			TranslateRelativePointToGlobal(m_Triangles[i][0], trans),
			TranslateRelativePointToGlobal(m_Triangles[i][0], trans)
		};
	}
}

bool Basic::PolygonCollider::IsSimple(const std::vector<sf::Vector2f>& vertices)
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

bool Basic::PolygonCollider::ContainsColinearEdges(const std::vector<sf::Vector2f>& vertices)
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

bool Basic::PolygonCollider::IsPolygonConvex(const std::vector<sf::Vector2f>& vertices)
{
	using Maths::Cross;

	// algorithm is going to check if there is a angle inside of the polygon
	// which is greater than 90 degrees
	// it compares all of the cross products of two vectors 
	// created from neighbours of current point and this point
	// with the first cross product

	// n represents the amount of the vertices
	int n = vertices.size();

	// cross procucts
	float crossProduct;
	float firstCrossProduct;

	// neighbours of current point
	sf::Vector2f neighbours[2];

	// getting neighbours
	neighbours[0] = PolygonHelpers::GetItem<sf::Vector2f>(vertices, 0 - 1);
	neighbours[1] = PolygonHelpers::GetItem<sf::Vector2f>(vertices, 0 + 1);

	// create vectors
	sf::Vector2f n1P = vertices[0] - neighbours[0]; // neighbour[0] -> curr point
	sf::Vector2f n2P = neighbours[1] - vertices[0]; // curr point -> neighbour[1]

	// counting cross product of these vectors
	firstCrossProduct = Cross(n1P, n2P);

	// going through all of the vertices
	for (int i = 1; i < n; i++)
	{
		// getting neighbours
		neighbours[0] = PolygonHelpers::GetItem<sf::Vector2f>(vertices, i - 1);
		neighbours[1] = PolygonHelpers::GetItem<sf::Vector2f>(vertices, i + 1);

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

void Basic::PolygonCollider::Triangulate(const std::vector<sf::Vector2f>& vertices, std::vector<std::array<sf::Vector2f, 3>>& triangles)
{
	// ear clipping algorithm
	// this method expects correct polygon!
	using Maths::Cross;

	// clear triangles
	triangles.clear();

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
			triangles.push_back({prev, curr, next});
			indexVector.erase(indexVector.begin() + i);
			break;
		}
	}
	// add last triangle
	triangles.push_back({vertices[indexVector[0]], vertices[indexVector[1]], vertices[indexVector[2]]});
}

sf::Vector2f Basic::PolygonCollider::FindCenterOfGravity(const std::vector<sf::Vector2f>& vertices) const 
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

sf::Vector2f Basic::PolygonCollider::TranslateRelativePointToGlobal(sf::Vector2f point, const Transform& trans) const
{
	sf::Vector2f scale = trans.getScale();
	float angle = trans.getRotation() * 3.141592653f / 180.0f;
	float colliderAngle = m_ColliderRotation * 3.141592653f / 180.0f;

	point += m_Fixer;

	point = sf::Vector2f(
		point.x * std::cos(colliderAngle) - point.y * std::sin(colliderAngle),
		point.x * std::sin(colliderAngle) + point.y * std::cos(colliderAngle));

	sf::Vector2f global = point + m_ColliderDisplacement;

	global.x *= scale.x;
	global.y *= scale.y;

	global = sf::Vector2f(
		global.x * std::cos(angle) - global.y * std::sin(angle),
		global.x * std::sin(angle) + global.y * std::cos(angle));

	global += trans.getPosition();

	return global;
}

int Basic::PolygonCollider::FindFurthestPointFromCentroidIndex()
{
	using Maths::VectorDistance;

	float maxDistance = -FLT_MAX;
	int bestIndex = 0;

	int i = 0;
	for (auto& point : m_Vertices)
	{
		float distance = VectorDistance(m_CenterOfGravity - point);
		if (distance > maxDistance)
		{
			bestIndex = i;
			maxDistance = distance;
		}
		i++;
	}

	return bestIndex;
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

Basic::PolygonCollider::PolygonCollider(std::initializer_list<sf::Vector2f> list, sf::Vector2f displacementOfCenter)
{
	Init();
	for (auto& it : list)
		m_Vertices.push_back(it);
	UpdatePolygon();
	MoveCollider(displacementOfCenter);
}

void Basic::PolygonCollider::Clear()
{
	m_Vertices.clear();
	m_GlobalVertices.clear();

	m_Triangles.clear();
	m_GlobalTriangles.clear();

	Init();
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
	m_ColliderDisplacement += displacement;
}

void Basic::PolygonCollider::RotateCollider(float angle)
{
	m_ColliderRotation += angle;
}

sf::Vector2f Basic::PolygonCollider::GetGlobalCenterOfGravity(const Transform& trans) const
{
	return TranslateRelativePointToGlobal(m_CenterOfGravity, trans);
}

float Basic::PolygonCollider::GetMomentOfInertia(const RigidBody& rb) const
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

Basic::AABB Basic::PolygonCollider::GetGlobalAABB(const Transform& trans) const
{
	sf::Vector2f min(FLT_MAX, FLT_MAX);
	sf::Vector2f max(-FLT_MAX, -FLT_MAX);

	for (size_t i = 0; i < m_Vertices.size(); i++)
	{
		auto& curr = m_GlobalVertices[i];

		curr = TranslateRelativePointToGlobal(m_Vertices[i], trans);

		if (curr.x > max.x)
		{
			max.x = curr.x;
		}
		if (curr.x < min.x) 
		{
			min.x = curr.x;
		}

		if (curr.y > max.y) 
		{
			max.y = curr.y;
		}
		if (curr.y < min.y)
		{
			min.y = curr.y;
		}
	}

	return { min, max };
}

void Basic::PolygonCollider::DrawOnceOnVisualGizmos(const Transform& trans) const
{
	float angle = trans.getRotation() * 3.141592653f / 180.0f;

	auto arr1 = sf::VertexArray(sf::PrimitiveType::LinesStrip, m_Vertices.size() + 1);

	for (size_t i = 0; i < m_Vertices.size(); i++)
	{
		arr1[i].position = TranslateRelativePointToGlobal(m_Vertices[i], trans);

		arr1[i].color = sf::Color(0.0f, 26.0f, 102.0f, 0.8f * 255.0f);
	}
	arr1[m_Vertices.size()] = arr1[0];

	auto arr2 = sf::VertexArray(sf::PrimitiveType::Triangles, m_Triangles.size() * 3);

	for (size_t i = 0, j = 0; j < m_Triangles.size(); i += 3, j++)
	{
		arr2[i].position = TranslateRelativePointToGlobal(m_Triangles[j][0], trans);
		arr2[i + 1].position = TranslateRelativePointToGlobal(m_Triangles[j][1], trans);
		arr2[i + 2].position = TranslateRelativePointToGlobal(m_Triangles[j][2], trans);

		sf::Color color = sf::Color(0.0f, 60.0f + 20.0f * (j % 4), 150.0f + 30 * (j % 4), 0.8f * 255.0f);
		arr2[i].color = color;
		arr2[i + 1].color = color;
		arr2[i + 2].color = color;
	}

	/*CircleShape gCenter(2.0f);
	gCenter.setFillColor(sf::Color(255.0f, 0.0f, 0.0f, 0.8f * 255.0f));
	gCenter.setOrigin(1.0f, 1.0f);
	gCenter.setPosition(GetGlobalCenterOfGravity(trans));

	Basic::VisualGizmos::DrawOnce(arr2);
	Basic::VisualGizmos::DrawOnce(arr1);
	Basic::VisualGizmos::DrawOnce(gCenter);*/
}

void Basic::PolygonCollider::DrawAABBOnceOnVisualGizmos(const Transform& trans) const
{
	/*AABB aabb = GetGlobalAABB(trans);

	Basic::RectangleShape rect;
	rect.setSize(sf::Vector2f(aabb.maxPoint - aabb.minPoint));
	rect.setPosition(aabb.minPoint);
	rect.setFillColor(sf::Color(0.0f, 150.0f, 50.0f, 0.4f * 255.0f));

	Basic::VisualGizmos::DrawOnce(rect);*/
}

const std::vector<sf::Vector2f>& Basic::PolygonCollider::GlobalVertices(const Transform& trans) const
{
	UpdateGlobalVertices(trans);
	return m_GlobalVertices;
}

const std::vector<std::array<sf::Vector2f, 3>>& Basic::PolygonCollider::GlobalTriangles(const Transform& trans) const
{
	UpdateGlobalTriangles(trans);
	return m_GlobalTriangles;
}

sf::Vector2f Basic::PolygonCollider::FindFurthestPointInDirection(const Transform& transform, sf::Vector2f direction) const
{
	assert(m_Correct && "PhysicsEngine: Can't find furthest point, since polygon is incorrectly defined.");

	assert(m_Convex && "PhysicsEngine: Can't find furthest point, since polygon is concave.");

	using Maths::Dot;


	// prepare values
	sf::Vector2f maxPoint = TranslateRelativePointToGlobal(m_Vertices[0], transform);
	float maxDistance = Dot(maxPoint, direction);

	// find max point
	for (int i = 1; i < m_Vertices.size(); i++)
	{
		sf::Vector2f currPoint = TranslateRelativePointToGlobal(m_Vertices[i], transform);
		float distance = Dot(currPoint, direction);
		if (distance > maxDistance)
		{
			maxDistance = distance;
			maxPoint = currPoint;
		}
	}

	return maxPoint;
}

int Basic::PolygonCollider::FindFurthestPointInDirectionIndex(const Transform& transform, sf::Vector2f direction) const
{
	assert(m_Correct && "PhysicsEngine: Can't find furthest point, since polygon is incorrectly defined.");

	assert(m_Convex && "PhysicsEngine: Can't find furthest point, since polygon is concave.");

	using Maths::Dot;


	// prepare values
	sf::Vector2f maxPoint = TranslateRelativePointToGlobal(m_Vertices[0], transform);
	float maxDistance = Dot(maxPoint, direction);
	int index = 0;

	// find max point
	for (int i = 1; i < m_Vertices.size(); i++)
	{
		sf::Vector2f currPoint = TranslateRelativePointToGlobal(m_Vertices[i], transform);
		float distance = Dot(currPoint, direction);
		if (distance > maxDistance)
		{
			maxDistance = distance;
			maxPoint = currPoint;
			index = i;
		}
	}

	return index;
}

float Basic::PolygonCollider::GetLongestDistanceFromCentroid() const
{
	using Maths::VectorDistance;
	return VectorDistance(m_CenterOfGravity - m_Vertices[m_FurthestPointFromCentroidIndex]);
}

sf::Vector2f Basic::PolygonCollider::GetGlobalPoint(const Transform& transform, int index) const
{
	assert(index < m_Vertices.size() && "Polygon's vertex array out of range.");

	return TranslateRelativePointToGlobal(m_Vertices[index], transform);
}

bool Basic::PolygonCollider::ContainsPoint(sf::Vector2f point)
{
	return false;
}

Basic::ClippingAlgo::CPEdge Basic::PolygonCollider::GetTheBestClippingEdge(const Transform& transform, sf::Vector2f normal) const
{
	using Maths::Dot;
	using Maths::NormalizeVector;

	// all operations are done on clockwise, correctly defined convex polygons
	assert(m_Correct && "PhysicsEngine: Can't find furthest point, since polygon is incorrectly defined.");

	assert(m_Convex && "PhysicsEngine: Can't find furthest point, since polygon is concave.");

	ClippingAlgo::CPEdge edge;

	// getting furthest point in given direction
	int furthestPointIndex = FindFurthestPointInDirectionIndex(transform, normal);

	// getting value
	edge.FurthestPoint = TranslateRelativePointToGlobal(m_Vertices[furthestPointIndex], transform);

	// get neighbours of furthest point
	int prevIndex = PolygonHelpers::FixIndex(furthestPointIndex - 1, m_Vertices.size());
	sf::Vector2f prev = TranslateRelativePointToGlobal(m_Vertices[prevIndex], transform);

	int nextIndex = PolygonHelpers::FixIndex(furthestPointIndex + 1, m_Vertices.size());
	sf::Vector2f next = TranslateRelativePointToGlobal(m_Vertices[nextIndex], transform);

	// create edge vectors(clockwise!)
	sf::Vector2f vec1 = edge.FurthestPoint - prev;
	sf::Vector2f vec2 = next - edge.FurthestPoint;

	// check which edge is better one
	if (std::abs(Dot(vec1, normal)) < std::abs(Dot(vec2, normal)))
	{
		// vec1 is better
		edge.A = prev;
		edge.B = edge.FurthestPoint;
		edge.Distance = Maths::Distance(edge.A, edge.B);
		edge.Normal = NormalizeVector(sf::Vector2f(vec1.y, -vec1.x));
	}
	else
	{
		// vec2 is better
		edge.A = edge.FurthestPoint;
		edge.B = next;
		edge.Distance = Maths::Distance(edge.A, edge.B);
		edge.Normal = NormalizeVector(sf::Vector2f(vec2.y, -vec2.x));
	}

	return edge;
}

