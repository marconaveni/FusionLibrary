#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Fusion
{

    class Shader
    {
    public:

        unsigned int m_id; // the program ID
        std::unordered_map<std::string, int> m_uniformCache;

        Shader();

        ~Shader();

        void LoadShader(const std::string& vertexCode, const std::string& fragmentCode);
        bool LoadShaderFromFile(const std::string& vertexPath, const std::string& fragmentPath);

        // use/activate the shader
        void Use();

        int GetUniformLocation(const std::string& name);
        void GetUniformMatrix4(const std::string& name, glm::mat4& projection);

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
        void CheckCompileErrors(unsigned int shader, std::string type);
    };

} // namespace Fusion

#endif