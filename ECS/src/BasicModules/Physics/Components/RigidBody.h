#pragma once
#include <SFML/Graphics.hpp>

namespace Basic {
	struct RigidBody
	{
		sf::Vector2f Velocity;
		sf::Vector2f Acceleration;
		sf::Vector2f Force;

		sf::Vector2f Impulse;

		float Mass = 10.0f;

		float AngleVelocity = 0.0f; // in radians
		float AngleAcceleration = 0.0f; 
		float Torque = 0.0f; 

		float TorqueImpulse;

		bool FreezeRotation = false;
		bool FreezeXAxisMovement = false;
		bool FreezeYAxisMovement = false;

		bool UseGravity = false;
		
		// adds const force
		void AddForce(sf::Vector2f force)
		{
			Force += force;
		}

		// adds force which will affect the body only for one frame
		void AddImpulse(sf::Vector2f force)
		{
			Impulse += force;
		}

		// adds const torque
		void AddTorque(float torque)
		{
			Torque += torque;
		}

		// adds torque which will affect the body only for one frame
		void AddTorqueImpulse(float torque)
		{
			TorqueImpulse += torque;
		}
	};
}