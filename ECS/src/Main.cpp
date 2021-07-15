#include "Game.h"
#include "ECS/ComponentManager.h"
#include "ECS/EntityManager.h"
#include <iostream>

struct Position
{
    int x = 0, y = 0;
};

struct RigidBody2d
{
    float velocity;
    float acceleration;
};

int main()
{
    Entity e1 = EntityManager::Create();
    Entity e2 = EntityManager::Create();

    ComponentManager<Position>* positionComponentManager = new ComponentManager<Position>();
    positionComponentManager->AddComponent(e1, Position{ 1,2 });
    positionComponentManager->AddComponent(e2, Position{ 6,2 });

    ComponentManager<RigidBody2d>* rigidBody2dComponentManager = new ComponentManager<RigidBody2d>();
    rigidBody2dComponentManager->AddComponent(e2, RigidBody2d{ 55, 0 });



    return 0;
}