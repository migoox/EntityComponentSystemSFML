#pragma once
#include "../BasicModules/ECS.h"
#include "../BasicModules/Game.h"
#include "../BasicModules/ResourceManager/ResourceManager.h"
#include "../BasicModules/Components.h"
#include "../BasicModules/EventSystem/Event.h"

using ECSWorld = Basic::World;
using ECSSystem = Basic::System;

using Basic::GameObject;
using Basic::Entity;
using Basic::Game;
using Basic::Event;
using Basic::ResourceManager;
using Basic::ColliderItem;

// external systems/components:
#include "../ExternalComponents/Ball.h"

class InputSystem : public ECSSystem
{
private:
	bool LeftMouseClicked = false;
	bool LeftMouseHeld = false;
	bool LeftMouseReleased = false;

	bool RightMouseClicked = false;
	bool RightMouseHeld = false;
	bool RightMouseReleased = false;

	bool BodyIsSelected = false;

public:
	void Init() override
	{
		SetSignatureType(SignatureType::Inclusive);
		AddToSignature<Collider>();
		AddToSignature<CircleShape>();
		AddToSignature<Ball>();
	}

	void Update(const sf::Time& deltaTime) override
	{
		InputUpdate();

		if (LeftMouseClicked)
		{
			BodyIsSelected = false;
		}

		for (auto& gameObject : m_GameObjects)
		{
			// getting joystick
			auto& ball = gameObject.GetComponent<Ball>();

			// getting circle collider
			CircleCollider* circleCollider = (CircleCollider*)gameObject.GetComponent<Collider>().Item;

			if (ball.SlingshotStretchingStarted)
			{
				ball.SlingshotStretchingStarted = false;
			}

			if (ball.SlingshotStretchingFinished)
			{
				ball.SlingshotStretchingFinished = false;
			}

			if (LeftMouseHeld && RightMouseHeld)
			{
				if (ball.Selected)
				{
					ball.FitMousePosition = false;
					return;
				}
			}

			if (!BodyIsSelected)
			{
				if (LeftMouseClicked)
				{
					if (circleCollider->ContainsGlobalPoint(Game::MouseWorldPosition(), gameObject.GetTransform()))
					{
						if (ball.Selected)
							ball.DoublySelected = true;
						else
							ball.Selected = true;

						BodyIsSelected = true;
					}
				}
			}

			if (LeftMouseClicked)
			{
				if (!circleCollider->ContainsGlobalPoint(Game::MouseWorldPosition(), gameObject.GetTransform()))
				{
					ball.Selected = false;
					ball.DoublySelected = false;
					ball.SlingshotStretchingStarted = false;
					ball.SlingshotStretchingFinished = false;
					ball.SlingshotStretchingHappens = false;
				}
			}

			if (LeftMouseHeld)
			{
				if (ball.DoublySelected)
				{
					if (circleCollider->ContainsGlobalPoint(Game::MouseWorldPosition(), gameObject.GetTransform()))
					{
						// begin changing position by mouse
						ball.FitMousePosition = true;
					}
				}
			}

			if (LeftMouseReleased || ball.SlingshotStretchingHappens)
			{
				// end changing position by mouse
				ball.FitMousePosition = false;
			}

			if (RightMouseClicked)
			{
				if (ball.Selected)
				{
					if (circleCollider->ContainsGlobalPoint(Game::MouseWorldPosition(), gameObject.GetTransform()))
					{
						// begin stretching the slingshot
						ball.SlingshotStretchingStarted = true;
						ball.SlingshotStretchingHappens = true;
					}
				}
			}

			if (RightMouseReleased)
			{
				// end stretching the slingshot
				ball.SlingshotStretchingHappens = false;
				ball.SlingshotStretchingFinished = true;
			}
		}
	}

	void InputUpdate()
	{
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
		{
			LeftMouseReleased = false;

			if (!LeftMouseHeld)
			{
				LeftMouseHeld = true;
				LeftMouseClicked = true;
			}
			else
			{
				LeftMouseClicked = false;
			}
		}
		else
		{
			if (LeftMouseHeld)
			{
				LeftMouseReleased = true;
			}
			else
			{
				LeftMouseReleased = false;
			}

			LeftMouseHeld = false;
			LeftMouseClicked = false;
		}


		if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
		{
			RightMouseReleased = false;

			if (!RightMouseHeld)
			{
				RightMouseHeld = true;
				RightMouseClicked = true;
			}
			else
			{
				RightMouseClicked = false;
			}
		}
		else
		{
			if (RightMouseHeld)
			{
				RightMouseReleased = true;
			}
			else
			{
				RightMouseReleased = false;
			}

			RightMouseHeld = false;
			RightMouseClicked = false;
		}
	}
};