#include "PhysicsEngine.h"
#include <Windows.h>

using namespace std;

namespace SimplePhysicsEngine
{
    vector<CollisionPoints> collisionInfos;
    std::vector<Collisions> collisions;

    void PhysicsEngine::UpdatePhysics(float dt) 
    {
        
        sBufferLock.lock();
                
        //adjust world forces 
        for (auto i=0; i < simulateBuffer.size(); i+=1)
        {            
            auto& rb = simulateBuffer[i].rigidBody;
            if (rb.isKinematic) continue;
            auto& tf = simulateBuffer[i].transform;

            rb.forces += defaultGravity *rb.mass;
            rb.velocity += rb.forces / rb.mass * dt;
            tf.position += rb.velocity * dt;
            
            rb.forces.x = 0;
            rb.forces.y = 0;
            rb.forces.z = 0;
        }           

        //collision detection
        vector<Collisions>().swap(collisions);//clear
        vector<CollisionPoints>().swap(collisionInfos);
        PreDetectCollision();
        SecondDetectCollision();

        //resolve collisions
        if (collisionInfos.size())
        {
            PositionSolver(dt);
            ImpulseSolver(dt);
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

    void PhysicsEngine::AddObjectsAtWaitingQueue()
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
        for (auto i = 0; i < simulateBuffer.size(); ++i)
        {
            for (auto j = i + 1; j < simulateBuffer.size(); ++j)
            {
                AABB worldPosAABB1 = simulateBuffer[i].collider.aabb.ComputeRotatedObjectAABB(simulateBuffer[i].transform.rotation) + simulateBuffer[i].transform.position;
                AABB worldPosAABB2 = simulateBuffer[j].collider.aabb.ComputeRotatedObjectAABB(simulateBuffer[j].transform.rotation) + simulateBuffer[j].transform.position;

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
        for (auto cInfo : collisions)
        {
            if (GJK(&simulateBuffer[cInfo.aInd].collider, simulateBuffer[cInfo.aInd].transform,
                &simulateBuffer[cInfo.bInd].collider, simulateBuffer[cInfo.bInd].transform))
            {                   
                auto& colData = collisionInfos.back();
                colData.aInd = cInfo.aInd;
                colData.bInd = cInfo.bInd;
            }
        }
    }

    void PhysicsEngine::AddObject(Object* obj)     
    {
        woBufferLock.lock();
        waitingObjects.push_back(obj);
        woBufferLock.unlock();
    }

    void PhysicsEngine::RemoveObject(int ID)
    {        
        rtBufferLock.lock();
        waitingRemoveTargets.push_back(ID);
        rtBufferLock.unlock();
    }

    void PhysicsEngine::RunPhysicsThread()
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
            dt = (float)(currentfr - lastfr) * 0.01f;
            lastfr = currentfr;   
            removeObjectsAtWaitingQueue();
            AddObjectsAtWaitingQueue();
            UpdatePhysics(dt);            
        }        
    }
        

    PhysicsData PhysicsEngine::PhysicsCopy(const Object& origin)
    {   
        return SimplePhysicsEngine::PhysicsData(*origin.transform, *origin.rigidBody, *origin.collider);
    }
    
    bool PhysicsEngine::GJK(const MeshCollider* colliderA, Transform tfColliderA, const MeshCollider* colliderB, Transform tfColliderB)
    {   
        //Transform Colliders
        MeshCollider wposColliderA = *colliderA;
        MeshCollider wposColliderB = *colliderB;
               
        glm::mat4 tfMatrix = glm::mat4(1.0f);
        tfMatrix = glm::translate(tfMatrix, tfColliderA.position) * glm::mat4_cast(tfColliderA.rotation);        
        for (auto& vertex : wposColliderA.colliderVertices)
        {        
            glm::vec4 tfVertex = tfMatrix * glm::vec4(vertex, 1.0f);
            vertex.x = tfVertex.x;
            vertex.y = tfVertex.y;
            vertex.z = tfVertex.z;                   
        }        
                
        tfMatrix = glm::mat4(1.0f);
        tfMatrix = glm::translate(tfMatrix, tfColliderB.position) * glm::mat4_cast(tfColliderB.rotation);

        for (auto& vertex : wposColliderB.colliderVertices)
        {            
            glm::vec4 tfVertex = tfMatrix * glm::vec4(vertex, 1.0f);
            vertex.x = tfVertex.x;
            vertex.y = tfVertex.y;
            vertex.z = tfVertex.z;
        }

        //initial support pnt
        glm::vec3 dir = glm::vec3{ 1,0,0 };
        glm::vec3 support = MeshCollider::Support(&wposColliderA, &wposColliderB, dir);
        Simplex points;
        points.Push(support);

        //New direction is towards the origin
        dir = -support;
        while (true)
        {
            support = MeshCollider::Support(&wposColliderA, &wposColliderB, dir);
            if (glm::dot(support, dir) <=0)                
            {              
                return false;
            }
            points.Push(support);
            
            if (NextSimplex(points, dir))
            {
                auto collisionData = EPA(points, &wposColliderA, &wposColliderB);                  
                collisionInfos.emplace_back(collisionData);
                return true;
            }
        }
    }    

    bool PhysicsEngine::NextSimplex(Simplex& points, glm::vec3& dir)
    {
        switch (points.Size())
        {
        case 2: return Line(points, dir);
        case 3: return Triangle(points, dir);
        case 4: return Tetrahedron(points, dir);
        }
        return false;
    }

    bool PhysicsEngine::SameDirection(const glm::vec3& dir, const glm::vec3& ao)
    {
        return glm::dot(dir, ao) > 0;
    }

    bool PhysicsEngine::Line(Simplex& points, glm::vec3& dir)
    {
        auto a = points[0];
        auto b = points[1];

        auto ab = b - a;
        auto ao = -a;
                
        if (SameDirection(ab, ao))
        {            
            dir = glm::cross(glm::cross(ab, ao), ab);
        }
        else
        {
            points = { a };
            dir = ao;
        }

        return false;
    }

    bool PhysicsEngine::Triangle(Simplex& points, glm::vec3& dir)
    {
        glm::vec3 a = points[0];
        glm::vec3 b = points[1];
        glm::vec3 c = points[2];

        glm::vec3 ab = b - a;
        glm::vec3 ac = c - a;
        glm::vec3 ao = -a;

        glm::vec3 abc = glm::cross(ab, ac);

        if (SameDirection(glm::cross(abc, ac), ao))
        {
            if (SameDirection(ac, ao))
            {
                points = { a,c };
                dir = glm::cross(glm::cross(ac, ao), ac);                
            }
            else
            {
                return Line(points = { a,b }, dir);
            }            
        }
        else
        {
            if (SameDirection(glm::cross(ab, abc), ao))
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

    bool PhysicsEngine::Tetrahedron(Simplex& points, glm::vec3& dir)
    {
        glm::vec3 a = points[0];
        glm::vec3 b = points[1];
        glm::vec3 c = points[2];
        glm::vec3 d = points[3];

        glm::vec3 ab = b - a;
        glm::vec3 ac = c - a;
        glm::vec3 ad = d - a;
        glm::vec3 ao = -a;

        glm::vec3 abc = glm::cross(ab, ac);
        glm::vec3 acd = glm::cross(ac, ad);
        glm::vec3 adb = glm::cross(ad, ab);

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
        std::vector<glm::vec3> polytope(simplex.Begin(), simplex.End());
        std::vector<size_t> faces =
        {
            0,1,2,
            0,3,1,
            0,2,3,
            1,3,2
        };

        auto faceNormalData = GetFaceNormals(polytope, faces);        
        auto normals = faceNormalData.first;
        auto minFace = faceNormalData.second;

        glm::vec3 minNormal;
        float minDistance = FLT_MAX;

        size_t iterations = 0;
        while (minDistance == FLT_MAX)
        {
            minNormal = glm::vec3(normals[minFace]);
            minDistance = normals[minFace].w;

            if (iterations++ > 32)
            {
                break;
            }

            glm::vec3 support = MeshCollider::Support(colliderA, colliderB, minNormal);
            float sDist = glm::dot(minNormal, support);

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

    std::pair<std::vector<glm::vec4>, size_t> PhysicsEngine::GetFaceNormals(const std::vector<glm::vec3>& polytope, const std::vector<size_t>& faces)
    {
        std::vector<glm::vec4> normals;
        size_t minTriangle = 0;
        float minDistance = FLT_MAX;

        for (size_t i = 0; i < faces.size(); i += 3)
        {
            auto a = polytope[faces[i]];
            auto b = polytope[faces[i+1]];
            auto c = polytope[faces[i+2]];
                        
            auto normal = glm::normalize(glm::cross((b - a), (c - a)));
            float dist = glm::dot(normal, a);

            if (dist < 0)
            {
                normal *= -1;
                dist *= -1;
            }

            auto normalWithDist = glm::vec4(normal, dist);
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

            auto aVelocity = isAKinematic ? glm::vec3(0) : aPhysicsData.rigidBody.velocity;
            auto bVelocity = isBKinematic ? glm::vec3(0) : bPhysicsData.rigidBody.velocity;
            auto rVelocity = bVelocity - aVelocity;
            float spd = glm::dot(rVelocity, cInfo.normal);

            float aInvMass = isAKinematic ? 1.0f : 1.0f / aPhysicsData.rigidBody.mass;
            float bInvMass = isBKinematic ? 1.0f : 1.0f / bPhysicsData.rigidBody.mass;

            if (spd >= 0)
                continue;

            float e = aPhysicsData.rigidBody.bounciness * bPhysicsData.rigidBody.bounciness;
            float j = -(1.0f + e) * spd / (aInvMass + bInvMass);

            glm::vec3 impulse = cInfo.normal * j;

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
            spd = glm::dot(rVelocity, cInfo.normal);

            glm::vec3 tan = rVelocity - (cInfo.normal*spd);

            if (glm::length(tan) > 0.0001f)
            {
                tan = glm::normalize(tan);
            }

            float fVelocity = glm::dot(rVelocity, tan);

            float aStaticFric = aPhysicsData.rigidBody.staticFriction;
            float bStaticFric = bPhysicsData.rigidBody.staticFriction;
            float aDynamicFric = aPhysicsData.rigidBody.dynamicFriction;
            float bDynamicFric = bPhysicsData.rigidBody.dynamicFriction;
            float m = glm::length(glm::vec3(aStaticFric, bStaticFric, 0));

            float fric = -fVelocity / (aInvMass + bInvMass);

            glm::vec3 friction;
            if (abs(fric) < j * m)
            {
                friction = tan * fric;
            }
            else
            {
                m = glm::length(glm::vec3(aDynamicFric, bDynamicFric, 0));
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

            glm::vec3 resolution = -cInfo.normal * cInfo.depth;           

            if (!aPhysicsData.rigidBody.isKinematic && !bPhysicsData.rigidBody.isKinematic)
                resolution = resolution * 0.5f;

            if (!aPhysicsData.rigidBody.isKinematic)
                aPhysicsData.transform.position += resolution;
            if (!bPhysicsData.rigidBody.isKinematic)
                bPhysicsData.transform.position -= resolution;
        } 
    }
}