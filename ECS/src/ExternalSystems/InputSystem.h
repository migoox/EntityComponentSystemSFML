#pragma once
#include "../BasicModules/ECS.h"
#include "../BasicModules/Game.h"
#include "../BasicModules/Tools/ResourceManager.h"
#include "../BasicModules/Components.h"
#include "../BasicModules/EventSystem/Event.h"

using ECSWorld = Basic::World;
using ECSSystem = Basic::System;

using Basic::GameObject;
using Basic::Entity;
using Basic::Game;
using Basic::Event;
using Basic::ResourceManager;

using namespace Basic::Components;

// external systems/components:
#include "../ExternalComponents/GrabbableElement.h"

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

private:
	bool ContainsGlobalPoint(const GameObject& gameObject, const Collider& coll)
	{
		auto aabb = coll->GetGlobalAABB(gameObject.GetTransform());
		auto mousePos = Game::MouseWorldPosition();

		if (aabb.minPoint.x < mousePos.x && aabb.maxPoint.x > mousePos.x &&
			aabb.minPoint.y < mousePos.y && aabb.maxPoint.y > mousePos.y)
		{
			return true;
		}

		return false;
	}

public:
	void Init() override
	{
		SetSignatureType(SignatureType::Inclusive);
		AddToSignature<Collider>();
		AddToSignature<GrabbableElement>();
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
			auto& grabbableElement = gameObject.GetComponent<GrabbableElement>();

			// getting circle collider
			auto& collider = gameObject.GetComponent<Collider>();

			if (grabbableElement.SlingshotStretchingStarted)
			{
				grabbableElement.SlingshotStretchingStarted = false;
			}

			if (grabbableElement.SlingshotStretchingFinished)
			{
				grabbableElement.SlingshotStretchingFinished = false;
			}

			if (LeftMouseHeld && RightMouseHeld)
			{
				if (grabbableElement.Selected)
				{
					grabbableElement.FitMousePosition = false;
					return;
				}
			}

			if (!BodyIsSelected)
			{
				if (LeftMouseClicked)
				{
					if (ContainsGlobalPoint(gameObject, collider))
					{
						if (grabbableElement.Selected)
							grabbableElement.DoublySelected = true;
						else
							grabbableElement.Selected = true;

						BodyIsSelected = true;
					}
				}
			}

			if (LeftMouseClicked)
			{
				if (!ContainsGlobalPoint(gameObject, collider))
				{
					grabbableElement.Selected = false;
					grabbableElement.DoublySelected = false;
					grabbableElement.SlingshotStretchingStarted = false;
					grabbableElement.SlingshotStretchingFinished = false;
					grabbableElement.SlingshotStretchingHappens = false;
				}
			}

			if (LeftMouseHeld)
			{
				if (grabbableElement.DoublySelected)
				{
					if (ContainsGlobalPoint(gameObject, collider))
					{
						// begin changing position by mouse
						grabbableElement.FitMousePosition = true;
					}
				}
			}

			if (LeftMouseReleased || grabbableElement.SlingshotStretchingHappens)
			{
				// end changing position by mouse
				grabbableElement.FitMousePosition = false;
			}

			if (RightMouseClicked)
			{
				if (grabbableElement.Selected)
				{
					if (ContainsGlobalPoint(gameObject, collider))
					{
						// begin stretching the slingshot
						grabbableElement.SlingshotStretchingStarted = true;
						grabbableElement.SlingshotStretchingHappens = true;
					}
				}
			}

			if (RightMouseReleased)
			{
				// end stretching the slingshot
				grabbableElement.SlingshotStretchingHappens = false;
				grabbableElement.SlingshotStretchingFinished = true;
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