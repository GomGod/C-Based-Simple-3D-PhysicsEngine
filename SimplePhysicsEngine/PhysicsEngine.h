#ifndef SIMPLEPHYSICSENGINE_PHYSICSWORLD_H
#define SIMPLEPHYSICSENGINE_PHYSICSWORLD_H
#include <algorithm>
#include <thread>
#include <iostream>
#include <chrono>
#include <queue>
#include <mutex>
#include <utility>

#include "Vector4.h"
#include "Object.h"
#include "MeshCollider.h"
#include "Simplex.h"

namespace SimplePhysicsEngine
{   
    struct PhysicsData
    {
        Transform transform;
        RigidBody rigidBody;
        MeshCollider collider;

        PhysicsData(Transform transform, RigidBody rigidBody, MeshCollider collider)
            :transform(transform), rigidBody(rigidBody), collider(collider)
        {}        
    };

    struct Collisions
    {
        int aInd;
        int bInd;
    };

    struct CollisionPoints
    {
        int aInd;
        int bInd;
        utils::Vector3 normal;
        float depth;
        bool hasCollision;
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

        void AddObjectsAtWaitingQueue();
        void removeObjectsAtWaitingQueue();

        //test
        void PreDetectCollision();
        void SecondDetectCollision();

        bool GJK(const MeshCollider* colliderA, utils::Vector3 posA, utils::Vector3 rotA, const MeshCollider* colliderB, utils::Vector3 posB,utils::Vector3 rotB);
        utils::Vector3 Support(const MeshCollider* colliderA, const MeshCollider* colliderB, utils::Vector3 dir);
        bool NextSimplex(Simplex& points, utils::Vector3& dir);
        bool SameDirection(const utils::Vector3& dir, const utils::Vector3 ao);
        bool Line(Simplex& points, utils::Vector3& dir);
        bool Triangle(Simplex& points, utils::Vector3& dir);
        bool Tetrahedron(Simplex& points, utils::Vector3& dir);

        //EPA
        CollisionPoints EPA(const Simplex& simplex, const MeshCollider* colliderA, const MeshCollider* colliderB);
        std::pair<std::vector<utils::Vector4>, size_t> GetFaceNormals(const std::vector<utils::Vector3>& polytope, const std::vector<size_t>& faces);
        void AddIfUniqueEdge(std::vector <std::pair<size_t, size_t>>& edges, const std::vector<size_t>& faces, size_t a, size_t b);

        //Collision Solver
        void ImpulseSolver(float delta);
        void PositionSolver(float delta);

    public:           
        static PhysicsData PhysicsCopy(const Object& origin);

        void RunPhysicsThread();
        void AddObject(Object* obj);
        void RemoveObject(int ID);
        void UpdatePhysics(float dt);

        int GetObjectsNumber() { return latestBuffer.size(); };
        std::vector<Object*> GetLatestBuffer() { return latestBuffer; };
    };
}

#endif //SIMPLEPHYSICSENGINE_PHYSICSWORLD_H
