#include "Object.h"

namespace SimplePhysicsEngine
{   
    void Object::BuildInterleavedVertices()
    {
        std::vector<float>().swap(interleavedVertexAttrib);        
        std::size_t i, j;
        std::size_t count = vertices.size();
        for (i = 0, j = 0; i+2 < count; i += 3, j += 3)
        {
            interleavedVertexAttrib.push_back(vertices[i]);
            interleavedVertexAttrib.push_back(vertices[i + 1]);
            interleavedVertexAttrib.push_back(vertices[i + 2]);

            interleavedVertexAttrib.push_back(normals[i]);
            interleavedVertexAttrib.push_back(normals[i + 1]);
            interleavedVertexAttrib.push_back(normals[i + 2]);
        }

        collider->BuildColliderVertices(vertices.data(), GetVertexCount());
        BuildVAO();
    }

    void Object::ClearArrays()
    {
        std::vector<float>().swap(vertices);
        std::vector<float>().swap(normals);
        std::vector<unsigned int>().swap(indices);
    }

    void Object::AddVertex(float x, float y, float z)
    {
        vertices.push_back(x);
        vertices.push_back(y);
        vertices.push_back(z);
    }

    void Object::AddNormal(float x, float y, float z)
    {
        normals.push_back(x);
        normals.push_back(y);
        normals.push_back(z);
    }

    void Object::AddIndices(unsigned int i1, unsigned int i2, unsigned int i3)
    {
        indices.push_back(i1);
        indices.push_back(i2);
        indices.push_back(i3);
    }

    std::vector<float> Object::GetFaceNormal(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3)
    {
        const float EPSILON = 0.000001f;

        std::vector<float> normal(3, 0.0f);     // default return value (0,0,0)
        float nx, ny, nz;

        // find 2 edge vectors: v1-v2, v1-v3
        float ex1 = x2 - x1;
        float ey1 = y2 - y1;
        float ez1 = z2 - z1;
        float ex2 = x3 - x1;
        float ey2 = y3 - y1;
        float ez2 = z3 - z1;

        // cross product: e1 x e2
        nx = ey1 * ez2 - ez1 * ey2;
        ny = ez1 * ex2 - ex1 * ez2;
        nz = ex1 * ey2 - ey1 * ex2;

        // normalize only if the length is > 0
        float length = sqrtf(nx * nx + ny * ny + nz * nz);
        if (length > EPSILON)
        {
            // normalize
            float lengthInv = 1.0f / length;
            normal[0] = nx * lengthInv;
            normal[1] = ny * lengthInv;
            normal[2] = nz * lengthInv;
        }

        return normal;
    }
    void Object::BuildVAO()
    {        
        glGenVertexArrays(1, &VAO); //Gen VAO
        glBindVertexArray(VAO); // Bind VAO

        //Set Vertex Data
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, GetInterleavedVertexAttribSize(), GetInterleavedVertexAttrib(), GL_STATIC_DRAW);

        //Set Draw Indicies
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, GetIndexSize(), GetIndicies(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));

        //Enable Vertex Attributes
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        glBindVertexArray(0); // Unbind VAO        

        glGenVertexArrays(1, &colVAO); //Gen VAO
        glBindVertexArray(colVAO); // Bind VAO

        //Set Vertex Data
        glGenBuffers(1, &colVBO);
        glBindBuffer(GL_ARRAY_BUFFER, colVBO);

        std::vector<float> colliderData;
        int i = 0;
        for (auto a : collider->colliderVertices)
        {
            colliderData.push_back(a.x);
            colliderData.push_back(a.y);
            colliderData.push_back(a.z);
        }

        glBufferData(GL_ARRAY_BUFFER, colliderData.size() * sizeof(float), colliderData.data(), GL_STATIC_DRAW);
        //Set Draw Indicies
        //glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        //glBufferData(GL_ELEMENT_ARRAY_BUFFER, GetIndexSize(), GetIndicies(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);        
        
        //Enable Vertex Attributes
        glEnableVertexAttribArray(0);        
        glBindVertexArray(0); // Unbind VAO        
    }

    void Object::UpdateTranform(glm::vec3 position, glm::quat rotation)
    {
        transform->position = position;
        transform->rotation = rotation;
    }

    void Object::UpdatePhysics(glm::vec3 velocity, glm::vec3 forces)
    {
        rigidBody->velocity = velocity;
        rigidBody->forces = forces;
    }

    void Object::Draw(glm::vec3 cameraPos, glm::mat4 view, glm::mat4 projection)
    {
        ActivateShader();
        glBindVertexArray(VAO);
        auto shader = GetShader();

        shader->SetVec3("viewPos", cameraPos.x, cameraPos.y, cameraPos.z);
        shader->SetMat4("view", view);
        shader->SetMat4("projection", projection);
        shader->SetVec3("material.ambient", material->ambient.x, material->ambient.y, material->ambient.z);
        shader->SetVec3("material.diffuse", material->diffuse.x, material->diffuse.y, material->diffuse.z);
        shader->SetVec3("material.specular", material->specular.x, material->specular.y, material->specular.z);
        shader->SetVec3("material.color", material->color.x, material->color.y, material->color.z);
        glm::mat4 model = glm::mat4(1.0);                        
        auto translateMat = glm::mat4(1.0);
        auto rotationMat = glm::mat4_cast(transform->rotation);
        auto scaleMat = glm::mat4(1.0);
        model = glm::translate(translateMat, transform->position)
            * glm::mat4_cast(transform->rotation) * scaleMat;
        shader->SetMat4("model", model);

        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawElements(GL_TRIANGLES, GetIndexCount(), GL_UNSIGNED_INT, (void*)0);       

        glBindVertexArray(colVAO);
        shader->SetVec3("material.color", 0.0f, 1.0f, 0.0f);
        scaleMat = glm::scale(scaleMat, glm::vec3(1.005f));
        model = glm::translate(translateMat, transform->position)
            * glm::mat4_cast(transform->rotation) * scaleMat;
        shader->SetMat4("model", model);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, GetIndexCount(), GL_UNSIGNED_INT, (void*)0);
    }    
}