#ifndef SHADER_H
#define SHADER_H


#include <string>
#include <unordered_map>


class Shader
{
public:
    
    unsigned int ID; // the program ID
    std::unordered_map<std::string, int> uniformCache;
  
    // constructor reads and builds the shader
    Shader(const std::string& vertexPath, const std::string& fragmentPath);

    // use/activate the shader
    void use();

    int getUniformLocation(const std::string& name);

    // utility uniform functions
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;

private:

    // utility function for checking shader compilation/linking errors.
    void checkCompileErrors(unsigned int shader, std::string type);
};
  
#endif