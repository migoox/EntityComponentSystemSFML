#pragma once

struct Ball
{
	bool Selected = false;
	bool DoublySelected = false;

	bool FitMousePosition = false;

	bool SlingshotStretchingStarted = false;
	bool SlingshotStretchingHappens = false;
	bool SlingshotStretchingFinished = false;
};