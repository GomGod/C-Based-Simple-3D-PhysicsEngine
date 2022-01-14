#include "PhysicsEngine.h"

using namespace std;
namespace SimplePhysicsEngine
{
    void PhysicsEngine::updatePhysics(float dt) {
        //adjust gravity
        sBufferLock.lock();
        for (auto i=0; i < simulateBuffer.size(); i+=1)
        {            
            if (simulateBuffer[i].ignore) continue;
            simulateBuffer[i].forces += defaultGravity;                        
            simulateBuffer[i].velocity += simulateBuffer[i].forces / simulateBuffer[i].mass * dt;
            simulateBuffer[i].position += simulateBuffer[i].velocity * dt;
            simulateBuffer[i].forces.x = 0;
            simulateBuffer[i].forces.y = 0;
            simulateBuffer[i].forces.z = 0;
        }   

        lBufferLock.lock();
        for (auto i = 0; i < latestBuffer.size(); ++i)
        {
            latestBuffer[i]->PhysicsUpdate(simulateBuffer[i].position, simulateBuffer[i].rotation, simulateBuffer[i].velocity, simulateBuffer[i].forces);
        }
        sBufferLock.unlock();
        lBufferLock.unlock();
    }

    void PhysicsEngine::addObjectsAtWaitingQueue()
    {
        woBufferLock.lock();
        lBufferLock.lock();
        sBufferLock.lock();
        for (auto obj : waitingObjects)
        {
            obj->ID = nextID;
            ++nextID;
            latestBuffer.push_back(obj);
            simulateBuffer.push_back(PhysicsCopy(*obj));
        }        
        vector<Object*>().swap(waitingObjects);

        sBufferLock.unlock();
        lBufferLock.unlock();
        woBufferLock.unlock();
    }

    void PhysicsEngine::removeObjectsAtWaitingQueue()
    {
        rtBufferLock.lock();
        lBufferLock.lock();
        sBufferLock.lock();

        for (auto id : waitingRemoveTargets)
        {
            for (auto i = 0; i < latestBuffer.size(); ++i)
            {
                if (latestBuffer[i]->ID == id)
                {
                    auto obj = latestBuffer.begin() + i;
                    Object* objptr = *obj;

                    latestBuffer.erase(latestBuffer.begin() + i);
                    simulateBuffer.erase(simulateBuffer.begin() + i);
                    delete(objptr);
                    break;
                }
            }
        } 

        vector<int>().swap(waitingRemoveTargets);

        sBufferLock.unlock();
        lBufferLock.unlock();
        rtBufferLock.unlock();
    }

    void PhysicsEngine::addObject(Object* obj)     
    {
        woBufferLock.lock();
        waitingObjects.push_back(obj);
        woBufferLock.unlock();
    }

    void PhysicsEngine::removeObject(int ID)
    {        
        rtBufferLock.lock();
        waitingRemoveTargets.push_back(ID);
        rtBufferLock.unlock();
    }

    void PhysicsEngine::runPhysicsThread()
    {
        physicsThread = std::thread(&PhysicsEngine::threadPhysicsUpdate, this);
        physicsThread.detach();
    }

    [[noreturn]] void PhysicsEngine::threadPhysicsUpdate()
    {
        float dt = 0.0f;        
        clock_t lastfr = 0.0f;
        while (true)
        {
            clock_t currentfr = clock();
            dt = (float)(currentfr - lastfr) * 0.0001f;
            lastfr = currentfr;                        
            removeObjectsAtWaitingQueue();
            addObjectsAtWaitingQueue();
            updatePhysics(0.1f);
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    }

    PhysicsData PhysicsEngine::PhysicsCopy(const Object& origin)
    {
        utils::Vector3 pos(origin.position);
        utils::Vector3 rot(origin.rotation);
        utils::Vector3 vel(origin.velocity);
        utils::Vector3 force(origin.forces);
        float mass = origin.mass;
        bool usePhy = origin.usePhysics;
                
        return SimplePhysicsEngine::PhysicsData(pos, rot, vel, force, mass, !usePhy);
    }
}