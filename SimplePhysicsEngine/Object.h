#pragma once
#ifndef SIMPLEPHYSICSENGINE_OBJECT_H
#define SIMPLEPHYSICSENGINE_OBJECT_H
#include "Shader.h"
#include "MeshCollider.h"
#include "Rigidbody.h"

#include "glad/glad.h"		
#include "GLFW/glfw3.h" 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

namespace SimplePhysicsEngine
{
    struct Transform
    {
        glm::vec3 position;
        glm::vec3 rotation;
    
        Transform(glm::vec3 position = glm::vec3(0), glm::vec3 rotation = glm::vec3(0))
        {
            this->position = position;
            this->rotation = rotation;
        }

        Transform(const Transform& origin)
        {
            position = origin.position;
            rotation = origin.rotation;
        }        
    };

    struct Material
    {
        glm::vec3 ambient;
        glm::vec3 diffuse;
        glm::vec3 specular;
        glm::vec4 color;
        Shader* shader;

        Material(Shader* shader, glm::vec4 color = glm::vec4(1.0f), glm::vec3 ambient = glm::vec3(0.5f), glm::vec3 diffuse = glm::vec3(0.5f), glm::vec3 specular = glm::vec3(0.5f)) :shader(shader), color(color), ambient(ambient), diffuse(diffuse), specular(specular)
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

        void Object::UpdateTranform(glm::vec3 position, glm::vec3 rotation);
        void Object::UpdatePhysics(glm::vec3 velocity, glm::vec3 forces);

        //Rendering for OpenGL   
        void SetShader(Shader* shader) { material->shader = shader; }
        const Shader* GetShader() const { return material->shader; }
        void ActivateShader() { material->shader->use(); }

        const float* GetVertices() const { return vertices.data(); }
        const float* GetNormals() const { return normals.data(); }
        const unsigned int* GetIndicies() const { return indices.data(); }

        const int GetVertexSize() const { return vertices.size() * sizeof(float); }
        const int GetNormalSize() const { return normals.size() * sizeof(float); }
        const int GetIndexSize() const { return indices.size() * sizeof(float); }

        unsigned int GetVertexCount() const { return (unsigned int)vertices.size(); }
        unsigned int GetNormalCount() const { return (unsigned int)normals.size(); }
        unsigned int GetIndexCount() const { return (unsigned int)indices.size(); }

        const float* GetInterleavedVertexAttrib() const { return interleavedVertexAttrib.data(); }
        const int GetInterleavedVertexAttribSize() { return interleavedVertexAttrib.size() * sizeof(float); }
        unsigned int GetInterleavedVertexAttribCount() const { return (unsigned int)interleavedVertexAttrib.size(); }
        unsigned int GetVAO() { return VAO; }

        void Draw(glm::vec3 cameraPos, glm::mat4 view, glm::mat4 projection);

    protected:        
        //Rendering for OpenGL    
        std::vector<float> vertices;
        std::vector<float> normals;
        std::vector<unsigned int> indices;
        std::vector<float> interleavedVertexAttrib;
        
        void BuildInterleavedVertices();
        void ClearArrays();
        void AddVertex(float x, float y, float z);
        void AddNormal(float x, float y, float z);
        void AddIndices(unsigned int i1, unsigned int i2, unsigned int i3);
        std::vector<float> GetFaceNormal(float x1, float y1, float z1,
            float x2, float y2, float z2,
            float x3, float y3, float z3);

        void BuildVAO();
        unsigned int VAO, VBO, EBO;        
    };
};

#endif //SIMPLEPHYSICSENGINE_OBJECT_H