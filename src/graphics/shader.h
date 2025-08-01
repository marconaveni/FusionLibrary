#ifndef SHADER_H
#define SHADER_H


#include <string>
#include <unordered_map>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

enum DefaultShader
{
    DEFAULT_SHADER, 
    TEXT_DEFAULT_SHADER,
    CUSTOM_SHADER
};

class Shader
{
public:
    
    unsigned int ID; // the program ID
    std::unordered_map<std::string, int> uniformCache;
  

    Shader();

    // constructor reads and builds the shader
    Shader(const std::string& vertexPath, const std::string& fragmentPath);

    ~Shader();

    void LoadShader(DefaultShader typeShader, const std::string& vertexPath = "", const std::string& fragmentPath = "");

    // use/activate the shader
    void use();

    int getUniformLocation(const std::string& name);
    void getUniformMatrix4(const std::string& name, glm::mat4& projection);

    // utility uniform functions
    void setBool(const std::string &name, bool value) const;  
    void setInt(const std::string &name, int value) const;   
    void setFloat(const std::string &name, float value) const;
    void Unload();
    
private:

    // utility function for checking shader compilation/linking errors.
    void checkCompileErrors(unsigned int shader, std::string type);

};
  
#endif