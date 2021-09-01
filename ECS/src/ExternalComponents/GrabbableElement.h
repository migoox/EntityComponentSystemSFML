#pragma once

struct GrabbableElement
{
	bool Selected = false;
	bool DoublySelected = false;

	bool FitMousePosition = false;

	bool SlingshotStretchingStarted = false;
	bool SlingshotStretchingHappens = false;
	bool SlingshotStretchingFinished = false;

	sf::Color DeafultColor;
};