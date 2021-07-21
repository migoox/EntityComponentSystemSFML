#pragma once
#include <map>

#include "Animation.h"

namespace Basic {
	class Animator
	{
	private:
		sf::Vector2u m_TextureSize;
		std::map<std::string, Animation> m_AnimationsMap;

		std::string m_CurrentAnimationName;
		std::string m_TempAnimationName;

		bool m_TempAnimationIsPlaying = false;
		bool m_AnimatorStopped = false;

		float m_Timer = 0.0f;

		sf::IntRect m_CurrentFrameRect;

	private:
		void NextFrame();

		void TempNextFrame();

	public:
		// force programmer to use 
		Animator() {};
		Animator(sf::Vector2u textureSize)
			: m_TextureSize(textureSize) { };

		void AddAnimation(const char* name, Animation&& anim);

		void Play(const char* animationName);
		void PlayOnce(const char* animationName);

		void UpdateAnimations(const sf::Time& deltaTime);

		void Stop();

		void Resume();

		const sf::IntRect& GetCurrentFrameRect() const;
	};
}
