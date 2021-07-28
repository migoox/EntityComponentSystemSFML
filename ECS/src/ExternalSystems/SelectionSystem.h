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
#include "../ExternalComponents/Selectable.h"

class SelectionSystem : public ECSSystem
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
		AddToSignature<Selectable>();
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
			auto& selectable = gameObject.GetComponent<Selectable>();

			// getting circle collider
			CircleCollider* circleCollider = (CircleCollider*)gameObject.GetComponent<Collider>().Item;

			if (selectable.SlingshotStretchingStarted)
			{
				selectable.SlingshotStretchingStarted = false;
			}

			if (selectable.SlingshotStretchingFinished)
			{
				selectable.SlingshotStretchingFinished = false;
			}

			if (LeftMouseHeld && RightMouseHeld)
			{
				if (selectable.Selected)
				{
					selectable.FitMousePosition = false;
					return;
				}
			}

			if (!BodyIsSelected)
			{
				if (LeftMouseClicked)
				{
					if (circleCollider->ContainsGlobalPoint(Game::MouseWorldPosition(), gameObject.GetTransform()))
					{
						if (selectable.Selected)
							selectable.DoublySelected = true;
						else
							selectable.Selected = true;

						BodyIsSelected = true;
					}
				}
			}

			if (LeftMouseClicked)
			{
				if (!circleCollider->ContainsGlobalPoint(Game::MouseWorldPosition(), gameObject.GetTransform()))
				{
					selectable.Selected = false;
					selectable.DoublySelected = false;
					selectable.SlingshotStretchingStarted = false;
					selectable.SlingshotStretchingFinished = false;
					selectable.SlingshotStretchingHappens = false;
				}
			}

			if (LeftMouseHeld)
			{
				if (selectable.DoublySelected)
				{
					if (circleCollider->ContainsGlobalPoint(Game::MouseWorldPosition(), gameObject.GetTransform()))
					{
						// begin changing position by mouse
						selectable.FitMousePosition = true;
					}
				}
			}

			if (LeftMouseReleased || selectable.SlingshotStretchingHappens)
			{
				// end changing position by mouse
				selectable.FitMousePosition = false;
			}

			if (RightMouseClicked)
			{
				if (selectable.Selected)
				{
					if (circleCollider->ContainsGlobalPoint(Game::MouseWorldPosition(), gameObject.GetTransform()))
					{
						// begin stretching the slingshot
						selectable.SlingshotStretchingStarted = true;
						selectable.SlingshotStretchingHappens = true;
					}
				}
			}

			if (RightMouseReleased)
			{
				// end stretching the slingshot
				selectable.SlingshotStretchingHappens = false;
				selectable.SlingshotStretchingFinished = true;
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