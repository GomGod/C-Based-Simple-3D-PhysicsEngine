#ifndef SIMPLEPHYSICSENGINE_PHYSICSWORLD_H
#define SIMPLEPHYSICSENGINE_PHYSICSWORLD_H
#include <algorithm>
#include <thread>
#include <iostream>
#include <chrono>
#include <queue>
#include"Object.h"

namespace SimplePhysicsEngine
{
    struct PhysicsData
    {
        utils::Vector3 position;
        utils::Vector3 rotation;
        utils::Vector3 velocity;
        utils::Vector3 forces;
        float mass;
        bool ignore;

        PhysicsData(utils::Vector3 position, utils::Vector3 rotation, utils::Vector3 velocity, utils::Vector3 forces, float mass, bool ignore)
            :position(position), rotation(rotation), velocity(velocity), forces(forces), mass(mass), ignore(ignore)
        {}        
    };

    class PhysicsEngine 
    {
    private:
        utils::Vector3 defaultGravity{ 0,-0.098f,0 };
        std::thread physicsThread;
                
        std::vector<PhysicsData> simulateBuffer;
        std::vector<Object*> latestBuffer;

        std::vector<Object*> waitingObjects;
        std::vector<int> waitingRemoveTargets;
        
        int nextID=0;

        [[noreturn]] void threadPhysicsUpdate();        
        void checkCollision();
        void adjustConstraints();
        void updateLatestBuffer();

        void addObjectsAtWaitingQueue();
        void removeObjectsAtWaitingQueue();

    public:     
        static PhysicsData PhysicsCopy(const Object& origin);

        void runPhysicsThread();
        void addObject(Object* obj);
        void removeObject(int ID);
        void updatePhysics(float dt);

        int getObjectsNumber() { return latestBuffer.size(); };
        std::vector<Object*> getLatestBuffer() { return latestBuffer; };
    };
}

#endif //SIMPLEPHYSICSENGINE_PHYSICSWORLD_H
