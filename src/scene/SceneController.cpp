
#include <iostream>

#include <scene/SceneController.h>
#include <renderer/Renderer.h>

#include <glad/glad.h>

SceneController::SceneController(Renderer* r) {
    renderer = r;

#pragma region SHADERS

    // compile the vert shader
    vertShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertShader, 1, &vertShaderSource, nullptr);
    glCompileShader(vertShader);

    // check that the compilation succeeded
    int success;
    char infoLog[512];
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // compile the frag shader
    fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragShader, 1, &fragShaderSource, nullptr);
    glCompileShader(fragShader);

    // check that the compilation succeeded
    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // create the shader program, and attach our compiled shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertShader);
    glAttachShader(shaderProgram, fragShader);
    glLinkProgram(shaderProgram);

    // check that the linking succeeded
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // clean up
    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

#pragma endregion

    // create the vertex/index buffers and vertex array object
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glGenVertexArrays(1, &VAO);
    
    // bind Vertex Array Object first
    glBindVertexArray(VAO);
    
    // copy our vertices array in a buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // copy our indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    // link the vertex attribute pointers
    // position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // clean up and unbind
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

SceneController::~SceneController() {
    renderer = nullptr;

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
}

void SceneController::Update(float dt) {
}

void SceneController::Render() {
    if(renderer == nullptr) {
        return;
    }
    
    renderer->BeginFrame(); // set render targets, etc

    RenderScene();
    
    renderer->EndFrame(); // swap buffers
}

void SceneController::RenderScene() {
    renderer->Clear();

    // draw scene objects
    
    glUseProgram(shaderProgram);

    // set any shader uniforms
    // int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
    // glUniform4f(vertexColorLocation, 0.0f, 1.0f, 0.0f, 1.0f);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}
