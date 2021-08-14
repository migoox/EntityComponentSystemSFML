#include "VisualGizmos.h"

Basic::VisualGizmos Basic::VisualGizmos::s_Instance;

Basic::VisualGizmos::VisualGizmos()
{
	m_Lines.reserve(512);
	m_CircleShapes.reserve(512);
	m_VertexArrays.reserve(512);
	m_RectangleShapes.reserve(512);

	m_TempLines.reserve(512);
	m_TempCircleShapes.reserve(512);
	m_TempVertexArrays.reserve(512);
	m_TempRectangleShapes.reserve(512);
}

void Basic::VisualGizmos::IDraw(Line line)
{
	m_Lines.push_back(line);
}

void Basic::VisualGizmos::IDraw(CircleShape circle)
{
	m_CircleShapes.push_back(circle);
}

void Basic::VisualGizmos::IDraw(VertexArray verArr)
{
	m_VertexArrays.push_back(verArr);
}

void Basic::VisualGizmos::IDraw(RectangleShape rect)
{
	m_RectangleShapes.push_back(rect);
}

void Basic::VisualGizmos::IDrawOnce(Line line)
{
	m_TempLines.push_back(line);
}

void Basic::VisualGizmos::IDrawOnce(CircleShape circle)
{
	m_CircleShapes.push_back(circle);
}

void Basic::VisualGizmos::IDrawOnce(VertexArray verArr)
{
	m_VertexArrays.push_back(verArr);
}

void Basic::VisualGizmos::IDrawOnce(RectangleShape rect)
{
	m_RectangleShapes.push_back(rect);
}

void Basic::VisualGizmos::IClearLines()
{
	m_Lines.clear();
}

void Basic::VisualGizmos::IClearCircleShapes()
{
	m_CircleShapes.clear();
}

void Basic::VisualGizmos::IClearVertexArrays()
{
	m_VertexArrays.clear();
}

void Basic::VisualGizmos::IClearRectangleShapes()
{
	m_RectangleShapes.clear();
}

void Basic::VisualGizmos::IClear()
{
	m_Lines.clear();
	m_CircleShapes.clear();
	m_VertexArrays.clear();
	m_RectangleShapes.clear();
}

void Basic::VisualGizmos::IRender(sf::RenderTarget& target)
{
	for (auto& it : m_Lines)
		target.draw(it);
	for (auto& it : m_CircleShapes)
		target.draw(it);
	for (auto& it : m_VertexArrays)
		target.draw(it);
	for (auto& it : m_RectangleShapes)
		target.draw(it);

	for (auto& it : m_TempLines)
		target.draw(it);
	for (auto& it : m_TempCircleShapes)
		target.draw(it);
	for (auto& it : m_TempVertexArrays)
		target.draw(it);
	for (auto& it : m_TempRectangleShapes)
		target.draw(it);

	m_TempLines.clear();
	m_TempCircleShapes.clear();
	m_TempVertexArrays.clear();
	m_TempRectangleShapes.clear();
}

void Basic::VisualGizmos::Draw(Line line)
{
	s_Instance.IDraw(line);
}

void Basic::VisualGizmos::Draw(CircleShape circle)
{
	s_Instance.IDraw(circle);
}

void Basic::VisualGizmos::Draw(VertexArray verArr)
{
	s_Instance.IDraw(verArr);
}

void Basic::VisualGizmos::Draw(RectangleShape rect)
{
	s_Instance.IDraw(rect);
}

void Basic::VisualGizmos::DrawOnce(Line line)
{
	s_Instance.IDrawOnce(line);
}

void Basic::VisualGizmos::DrawOnce(CircleShape circle)
{
	s_Instance.IDrawOnce(circle);
}

void Basic::VisualGizmos::DrawOnce(VertexArray verArr)
{
	s_Instance.IDrawOnce(verArr);
}

void Basic::VisualGizmos::DrawOnce(RectangleShape rect)
{
	s_Instance.IDrawOnce(rect);
}

void Basic::VisualGizmos::ClearLines()
{
	s_Instance.IClearLines();
}

void Basic::VisualGizmos::ClearCircleShapes()
{
	s_Instance.IClearCircleShapes();
}

void Basic::VisualGizmos::ClearVertexArrays()
{
	s_Instance.IClearVertexArrays();
}

void Basic::VisualGizmos::ClearRectangleShapes()
{
	s_Instance.IClearRectangleShapes();
}

void Basic::VisualGizmos::Clear()
{
	s_Instance.IClear();
}

void Basic::VisualGizmos::Render(sf::RenderTarget& target)
{
	s_Instance.IRender(target);
}
