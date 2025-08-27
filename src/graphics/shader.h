#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Fusion
{

    enum DefaultShader
    {
        DEFAULT_SHADER,
        TEXT_DEFAULT_SHADER
    };

    class Shader
    {
    public:

        unsigned int ID; // the program ID
        std::unordered_map<std::string, int> uniformCache;

        Shader(DefaultShader typeShader = DEFAULT_SHADER);

        ~Shader();

        void LoadShader(const std::string& vertexPath = "", const std::string& fragmentPath = "");

        // use/activate the shader
        void use();

        int getUniformLocation(const std::string& name);
        void getUniformMatrix4(const std::string& name, glm::mat4& projection);

        // utility uniform functions
        void SetBool(const std::string& name, bool value) const;
        void SetInt(const std::string& name, int value) const;
        void SetFloat(const std::string& name, float value) const;
        void SetVec2(const std::string& name, float v1, float v2) const;
        void SetVec3(const std::string& name, float v1, float v2, float v3) const;
        void SetVec4(const std::string& name, float v1, float v2, float v3, float v4) const;
        void Unload();

    private:

        // utility function for checking shader compilation/linking errors.
        void checkCompileErrors(unsigned int shader, std::string type);
        DefaultShader m_TypeShader;
    };

} // namespace Fusion

#endif