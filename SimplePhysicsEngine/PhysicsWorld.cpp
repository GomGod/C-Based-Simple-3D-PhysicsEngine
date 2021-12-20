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
    _objects.erase(remove(_objects.begin(), _objects.end(), obj), _objects.end());
}

int PhysicsWorld::GetObjectsNumber()
{
    return _objects.size();
}

void PhysicsWorld::InitPhysics()
{
    physicsThread = std::thread(&PhysicsWorld::threadPhysicsUpdate, this);
    physicsThread.detach();
}

[[noreturn]] void PhysicsWorld::threadPhysicsUpdate()
{    
    while (true)
    {
        UpdateForces(DELTA_TIME);
        this_thread::sleep_for(chrono::milliseconds(10));
    }
}