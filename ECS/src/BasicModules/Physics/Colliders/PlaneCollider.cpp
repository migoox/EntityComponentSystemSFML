#include "PlaneCollider.h"

sf::Vector2f Basic::PlaneCollider::TranslateRelativePointToGlobal(sf::Vector2f point, const Transform& trans) const
{
	sf::Vector2f scale = trans.getScale();
	float angle = trans.getRotation() * 3.141592653f / 180.0f;
	float colliderAngle = m_ColliderRotation * 3.141592653f / 180.0f;

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

float Basic::PlaneCollider::Distance() const
{
	return m_Distance;
}

sf::Vector2f Basic::PlaneCollider::GetGlobalCenterOfGravity(const Transform& trans) const
{
	return TranslateRelativePointToGlobal(sf::Vector2f(0.0f, 0.0f), trans);
}

float Basic::PlaneCollider::GetMomentOfInertia(const RigidBody& rb) const
{
	return 1.0f / 12.0f * float(std::pow(m_Distance, 2)) * rb.Mass;
}

Basic::AABB Basic::PlaneCollider::GetGlobalAABB(const Transform& trans) const
{
	sf::Vector2f globalA = GetGlobalAPoint(trans);
	sf::Vector2f globalB = GetGlobalBPoint(trans);

	if (globalA.x == globalB.x) // vertical
	{
		if (globalA.y > globalB.y) // A is above B
		{
			return { globalA + sf::Vector2f(-1.0f, 0.0f), globalB + sf::Vector2f(1.0f, 0.0f) };
		}
		else if(globalA.y < globalB.y) // B is above A
		{
			return { globalB + sf::Vector2f(-1.0f, 0.0f), globalA + sf::Vector2f(1.0f, 0.0f) };
		}
	}
	else if (globalA.y == globalB.y) // horizontal
	{
		if (globalA.x > globalB.x) // A is on the right of B
		{
			return { globalB + sf::Vector2f(0.0f, -1.0f), globalA + sf::Vector2f(0.0f, 1.0f) };
		}
		else if (globalA.x < globalB.x) // B is on the right of A
		{
			return { globalA + sf::Vector2f(0.0f, -1.0f), globalB + sf::Vector2f(0.0f, 1.0f) };
		}
	}
	else // it is not vertical or horizontal
	{
		sf::Vector2f min;
		sf::Vector2f max;
		bool correct = true;

		if (globalA.x > globalB.x) // A is on the right of B
		{
			max.x = globalA.x;
			min.x = globalB.x;
		}
		else if (globalA.x < globalB.x) // B is on the right of A
		{
			max.x = globalB.x;
			min.x = globalA.x;
		}
		else
		{
			correct = false;
		}

		if (globalB.y > globalA.y) // A is above B
		{
			max.y = globalB.y;
			min.y = globalA.y;
		}
		else if (globalB.y < globalA.y) // B is above A
		{
			max.y = globalA.y;
			min.y = globalB.y;
		}
		else
		{
			correct = false;
		}

		if (correct)
			return { min, max };
	}

	// plane is uncorrectly defined
	return { sf::Vector2f(0.0f, 0.0f), sf::Vector2f(0.0f, 0.0f) };
}

void Basic::PlaneCollider::DrawOnceOnVisualGizmos(const Transform& trans) const
{
	/*Line line(GetGlobalAPoint(trans), GetGlobalBPoint(trans), 4.0f, sf::Color(0.0f, 0.0f, 255.0f, 0.8f * 255.0f));
	CircleShape gCenter(2.0f);
	gCenter.setFillColor(sf::Color(255.0f, 0.0f, 0.0f, 0.8f * 255.0f));
	gCenter.setOrigin(1.0f, 1.0f);
	gCenter.setPosition(GetGlobalCenterOfGravity(trans));

	Basic::VisualGizmos::DrawOnce(line);
	Basic::VisualGizmos::DrawOnce(gCenter);*/
}

void Basic::PlaneCollider::DrawAABBOnceOnVisualGizmos(const Transform& trans) const
{
	/*AABB aabb = GetGlobalAABB(trans);

	Basic::RectangleShape rect;
	rect.setSize(sf::Vector2f(aabb.maxPoint - aabb.minPoint));
	rect.setPosition(aabb.minPoint);
	rect.setFillColor(sf::Color(0.0f, 150.0f, 50.0f, 0.4f * 255.0f));

	Basic::VisualGizmos::DrawOnce(rect);*/
}

void Basic::PlaneCollider::MoveCollider(sf::Vector2f displacement)
{
	m_ColliderDisplacement += displacement;
}

void Basic::PlaneCollider::RotateCollider(float angle)
{
	m_ColliderRotation += angle;
}

sf::Vector2f Basic::PlaneCollider::GetGlobalAPoint(const Transform& trans) const
{
	return TranslateRelativePointToGlobal(sf::Vector2f(-m_Distance / 2.0f, 0.0f), trans);
}

sf::Vector2f Basic::PlaneCollider::GetGlobalBPoint(const Transform& trans) const
{
	return TranslateRelativePointToGlobal(sf::Vector2f(m_Distance / 2.0f, 0.0f), trans);
}

sf::Vector2f Basic::PlaneCollider::FindFurthestPointInDirection(const Transform& transform, sf::Vector2f direction) const
{
	using Maths::Dot;

	// prepare values
	sf::Vector2f aPoint = GetGlobalAPoint(transform);
	sf::Vector2f bPoint = GetGlobalBPoint(transform);

	if (Dot(aPoint, direction) > Dot(bPoint, direction))
		return aPoint;
	else
		return bPoint;
}

float Basic::PlaneCollider::GetLongestDistanceFromCentroid() const
{
	return m_Distance / 2.0f;
}
