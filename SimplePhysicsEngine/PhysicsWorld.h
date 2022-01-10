#ifndef SIMPLEPHYSICSENGINE_PHYSICSWORLD_H
#define SIMPLEPHYSICSENGINE_PHYSICSWORLD_H
#include <vector>
#include <algorithm>
#include <thread>
#include <iostream>
#include"Object.h"

#define DELTA_TIME 0.1f

class PhysicsWorld {
private:
    utils::Vector3 defaultGravity{ 0,0, -0.98f };
    std::vector<Object*> _objects;
    std::thread physicsThread;

    [[noreturn]] void threadPhysicsUpdate();
    void UpdateForces(float dt);
    void CheckCollision();
    void AdjustConstraints();    

public:
    void InitPhysics();
    void AddObject(Object* obj);
    void RemoveObject(Object* obj);
    int GetObjectsNumber();
};

#endif //SIMPLEPHYSICSENGINE_PHYSICSWORLD_H
