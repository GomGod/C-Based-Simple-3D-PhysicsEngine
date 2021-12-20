#ifndef SIMPLEPHYSICSENGINE_PHYSICSWORLD_H
#define SIMPLEPHYSICSENGINE_PHYSICSWORLD_H
#include <vector>
#include <algorithm>
#include <thread>
#include"Object.h"

class PhysicsWorld {
private:
    utils::Vector3 defaultGravity{ 0,-0.98f,0 };
    std::vector<utils::Object*> _objects;

    [[noreturn]] void threadPhysicsUpdate();
    void UpdateForces(float dt);
    void CheckCollision();
    void AdjustConstraints();

public:
    void InitPhysics();
    void AddObject(utils::Object* obj);
    void RemoveObject(utils::Object* obj);
};

#endif //SIMPLEPHYSICSENGINE_PHYSICSWORLD_H
