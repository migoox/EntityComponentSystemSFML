#pragma once
#include <SFML/Graphics.hpp>

namespace Basic {
	struct RigidBody
	{
		sf::Vector2f Velocity;
		sf::Vector2f Acceleration;
		sf::Vector2f Force;

		float Mass = 1.0f;

		float AngleVelocity = 0.0f;
		float AngleAcceleration = 0.0f;
		float Torque = 0.0f;

		bool FreezeRotation = false;
		bool FreezeXAxisMovement = false;
		bool FreezeYAxisMovement = false;

		bool UseGravity = false;

		void AddForce(sf::Vector2f force)
		{
			Force += force;
		}

		void ImpulseForce(sf::Vector2f force, float durationTime = 0.1f)
		{
			Velocity += durationTime / Mass * force;
		}
	};
}