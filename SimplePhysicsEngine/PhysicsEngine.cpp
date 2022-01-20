#include "PhysicsEngine.h"
#include <Windows.h>

using namespace std;

namespace SimplePhysicsEngine
{
    vector<CollisionPoints> collisionInfos;
    std::vector<Collisions> collisions;

    void PhysicsEngine::updatePhysics(float dt) {
        //collision detection (test)
        sBufferLock.lock();
        PreDetectCollision();
        SecondDetectCollision();
        
        //resolve collisions
        if (collisionInfos.size())
        {
            PositionSolver(dt);
            ImpulseSolver(dt);
        }
        
        //adjust gravity        
        for (auto i=0; i < simulateBuffer.size(); i+=1)
        {            
            auto& rb = simulateBuffer[i].rigidBody;
            auto& tf = simulateBuffer[i].transform;
            if (rb.isKinematic) continue;
            rb.forces += defaultGravity;                        
            rb.velocity += rb.forces / rb.mass * dt;
            tf.position += rb.velocity * dt;
            
            rb.forces.x = 0;
            rb.forces.y = 0;
            rb.forces.z = 0;
        }   

        //update latestBuffer
        lBufferLock.lock();
        for (auto i = 0; i < latestBuffer.size(); ++i)
        {
            auto& rb = simulateBuffer[i].rigidBody;
            auto& tf = simulateBuffer[i].transform;

            latestBuffer[i]->UpdatePhysics(rb.velocity, rb.forces);
            latestBuffer[i]->UpdateTranform(tf.position, tf.rotation);
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

    void PhysicsEngine::PreDetectCollision()
    {
        vector<Collisions>().swap(collisions); // clear

        for (auto i = 0; i < simulateBuffer.size(); ++i)
        {
            for (auto j = i + 1; j < simulateBuffer.size(); ++j)
            {
                AABB worldPosAABB1 = simulateBuffer[i].aabb + simulateBuffer[i].transform.position;
                AABB worldPosAABB2 = simulateBuffer[j].aabb + simulateBuffer[j].transform.position;

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
        for (auto i = 0; i < collisions.size(); ++i)
        {
            if (GJK(&simulateBuffer[collisions[i].aInd].collider, simulateBuffer[collisions[i].aInd].transform.position,
                &simulateBuffer[collisions[i].bInd].collider, simulateBuffer[collisions[i].bInd].transform.position))
            {
                auto& colData = collisionInfos.back();
                colData.aInd = collisions[i].aInd;
                colData.bInd = collisions[i].bInd;
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
        return SimplePhysicsEngine::PhysicsData(*origin.transform, *origin.rigidBody, *origin.aabb, *origin.collider);
    }    

    bool PhysicsEngine::GJK(const MeshCollider* colliderA, utils::Vector3 posA , const MeshCollider* colliderB, utils::Vector3 posB)
    {
        vector<CollisionPoints>().swap(collisionInfos);

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
                return false;
            }
            points.push(support);
            
            if (NextSimplex(points, dir))
            {
                auto collisionData = EPA(points, &wposColliderA, &wposColliderB);                
                collisionInfos.emplace_back(collisionData);                
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


    CollisionPoints PhysicsEngine::EPA(const Simplex& simplex, const MeshCollider* colliderA, const MeshCollider* colliderB)
    {
        std::vector<utils::Vector3> polytope(simplex.begin(), simplex.end());
        std::vector<size_t> faces =
        {
            0,1,2,
            0,3,1,
            0,2,3,
            1,3,2
        }; //사면체 각 면의 polygon draw 순서

        auto faceNormalData = GetFaceNormals(polytope, faces);        
        auto normals = faceNormalData.first;
        auto minFace = faceNormalData.second;

        utils::Vector3 minNormal;
        float minDistance = FLT_MAX;

        size_t iterations = 0;
        while (minDistance == FLT_MAX)
        {
            minNormal = normals[minFace].xyz();
            minDistance = normals[minFace].w;

            if (iterations++ > 32)
            {
                break;
            }

            utils::Vector3 support = Support(colliderA, colliderB, minNormal);
            float sDist = utils::Vector3::DotProduct(minNormal, support);

            if (abs(sDist - minDistance) > 0.001f)
            {
                minDistance = FLT_MAX;
                std::vector<std::pair<size_t, size_t>> uniqueEdges;

                for (size_t i = 0; i < normals.size(); ++i)
                {
                    if (SameDirection(normals[i], support))
                    {
                        size_t f = i * 3;

                        AddIfUniqueEdge(uniqueEdges, faces, f, f + 1);
                        AddIfUniqueEdge(uniqueEdges, faces, f + 1, f + 2);
                        AddIfUniqueEdge(uniqueEdges, faces, f + 2, f);

                        faces[f + 2] = faces.back(); faces.pop_back();
                        faces[f + 1] = faces.back(); faces.pop_back();
                        faces[f] = faces.back(); faces.pop_back();

                        normals[i] = normals.back(); normals.pop_back();

                        i--;
                    }
                }

                if (uniqueEdges.size() == 0) 
                {
                    break;
                }

                std::vector<size_t> newFaces;
                for (auto& uEdgeData : uniqueEdges)
                {
                    newFaces.push_back(uEdgeData.first);
                    newFaces.push_back(uEdgeData.second);
                    newFaces.push_back(polytope.size());
                }
                polytope.push_back(support);

                auto pfNormalData = GetFaceNormals(polytope, newFaces);
                auto newNormals = pfNormalData.first;
                auto newMinFace = pfNormalData.second;

                float oldMinDist = FLT_MAX;
                for (size_t i = 0; i < normals.size(); i += 1)
                {
                    if (normals[i].w < oldMinDist)
                    {
                        oldMinDist = normals[i].w;
                        minFace = i;
                    }
                }

                if (newNormals[newMinFace].w < oldMinDist)
                {
                    minFace = newMinFace + normals.size();
                }

                faces.insert(faces.end(), newFaces.begin(), newFaces.end());
                normals.insert(normals.end(), newNormals.begin(), newNormals.end());
            }
        }

        CollisionPoints points;

        points.normal = minNormal;
        points.depth = minDistance + 0.001f;
        points.hasCollision = true;

        return points;
    }

    std::pair<std::vector<utils::Vector4>, size_t> PhysicsEngine::GetFaceNormals(const std::vector<utils::Vector3>& polytope, const std::vector<size_t>& faces)
    {
        std::vector<utils::Vector4> normals;
        size_t minTriangle = 0;
        float minDistance = FLT_MAX;

        for (size_t i = 0; i < faces.size(); i += 3)
        {
            auto a = polytope[faces[i]];
            auto b = polytope[faces[i+1]];
            auto c = polytope[faces[i+2]];

            auto cross = utils::Vector3::CrossProduct((b - a), (c - a));
            auto normal = utils::Vector3::Normalize(cross);
            float dist = utils::Vector3::DotProduct(normal, a);

            if (dist < 0)
            {
                normal *= -1;
                dist *= -1;
            }

            auto normalWithDist = utils::Vector4();
            normalWithDist.x = normal.x;
            normalWithDist.y = normal.y;
            normalWithDist.z = normal.z;
            normalWithDist.w = dist;
            normals.emplace_back(normalWithDist);

            if (dist < minDistance)
            {
                minTriangle = i / 3;
                minDistance = dist;
            }
        }

        return { normals, minTriangle };
    }

    void PhysicsEngine::AddIfUniqueEdge(std::vector <std::pair<size_t, size_t>>& edges, const std::vector<size_t>& faces, size_t a, size_t b)
    {
        auto rev = std::find
        (edges.begin(), edges.end(), 
         std::make_pair(faces[b], faces[a]));

        if (rev != edges.end())
        {
            edges.erase(rev);
        }
        else
        {
            edges.emplace_back(faces[a], faces[b]);
        }
    }

    void PhysicsEngine::ImpulseSolver(float dt)
    {
        for (auto& cInfo : collisionInfos)
        {
            PhysicsData& aPhysicsData = simulateBuffer[cInfo.aInd];
            PhysicsData& bPhysicsData = simulateBuffer[cInfo.bInd];
            //Simple Model

            bool isAKinematic = aPhysicsData.rigidBody.isKinematic;
            bool isBKinematic = bPhysicsData.rigidBody.isKinematic;

            auto aVelocity = isAKinematic ? utils::Vector3(0, 0, 0) : aPhysicsData.rigidBody.velocity;
            auto bVelocity = isBKinematic ? utils::Vector3(0, 0, 0) : bPhysicsData.rigidBody.velocity;
            auto rVelocity = bVelocity - aVelocity;
            float spd = utils::Vector3::DotProduct(rVelocity, cInfo.normal);

            float aInvMass = isAKinematic ? 1.0f : 1.0f / aPhysicsData.rigidBody.mass;
            float bInvMass = isBKinematic ? 1.0f : 1.0f / bPhysicsData.rigidBody.mass;

            if (spd >= 0)
                continue;

            float e = aPhysicsData.rigidBody.bounciness * bPhysicsData.rigidBody.bounciness;
            float j = -(1.0f + e) * spd / (aInvMass + bInvMass);

            utils::Vector3 impulse = cInfo.normal * j;

            if (!isAKinematic)
            {
                aVelocity -= impulse * aInvMass;
            }

            if (!isBKinematic)
            {
                bVelocity += impulse * bInvMass;
            }

            //friction
            rVelocity = bVelocity - aVelocity;
            spd = utils::Vector3::DotProduct(rVelocity, cInfo.normal);

            utils::Vector3 tan = rVelocity - (cInfo.normal*spd);

            if (tan.GetMagnitude() > 0.0001f)
            {
                tan = utils::Vector3::Normalize(tan);
            }

            float fVelocity = utils::Vector3::DotProduct(rVelocity, tan);

            float aStaticFric = aPhysicsData.rigidBody.staticFriction;
            float bStaticFric = bPhysicsData.rigidBody.staticFriction;
            float aDynamicFric = aPhysicsData.rigidBody.dynamicFriction;
            float bDynamicFric = bPhysicsData.rigidBody.dynamicFriction;
            float m = utils::Vector3(aStaticFric, bStaticFric, 0).GetMagnitude();

            float fric = -fVelocity / (aInvMass + bInvMass);

            utils::Vector3 friction;
            if (abs(fric) < j * m)
            {
                friction = tan * fric;
            }
            else
            {
                m = utils::Vector3(aDynamicFric, bDynamicFric, 0).GetMagnitude();
                friction = tan * -j * m;
            }
            //friction


            if (!isAKinematic)
            {
                aPhysicsData.rigidBody.velocity = aVelocity - friction*aInvMass;
            }

            if (!isBKinematic)
            {
                bPhysicsData.rigidBody.velocity = bVelocity + friction*bInvMass;
            }
        }
    }

    void PhysicsEngine::PositionSolver(float dt)
    {
        for (auto& cInfo : collisionInfos)
        {
            PhysicsData& aPhysicsData = simulateBuffer[cInfo.aInd];
            PhysicsData& bPhysicsData = simulateBuffer[cInfo.bInd];

            utils::Vector3 resolution = cInfo.normal * cInfo.depth;

            if (!aPhysicsData.rigidBody.isKinematic)
                aPhysicsData.transform.position -= resolution;
            if (!bPhysicsData.rigidBody.isKinematic)
                bPhysicsData.transform.position += resolution;
        }
    }
}