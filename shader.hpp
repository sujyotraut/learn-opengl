#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>
#include <glad/glad.h>

class Shader {
public:
    // The program ID
    unsigned int ID;

    // Constructor that reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath);

    // Use/activate the shader
    void use() const;

    // Utility uniform functions
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;

private:
    void checkCompileErrors(unsigned int shader, const std::string& type);
};

#endif
