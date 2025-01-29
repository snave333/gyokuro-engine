
#include <gyo/drawable/AABBWireframe.h>
#include <math/AABB.h>
#include <gyo/internal/shading/Shader.h>
#include <gyo/resources/Resources.h>

#include <glad/glad.h>

namespace gyo {

AABBWireframe::AABBWireframe(const AABB& aabb, glm::vec4 color) {
    this->color = color;

    // the 8 corners of the AABB
    std::vector<glm::vec3> vertices = GetVertices(aabb.min, aabb.max);

    // the edges of the wireframe (indices)
    std::vector<unsigned int> indices = {
        0, 1, 1, 2, 2, 3, 3, 0, // bottom face edges
        4, 5, 5, 6, 6, 7, 7, 4, // top face edges
        0, 4, 1, 5, 2, 6, 3, 7  // vertical edges
    };

    // generate and bind VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // generate and bind VBO
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_DYNAMIC_DRAW);

    // generate and bind EBO
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

    // vertex attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    // unbind
    glBindVertexArray(0);

    // create shader
    shader = Resources::GetShader("wireframe.vert", "solidColor.frag");
    shader->Use();
    shader->SetUniformBlockBinding("Camera", 0);
}

AABBWireframe::~AABBWireframe() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    shader = nullptr;
}

std::vector<glm::vec3> AABBWireframe::GetVertices(const glm::vec3& min, const glm::vec3& max) {
    return {
        {min.x, min.y, min.z}, {max.x, min.y, min.z}, {max.x, max.y, min.z}, {min.x, max.y, min.z}, // bottom face
        {min.x, min.y, max.z}, {max.x, min.y, max.z}, {max.x, max.y, max.z}, {min.x, max.y, max.z}  // top face
    };
}

void AABBWireframe::Update(const AABB& aabb) {
    const glm::vec3& min = aabb.min;
    const glm::vec3& max = aabb.max;

    std::vector<glm::vec3> vertices = GetVertices(aabb.min, aabb.max);

    // update content of VBO memory
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(glm::vec3), &vertices[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void AABBWireframe::Draw() {
    shader->Use();
    shader->SetVec4("color", color);

    glBindVertexArray(VAO);
    glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0); // 12 lines
    glBindVertexArray(0);
}

} // namespace gyo
