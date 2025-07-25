#ifndef SHADER_H
#define SHADER_H


#include <string>


class Shader
{
public:
    
    unsigned int ID; // the program ID
  
    // constructor reads and builds the shader
    Shader(const std::string& vertexPath, const std::string& fragmentPath);

    // use/activate the shader
    void use();

    // utility uniform functions
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;

private:

    // utility function for checking shader compilation/linking errors.
    void checkCompileErrors(unsigned int shader, std::string type);
};
  
#endif