#include "BasicModules/Game.h"
#include "Scenes/GameScene.h"
#include "BasicModules/Physics/Colliders/PolygonCollider.h"
#include <iostream>

/*
struct AllocationMetrics
{
	uint32_t TotalAllocated = 0;
	uint32_t TotalFreed = 0;

	uint32_t CurrentUsage() { return TotalAllocated - TotalFreed; }
};

static AllocationMetrics s_AllocationMetrics;

static void PrintMemoryUsage() { std::cout << "Total memory usage: " << s_AllocationMetrics.CurrentUsage() << "bytes\n"; }

void* operator new(size_t size)
{
	s_AllocationMetrics.TotalAllocated += size;

	PrintMemoryUsage();

	return malloc(size);
}

void operator delete(void* memory, size_t size)
{
	s_AllocationMetrics.TotalFreed += size;

	PrintMemoryUsage();

	free(memory);
}
*/



static Basic::PolygonCollider CreateRandomPolygon(std::vector<sf::Vector2f>& vertices)
{
	int n = vertices.size();

	vertices[0] = sf::Vector2f(rand() % 100 * 1.0f, rand() % 100 * 1.0f);
	vertices[1] = sf::Vector2f(100.0f + rand() % 100 * 1.0f, rand() % 100 * 1.0f);
	vertices[2] = sf::Vector2f(150.0f + rand() % 100 * 1.0f, 100.0f + rand() % 100 * 1.0f);
	vertices[3] = sf::Vector2f(250.0f + rand() % 100 * 1.0f, 100.0f + rand() % 100 * 1.0f);
	vertices[4] = sf::Vector2f( 300.0f + rand() % 100 * 1.0f, 200.0f + rand() % 100 * 1.0f);
	vertices[5] = sf::Vector2f(rand() % 100 * 1.0f, 200.0f + rand() % 100 * 1.0f);

	return Basic::PolygonCollider({ vertices[0], vertices[1], vertices[2], vertices[3], vertices[4], vertices[5] });
}

int main()
{
	/*
	// set start scene
	Basic::Game::SetScene(new GameScene());

	// run game
	Basic::Game::Run();
	*/

	std::vector<sf::Vector2f> vertices;
	vertices.resize(6);

	sf::RenderWindow window(sf::VideoMode(400, 400), "SFML works!");
	sf::VertexArray vertexArr(sf::PrimitiveType::Triangles);
	vertexArr.resize(4 * 3);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::KeyPressed)
			{
				if (event.key.code == sf::Keyboard::Space)
				{
					Basic::PolygonCollider coll = CreateRandomPolygon(vertices);

					int i = 0;
					for (auto& it : coll.Polygons())
					{
						for (int j = 0; j < 4 * 3; j++)
						{
							std::cout << "halo\n";
							vertexArr[i].position = vertices[i];
							vertexArr[i].color = sf::Color::Green;
							i++;
						}
					}



				}
			}
		}

		window.clear();
		window.draw(vertexArr);
		window.display();
	}

	return 0;
}