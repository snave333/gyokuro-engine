#ifndef SHADER_H
#define SHADER_H

#include <string>

class Shader {
public:
    // the program ID
    unsigned int ID;
  
    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);
    ~Shader();

    // use/activate the shader
    void Use();
    
    // utility uniform functions
    void SetBool(const std::string &name, bool value) const;  
    void SetInt(const std::string &name, int value) const;   
    void SetFloat(const std::string &name, float value) const;
    void SetFloat2(const std::string &name, float value1, float value2) const;
    void SetFloat3(const std::string &name, float value1, float value2, float value3) const;
    void SetFloat4(const std::string &name, float value1, float value2, float value3, float value4) const;
};
  
#endif // SHADER_H
