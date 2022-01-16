#ifndef SIMPLEPHYSICSENGINE_PHYSICSWORLD_H
#define SIMPLEPHYSICSENGINE_PHYSICSWORLD_H
#include <algorithm>
#include <thread>
#include <iostream>
#include <chrono>
#include <queue>
#include <mutex>
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
        bool isKinematic;

        PhysicsData(utils::Vector3 position, utils::Vector3 rotation, utils::Vector3 velocity, utils::Vector3 forces, float mass, bool isKinematic)
            :position(position), rotation(rotation), velocity(velocity), forces(forces), mass(mass), isKinematic(isKinematic)
        {}        
    };

    class PhysicsEngine 
    {
    private:
        utils::Vector3 defaultGravity{ 0,-0.098f,0 };
        std::thread physicsThread;
                
//critical section
        std::vector<PhysicsData> simulateBuffer;
        std::vector<Object*> latestBuffer;
        std::vector<Object*> waitingObjects;
        std::vector<int> waitingRemoveTargets;

        int nextID=0;

        std::mutex sBufferLock;
        std::mutex lBufferLock;
        std::mutex woBufferLock;
        std::mutex rtBufferLock;

        [[noreturn]] void threadPhysicsUpdate();        

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
