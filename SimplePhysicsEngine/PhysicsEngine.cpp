#include "PhysicsEngine.h"
#include <Windows.h>

using namespace std;

namespace SimplePhysicsEngine
{
    void PhysicsEngine::updatePhysics(float dt) {
        //adjust gravity
        sBufferLock.lock();
        for (auto i=0; i < simulateBuffer.size(); i+=1)
        {            
            if (simulateBuffer[i].isKinematic) continue;
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
            latestBuffer[i]->UpdatePhysics(simulateBuffer[i].velocity, simulateBuffer[i].forces);
            latestBuffer[i]->UpdateTranform(simulateBuffer[i].position, simulateBuffer[i].rotation);
        }

        lBufferLock.unlock();


        //collision detection (test)
        PreDetectCollision();
        SecondDetectCollision();

        sBufferLock.unlock();       
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

    std::vector<Collisions> collisions;
    std::vector<Collisions> finalResultCollisions;

    void PhysicsEngine::PreDetectCollision()
    {
        vector<Collisions>().swap(collisions); // clear

        for (auto i = 0; i < simulateBuffer.size(); ++i)
        {
            for (auto j = i + 1; j < simulateBuffer.size(); ++j)
            {
                AABB worldPosAABB1 = simulateBuffer[i].aabb + simulateBuffer[i].position;
                AABB worldPosAABB2 = simulateBuffer[j].aabb + simulateBuffer[j].position;

                if (worldPosAABB1.TestAABBCollision(worldPosAABB2))
                {  
                    collisions.push_back(Collisions{ i, j });
                }
            }
        }
        auto precolCnt = collisions.size();
    }

    void PhysicsEngine::SecondDetectCollision()
    {
        vector<Collisions>().swap(finalResultCollisions);        
        for (auto i=0; i<collisions.size(); ++i)
        {
            if (GJK(&simulateBuffer[collisions[i].aInd].collider, simulateBuffer[collisions[i].aInd].position,  
                &simulateBuffer[collisions[i].bInd].collider, simulateBuffer[collisions[i].bInd].position))
            {
                finalResultCollisions.push_back(collisions[i]);                
            }
        }
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
        const Transform* tf{ origin.transform };
        const RigidBody* rb{ origin.rigidBody };

        return SimplePhysicsEngine::PhysicsData(tf->position, tf->rotation, rb->velocity, rb->forces, MeshCollider(*origin.collider), AABB(*origin.aabb), rb->mass, rb->isKinematic);
    }

    bool PhysicsEngine::GJK(const MeshCollider* colliderA, utils::Vector3 posA , const MeshCollider* colliderB, utils::Vector3 posB)
    {
        utils::Vector3 dir = utils::Vector3{ 1,0,0 };
                
        auto wposColliderA = *colliderA + posA;
        auto wposColliderB = *colliderB + posB;

        //initial support pnt
        utils::Vector3 support = Support(&wposColliderA, &wposColliderB, dir);      
        Simplex points;
        points.push(support);

        //New direction is towards the origin
        dir = -support;
        while (true)
        {
            support = Support(&wposColliderA, &wposColliderB, dir);                        
            if (utils::Vector3::DotProduct(support, dir) <= 0)
            {
                cout << "aabb paseed but does not collide.\n";
                return false;
            }
            points.push(support);
            
            if (NextSimplex(points, dir))
            {               
                cout << "collision!\n";
                return true;
            }
        }
    }

    utils::Vector3 PhysicsEngine::Support(const MeshCollider* colliderA, const MeshCollider* colliderB, utils::Vector3 dir)
    {
        return colliderA->FindFurthestPoint(dir) - colliderB->FindFurthestPoint(-dir);
    }

    bool PhysicsEngine::NextSimplex(Simplex& points, utils::Vector3& dir)
    {
        switch (points.size())
        {
        case 2: return Line(points, dir);
        case 3: return Triangle(points, dir);
        case 4: return Tetrahedron(points, dir);
        }
        return false;
    }

    bool PhysicsEngine::SameDirection(const utils::Vector3& dir, const utils::Vector3 ao)
    {
        return utils::Vector3::DotProduct(dir, ao) > 0;
    }

    bool PhysicsEngine::Line(Simplex& points, utils::Vector3& dir)
    {
        auto a = points[0];
        auto b = points[1];

        auto ab = b - a;
        auto ao = -a;

        if (SameDirection(ab, ao))
        {            
            dir = utils::Vector3::CrossProduct(ab, ao);
            dir = utils::Vector3::CrossProduct(dir, ab);
        }
        else
        {
            points = { a };
            dir = ao;
        }

        return false;
    }

    bool PhysicsEngine::Triangle(Simplex& points, utils::Vector3& dir)
    {
        utils::Vector3 a = points[0];
        utils::Vector3 b = points[1];
        utils::Vector3 c = points[2];

        utils::Vector3 ab = b - a;
        utils::Vector3 ac = c - a;
        utils::Vector3 ao = -a;

        utils::Vector3 abc = utils::Vector3::CrossProduct(ab, ac);

        if (SameDirection(utils::Vector3::CrossProduct(abc, ac), ao))
        {
            if (SameDirection(ac, ao))
            {
                points = { a,c };
                dir = utils::Vector3::CrossProduct(ac, ao);
                dir = utils::Vector3::CrossProduct(dir, ac);
            }
            else
            {
                return Line(points = { a,b }, dir);
            }            
        }
        else
        {
            if (SameDirection(utils::Vector3::CrossProduct(ab, abc), ao))
            {
                return Line(points = { a,b }, dir);
            }
            else
            {
                if (SameDirection(abc, ao))
                {
                    dir = abc;
                }
                else
                {
                    points = { a,c,b };
                    dir = -abc;
                }
            }

        }

        return false;        
    }

    bool PhysicsEngine::Tetrahedron(Simplex& points, utils::Vector3& dir)
    {
        utils::Vector3 a = points[0];
        utils::Vector3 b = points[1];
        utils::Vector3 c = points[2];
        utils::Vector3 d = points[3];

        utils::Vector3 ab = b - a;
        utils::Vector3 ac = c - a;
        utils::Vector3 ad = d - a;
        utils::Vector3 ao = -a;

        utils::Vector3 abc = utils::Vector3::CrossProduct(ab, ac);
        utils::Vector3 acd = utils::Vector3::CrossProduct(ac, ad);
        utils::Vector3 adb = utils::Vector3::CrossProduct(ad, ab);

        if (SameDirection(abc, ao))
        {
            return Triangle(points = { a,b,c }, dir);
        }
        if (SameDirection(acd, ao))
        {
            return Triangle(points = { a,c,d }, dir);
        }
        if (SameDirection(adb, ao))
        {
            return Triangle(points = { a,d,b }, dir);
        }

        return true;
    }
}