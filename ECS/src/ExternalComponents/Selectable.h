#pragma once

struct Selectable
{
	bool Selected;
	bool DoublySelected;

	bool FitMousePosition;

	bool SlingshotStretchingStarted;
	bool SlingshotStretchingHappens;
	bool SlingshotStretchingFinished;
};