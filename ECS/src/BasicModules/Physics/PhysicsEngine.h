#pragma once
#include <SFML/Graphics.hpp>

namespace Basic{
	class PhysicsEngine
	{
	private:
		bool m_TopView;
		float m_GravityAccelerationValue;
		sf::Vector2f m_GravityDirection;

		static PhysicsEngine s_Instance;

	private:
		PhysicsEngine();

		void ISetAsTopView();
		void ISetAsSideView();

		void ISetGravityAccelerationValue(float acc);
		void ISetGravityAccelerationDirection(sf::Vector2f direction);

	public:
		static void SetAsTopView();
		static void SetAsSideView();
		static bool IsTopViewToggled();
		static bool IsSideViewToggled();

		static void SetGravityAccelerationValue(float acc);
		static float GravityAccelerationValue();

		static void SetGravityAccelerationDirection(sf::Vector2f direction);
		static sf::Vector2f GravityAccelerationDirection();
	};
}