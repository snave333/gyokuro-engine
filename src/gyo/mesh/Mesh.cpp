
#include <map>

#include <gyo/mesh/Mesh.h>
#include <gyo/mesh/Vertex.h>
#include <gyo/geometry/Geometry.h>
#include <gyo/shading/Material.h>
#include <gyo/utilities/GetError.h>
#include <gyo/shading/ShaderSemantics.h>

#include <glad/glad.h>

namespace gyo {

struct VertexAttributeEntry {
    unsigned int semantic;
    GLint index;            // i.e. location
    GLint size;             // in float values, not bytes
    unsigned long offset;   // in float values, not bytes
};


Mesh::Mesh(Geometry* geometry, Material* material) {
    this->geometry = geometry;
    this->material = material;

    // only compute the tangents if they haven't already been
    if(geometry->tangents.empty()) {
        this->geometry->ComputeTangents();
    }

    Initialize();
    ComputeBounds();

    indexCount = this->geometry->indices.size();
    numTris = indexCount / 3;
}

void Mesh::Initialize() {
    if(VAO != 0) {
        return;
    }

    // create the vertex/index buffers and vertex array object
    glGenVertexArrays(1, &VAO);
    glCheckError();
    glGenBuffers(1, &VBO);
    glCheckError();
    glGenBuffers(1, &EBO);
    glCheckError();
    
    // bind Vertex Array Object first
    glBindVertexArray(VAO);
    glCheckError();
    
    // copy our vertices array in a buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glCheckError();
    glBufferData(GL_ARRAY_BUFFER, 0, NULL, GL_STATIC_DRAW); // set to 0 initially; computed in ComputeVertexArrayBuffer
    glCheckError();

    // copy our indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glCheckError();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, geometry->indices.size() * sizeof(unsigned int), &geometry->indices[0], GL_STATIC_DRAW);
    glCheckError();

    // clean up and unbind
    glBindVertexArray(0);
    glCheckError();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glCheckError();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glCheckError();

    // compute and set our vertex array buffer
    ComputeVertexArrayBuffer();
}

void Mesh::ComputeVertexArrayBuffer() {
    if(VAO == 0) {
        std::cerr << "Cannot set vertex array buffer without vertex array object" << std::endl;
        return;
    }

    if(material == nullptr) {
        std::cout << "Cannot initialize mesh; missing material" << std::endl;
        return;
    }

    if(!material->ValidateShaderAttributes()) {
        std::cerr << "Invalid shader attributes and declared semantics" << std::endl;
        return;
    }

    const Shader& shader = material->GetShader();
    const std::map<std::string, AttributeInfo>& shaderAttributes = shader.GetAttributes();

    // gather our vertex array entries

    unsigned int vertexCount = geometry->positions.size();
    unsigned int floatsPerVertex = 0U;
    unsigned long bytesPerVertex = 0UL;

    auto declaredAttributes = material->GetShaderSemantics();
    std::vector<VertexAttributeEntry> attributes;
    attributes.reserve(declaredAttributes.size());

    for(auto& pair : declaredAttributes) {
        std::string name = pair.first;
        unsigned int semantic = pair.second;

        auto shaderAttribute = shaderAttributes.at(name);
        auto attributeSizeDesc = SEMANTIC_TO_GLSIZE.at(semantic);

        attributes.push_back({
            semantic,
            shaderAttribute.location,
            attributeSizeDesc.first,
            floatsPerVertex
        });
        
        floatsPerVertex += attributeSizeDesc.first;
        bytesPerVertex += attributeSizeDesc.first * attributeSizeDesc.second;
    }

    // generate our vertex array

    unsigned int vertexArrayLength = vertexCount * floatsPerVertex;
    float vertexArray[vertexArrayLength];
    for(const VertexAttributeEntry& attribute : attributes) {
        for(int v = 0; v < vertexCount; v++) {
            int i0 = v * floatsPerVertex + attribute.offset;
            int i1 = i0 + 1;
            int i2 = i0 + 2;
            int i3 = i0 + 3;
            
            switch(attribute.semantic) {
                case SEMANTIC_POSITION:
                    vertexArray[i0] = geometry->positions[v].x;
                    vertexArray[i1] = geometry->positions[v].y;
                    vertexArray[i2] = geometry->positions[v].z;
                    break;

                case SEMANTIC_NORMAL:
                    vertexArray[i0] = geometry->normals[v].x;
                    vertexArray[i1] = geometry->normals[v].y;
                    vertexArray[i2] = geometry->normals[v].z;
                    break;

                case SEMANTIC_TEXCOORD0:
                    vertexArray[i0] = geometry->texCoords[v].x;
                    vertexArray[i1] = geometry->texCoords[v].y;
                    break;

                case SEMANTIC_TANGENT:
                    vertexArray[i0] = geometry->tangents[v].x;
                    vertexArray[i1] = geometry->tangents[v].y;
                    vertexArray[i2] = geometry->tangents[v].z;
                    break;

                // Currently unsupported; get this from the shader?
                // case SEMANTIC_COLOR:
                //     vertexArray[i0] = geometry->colors[v].r;
                //     vertexArray[i1] = geometry->colors[v].g;
                //     vertexArray[i2] = geometry->colors[v].b;
                //     vertexArray[i3] = geometry->colors[v].a;
                //     break;

                default:
                    std::cerr << "Error: Unsupported semantic type " << attribute.semantic << std::endl;
                    break;
            }
        }
    }

    // bind Vertex Array Object first
    glBindVertexArray(VAO);
    glCheckError();

    // disable all attributes in case we're rebuilding
    for (GLuint i = 0; i < SEMANTIC_COLOR; ++i) {
        glDisableVertexAttribArray(i);
    }

    // generate the vertex buffer object in case we're not rebuilding
    if(VBO == 0) {
        glGenBuffers(1, &VBO);
        glCheckError();
    }

    // copy our vertices array in a buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glCheckError();
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), &vertexArray, GL_STATIC_DRAW);
    glCheckError();

    // link the vertex attribute pointers
    for(const VertexAttributeEntry& attribute : attributes) {
        glEnableVertexAttribArray(attribute.index);
        glCheckError();
        glVertexAttribPointer(attribute.index, attribute.size, GL_FLOAT, GL_FALSE, bytesPerVertex, (void*)(attribute.offset * sizeof(float)));
        glCheckError();
    }

    // clean up and unbind
    glBindVertexArray(0);
    glCheckError();
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glCheckError();
}

void Mesh::ComputeBounds() {
    // init min and max with extreme values
    glm::vec3 minPoint(std::numeric_limits<float>::max());
    glm::vec3 maxPoint(std::numeric_limits<float>::lowest());

    // iterate through the vertex array
    for (const auto& position : geometry->positions) {
        minPoint = glm::min(minPoint, position);
        maxPoint = glm::max(maxPoint, position);
    }

    bounds = { minPoint, maxPoint };
}

Mesh::~Mesh() {
    delete geometry;
    geometry = nullptr;

    delete material;
    material = nullptr;

    glDeleteVertexArrays(1, &VAO);
    glCheckError();
    glDeleteBuffers(1, &VBO);
    glCheckError();
    glDeleteBuffers(1, &EBO);
    glCheckError();
}

void Mesh::SetMaterial(Material* newMaterial) {
    if(this->material) {
        delete this->material;
        this->material = nullptr;
    }

    if(newMaterial) {
        this->material = newMaterial;
        this->ComputeVertexArrayBuffer();
    }
}

void Mesh::Draw() {
    if(VAO == 0) {
        return;
    }

    glBindVertexArray(VAO);
    glCheckError();
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
    glCheckError();
    glBindVertexArray(0);
    glCheckError();
}

} // namespace gyo
