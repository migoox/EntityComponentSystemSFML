#pragma once
#include "SFML/Graphics.hpp"

namespace Basic {
	struct Animation
	{
		sf::Vector2u FirstFramePos;
		sf::Vector2u FrameSize;
		int FramesCount;
		float FrameDuration;

		Animation() {} 

		Animation(sf::Vector2u firstFramePos, sf::Vector2u frameSize, int framesCount, float frameDuration)
			: FirstFramePos(firstFramePos), FrameSize(frameSize), 
			FramesCount(framesCount), FrameDuration(frameDuration) { }
	};
}