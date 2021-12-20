#include "PhysicsWorld.h"

using namespace std;

void PhysicsWorld::UpdateForces(float dt) {
    //adjust gravity
    for (auto obj : _objects)
    {
        obj->Forces += defaultGravity;
        obj->Velocity += obj->Forces / obj->Mass * dt;
        obj->Position += obj->Velocity * dt;

        obj->Forces = utils::Vector3{ 0,0,0 };
    }
}

void PhysicsWorld::CheckCollision()
{
}

void PhysicsWorld::AdjustConstraints()
{
}

void PhysicsWorld::AddObject(utils::Object* obj) {
    _objects.push_back(obj);
}

void PhysicsWorld::RemoveObject(utils::Object* obj) {
    _objects.erase(remove_if(_objects.begin(), _objects.end(), obj), _objects.end());
}

void PhysicsWorld::InitPhysics()
{
    
}

[[noreturn]] void PhysicsWorld::threadPhysicsUpdate()
{
    float dt = 0.3f;
    while (true)
    {
        UpdateForces(dt);
        this_thread::sleep_for(chrono::milliseconds(10));
    }
}