#include "Animator.h"

void Basic::Animator::NextFrame()
{
	auto& currAnim = m_AnimationsMap[m_CurrentAnimationName];

	auto& startPos = currAnim.FirstFramePos;
	auto& size = currAnim.FrameSize;

	int tableX = m_TextureSize.x / size.x;

	int startX = startPos.x / size.x;
	int startY = startPos.y / size.y;

	int rectX = m_CurrentFrameRect.left / size.x;
	int rectY = m_CurrentFrameRect.top / size.y;

	int framesPlayed;

	// if rect and start are in the same line
	if (rectY == startY)
	{
		framesPlayed = rectX - startX + 1;
	}
	else
	{
		framesPlayed = (tableX - startX) + (rectX + 1)
			+ (rectY - startY - 1) * tableX;
	}

	// if animation is finished go to the first 
	if (framesPlayed == currAnim.FramesCount)
	{
		m_CurrentFrameRect = sf::IntRect(startPos.x, startPos.y, size.x, size.y);
	}
	else
	{
		// if we are on the last frame in the row
		if (rectX == tableX - 1)
		{
			m_CurrentFrameRect = sf::IntRect(0, m_CurrentFrameRect.top + size.y, 
				size.x, size.y);
		}
		else
		{
			m_CurrentFrameRect = sf::IntRect(m_CurrentFrameRect.left + size.x, 
				m_CurrentFrameRect.top,
				size.x, size.y);
		}
	}
}

void Basic::Animator::TempNextFrame()
{
	auto& tempAnim = m_AnimationsMap[m_TempAnimationName];

	auto& startPos = tempAnim.FirstFramePos;
	auto& size = tempAnim.FrameSize;

	int tableX = m_TextureSize.x / size.x;

	int startX = startPos.x / size.x;
	int startY = startPos.y / size.y;

	int rectX = m_CurrentFrameRect.left / size.x;
	int rectY = m_CurrentFrameRect.top / size.y;

	int framesPlayed;

	// if rect and start are in the same row
	if (rectY == startY)
	{
		framesPlayed = rectX - startX + 1;
	}
	else
	{
		framesPlayed = (tableX - startX) + (rectX + 1)
			+ (rectY - startY - 1) * tableX;
	}

	// if temporary animation is finished
	if (framesPlayed == tempAnim.FramesCount)
	{
		// turn off temporary animation
		m_TempAnimationIsPlaying = false;

		// return rectangle to current animation
		auto& pos = m_AnimationsMap[m_CurrentAnimationName].FirstFramePos;
		auto& currSize = m_AnimationsMap[m_CurrentAnimationName].FrameSize;

		m_CurrentFrameRect = sf::IntRect(pos.x, pos.y, currSize.x, currSize.y);
	}
	else
	{
		// if we are on the last frame in the row
		if (rectX == tableX - 1)
		{
			m_CurrentFrameRect = sf::IntRect(0, m_CurrentFrameRect.top + size.y,
				size.x, size.y);
		}
		else
		{
			m_CurrentFrameRect = sf::IntRect(m_CurrentFrameRect.left + size.x,
				m_CurrentFrameRect.top,
				size.x, size.y);
		}
	}
}

void Basic::Animator::AddAnimation(const char* name, Animation&& anim)
{
	// add animation
	m_AnimationsMap[name] = anim;

	// if it is the first animation added, make it default
	if (m_AnimationsMap.size() == 1)
	{
		Play(name);
	}
}

void Basic::Animator::Play(const char* animationName)
{
	// if this animation is playing don't do anything
	if (animationName == m_CurrentAnimationName)
		return;

	// reset timer
	m_Timer = 0.0f;

	// set current animation name
	m_CurrentAnimationName = animationName;

	if (!m_TempAnimationIsPlaying)
	{
		// set position of the rectangle if there is no temp animation playing
		auto& pos = m_AnimationsMap[m_CurrentAnimationName].FirstFramePos;
		auto& size = m_AnimationsMap[m_CurrentAnimationName].FrameSize;

		m_CurrentFrameRect = sf::IntRect(pos.x, pos.y, size.x, size.y);
	}
}

void Basic::Animator::PlayOnce(const char* animationName)
{
	// if this temporary animation is playing don't do anything
	if (m_TempAnimationIsPlaying && animationName == m_TempAnimationName)
		return;

	// reset timer
	m_Timer = 0.0f;

	// prepare temporary animation modules
	m_TempAnimationIsPlaying = true;

	// set temporary animation name
	m_TempAnimationName = animationName;

	// set position of the rectangle
	auto& pos = m_AnimationsMap[m_TempAnimationName].FirstFramePos;
	auto& size = m_AnimationsMap[m_TempAnimationName].FrameSize;

	m_CurrentFrameRect = sf::IntRect(pos.x, pos.y, size.x, size.y);
}

void Basic::Animator::UpdateAnimations(const sf::Time& deltaTime)
{
	// do not update animator in case it is stopped
	if (m_AnimatorStopped)
		return;

	// update timer
	m_Timer += deltaTime.asSeconds();

	if (m_TempAnimationIsPlaying)
	{
		auto& tempAnim = m_AnimationsMap[m_TempAnimationName];

		// if frame time is up
		if (m_Timer >= tempAnim.FrameDuration)
		{
			m_Timer = 0.0f;
			TempNextFrame();
		}
	}
	else
	{
		auto& currAnim = m_AnimationsMap[m_CurrentAnimationName];

		// if frame time is up
		if (m_Timer >= currAnim.FrameDuration)
		{
			m_Timer = 0.0f;
			NextFrame();
		}
	}
}

void Basic::Animator::Stop()
{
	m_AnimatorStopped = true;
}

void Basic::Animator::Resume()
{
	m_AnimatorStopped = false;
}

const sf::IntRect& Basic::Animator::GetCurrentFrameRect() const
{
	return m_CurrentFrameRect;
}
