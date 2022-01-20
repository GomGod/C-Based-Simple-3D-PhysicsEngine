#pragma once
#ifndef SIMPLEPHYSICSENGINE_OBJECT_H
#define SIMPLEPHYSICSENGINE_OBJECT_H
#include "Vector3.h"
#include "Shader.h"
#include "MeshCollider.h"

#include "glad/glad.h"		
#include "GLFW/glfw3.h" 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

namespace SimplePhysicsEngine
{
    struct AABB
    {
        float minX, maxX;
        float minY, maxY;
        float minZ, maxZ;

        bool TestAABBCollision(const utils::Vector3& point)
        {
            return (minX < point.x && point.x < maxX) &&
                (minY < point.y && point.y < maxY) &&
                (minZ < point.z && point.z < maxZ);
        }

        bool TestAABBCollision(const AABB& box)
        {   
            if (maxX < box.minX || minX > box.maxX) return false;
            if (maxY < box.minY || minY > box.maxY) return false;
            if (maxZ < box.minZ || minZ > box.maxZ) return false;
            return true;        
        }

        AABB& operator+ (const utils::Vector3& pos)
        {
            return AABB{ minX + pos.x, maxX + pos.x, minY + pos.y, maxY + pos.y, minZ + pos.z, maxZ + pos.z };
        }

        AABB& operator& (const AABB& origin)
        {
            return AABB{ origin.minX , origin.maxX , origin.minY , origin.maxY , origin.minZ, origin.maxZ };
        }
    };

    struct Transform
    {
        utils::Vector3 position;
        utils::Vector3 rotation;

        Transform(utils::Vector3 position = utils::Vector3{ 0, 0, 0 }, utils::Vector3 rotation = utils::Vector3{ 0, 0, 0 })
        {
            this->position = position;
            this->rotation = rotation;
        }
    };

    struct Material
    {
        utils::Vector3 ambient;
        utils::Vector3 diffuse;
        utils::Vector3 specular;
        utils::Vector3 color;
        Shader* shader;

        Material(Shader* shader, utils::Vector3 color = utils::Vector3{ 1.0f, 1.0f, 1.0f }, utils::Vector3 ambient = utils::Vector3{ 0.5f, 0.5f, 0.5f }, utils::Vector3 diffuse = utils::Vector3{ 0.5f, 0.5f, 0.5f }, utils::Vector3 specular = utils::Vector3{ 0.5f, 0.5f, 0.5f }) :shader(shader), color(color), ambient(ambient), diffuse(diffuse), specular(specular)
        {}
    };

    struct RigidBody
    {
        float mass;
        float gravity;
        utils::Vector3 velocity;
        utils::Vector3 forces;
        bool isKinematic;

        float staticFriction;
        float dynamicFriction;
        float bouciness;

        RigidBody(float mass = 10.0f, float gravity = 1.0f, utils::Vector3 velocity = utils::Vector3{0,0,0}, utils::Vector3 forces= utils::Vector3{ 0,0,0 }, bool isKinematic=false) :mass(mass), gravity(gravity), velocity(velocity), forces(forces), isKinematic(isKinematic)
        {}
    };

    class Object {
    public:
        //For Manage Object
        int ID;

        //Vars for physics        
        Transform* transform;
        RigidBody* rigidBody;
        MeshCollider* collider;
        AABB* aabb;

        //Vars for render
        Material* material;

               
        Object(Transform* transform, RigidBody* rigidBody, Material* material) : transform(transform), rigidBody(rigidBody), material(material)
        {
            collider = new MeshCollider;            
            aabb = new AABB;
            VAO = 0;
            VBO = 0;
            EBO = 0;
            ID = -1;
        }

        ~Object()
        {
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
            glDeleteVertexArrays(1, &VAO);
            delete transform;
            delete rigidBody;
            delete material;
            delete aabb;
        }

        void Object::UpdateTranform(utils::Vector3 position, utils::Vector3 rotation);
        void Object::UpdatePhysics(utils::Vector3 velocity, utils::Vector3 forces);

        //Rendering for OpenGL   
        void setShader(Shader* shader) { material->shader = shader; }
        const Shader* getShader() const { return material->shader; }
        void activateShader() { material->shader->use(); }

        const float* getVertices() const { return vertices.data(); }
        const float* getNormals() const { return normals.data(); }
        const unsigned int* getIndicies() const { return indices.data(); }

        const int getVertexSize() const { return vertices.size() * sizeof(float); }
        const int getNormalSize() const { return normals.size() * sizeof(float); }
        const int getIndexSize() const { return indices.size() * sizeof(float); }

        unsigned int getVertexCount() const { return (unsigned int)vertices.size(); }
        unsigned int getNormalCount() const { return (unsigned int)normals.size(); }
        unsigned int getIndexCount() const { return (unsigned int)indices.size(); }

        const float* getInterleavedVertexAttrib() const { return interleavedVertexAttrib.data(); }
        const int getInterleavedVertexAttribSize() { return interleavedVertexAttrib.size() * sizeof(float); }
        unsigned int getInterleavedVertexAttribCount() const { return (unsigned int)interleavedVertexAttrib.size(); }

        unsigned int getVAO() { return VAO; }

        void draw(glm::vec3 cameraPos, glm::mat4 view, glm::mat4 projection);

    protected:
        //Rendering for OpenGL    
        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<unsigned int> indices;
        std::vector<float> interleavedVertexAttrib;
        
        void buildInterleavedVertices();
        void clearArrays();
        void addVertex(float x, float y, float z);
        void addNormal(float x, float y, float z);
        void addIndices(unsigned int i1, unsigned int i2, unsigned int i3);
        std::vector<float> getFaceNormal(float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3);

        void buildVAO();
        unsigned int VAO, VBO, EBO;
    };
};

#endif //SIMPLEPHYSICSENGINE_OBJECT_H