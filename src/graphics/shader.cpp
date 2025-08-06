#include "shader.h"
#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Fusion
{

    Shader::Shader(DefaultShader typeShader)
        : ID(0), m_TypeShader(typeShader)
    {
    }

    Shader::~Shader()
    {
        Unload();
    }

    void Shader::LoadShader(const std::string &vertexPath, const std::string &fragmentPath)
    {

        const char *fsDefaultShader = R"(
    #version 330 core
    
    in vec4 ourColor;       // Cor recebida do Vertex Shader
    in vec2 TexCoord;

    uniform sampler2D ourTexture;

    out vec4 FragColor;

    void main()
    {
        vec4 texColor = texture(ourTexture, TexCoord); // Pega a cor da textura normalmente  
        FragColor = texColor * ourColor;               // Combina a cor da textura com a cor do vértice (que inclui o alfa)
    }
)";

        const char *vsDefaultShader = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;         // Posição já em coordenadas de mundo
    layout (location = 1) in vec4 aColor;       // Cor do vértice (RGBA)
    layout (location = 2) in vec2 aTexCoord;    // Coordenada da textura

    out vec4 ourColor; // Saídas para o Fragment Shader
    out vec2 TexCoord;

    uniform mat4 projection; // Só precisamos da projeção, pois aPos já está no espaço do mundo
    uniform mat4 view;

    void main()
    {
        gl_Position = projection * view * vec4(aPos, 1.0); // Aplica apenas a projeção
        ourColor = aColor;
        TexCoord = aTexCoord;
    }
)";

        const char *fsDefaultTextShader = R"(
    #version 330 core
    in vec2 TexCoord;

    uniform sampler2D ourTexture;
    uniform vec4 textColor;

    out vec4 FragColor;

    void main()
    {
        float alpha = texture(ourTexture, TexCoord).r;        // Pega o alfa do canal vermelho da textura da fonte
        FragColor = vec4(textColor.rgb, alpha * textColor.a); // branco com alpha da fonte
    }
)";

        const char *vsDefaultTextShader = R"(
    #version 330 core

    layout (location = 0) in vec3 aPos;      // Posição do vértice
    layout (location = 1) in vec4 aColor;    // Cor do vértice (mesmo que não usemos, precisa estar aqui)
    layout (location = 2) in vec2 aTexCoord; // Coordenada da textura

    uniform mat4 projection;
    uniform mat4 view;

    out vec2 TexCoord;

    void main()
    {
        gl_Position = projection * view * vec4(aPos, 1.0);  // A posição já vem calculada, só aplicamos a projeção
        TexCoord = aTexCoord;
    }
)";

        std::string vertexCode;
        std::string fragmentCode;

        if (m_TypeShader == DEFAULT_SHADER)
        {
            vertexCode = vsDefaultShader;
            fragmentCode = fsDefaultShader;
        }
        else if (m_TypeShader == TEXT_DEFAULT_SHADER)
        {
            vertexCode = vsDefaultTextShader;
            fragmentCode = fsDefaultTextShader;
        }

        // 1. retrieve the vertex/fragment source code from filePath
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        // Carrega o Vertex Shader customizado se caminho foi informado
        if (!vertexPath.empty())
        {
            try
            {
                vShaderFile.open(vertexPath);
                std::stringstream vShaderStream;
                vShaderStream << vShaderFile.rdbuf();
                vShaderFile.close();
                vertexCode = vShaderStream.str();
            }
            catch (std::ifstream::failure &e)
            {
                std::cout << "ERRO::SHADER::ARQUIVO_DE_VERTICE_NAO_LIDO: " << e.what() << std::endl;
            }
        }
        else
        {
            // Se o caminho do vértice for vazio, usa o shader de vértice padrão para sprites
            std::cout << "INFO::SHADER:: Usando Vertex Shader padrao." << std::endl;
        }

        // Carrega o Fragment Shader customizado se caminho foi informado
        if (!fragmentPath.empty())
        {
            try
            {
                fShaderFile.open(fragmentPath);
                std::stringstream fShaderStream;
                fShaderStream << fShaderFile.rdbuf();
                fShaderFile.close();
                fragmentCode = fShaderStream.str();
            }
            catch (std::ifstream::failure &e)
            {
                std::cout << "ERRO::SHADER::ARQUIVO_DE_FRAGMENTO_NAO_LIDO: " << e.what() << std::endl;
            }
        }
        else
        {
            // Se o caminho do fragmento for vazio, usa o shader de fragmento padrão para sprites
            std::cout << "INFO::SHADER:: Usando Fragment Shader padrao." << std::endl;
        }

        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertex, fragment;
        // vertex shader
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");
        // fragment Shader
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");
        // shader Program
        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");
        // delete the shaders as they're linked into our program now and no longer necessary
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void Shader::use()
    {
        glUseProgram(ID);
    }

    void Shader::SetBool(const std::string &name, bool value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
    }

    void Shader::SetInt(const std::string &name, int value) const
    {
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
    }

    void Shader::SetFloat(const std::string &name, float value) const
    {
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
    }

    void Shader::SetVec2(const std::string &name, float v1, float v2) const
    {
        glUniform2f(glGetUniformLocation(ID, name.c_str()), v1, v2);
    }

    void Shader::SetVec3(const std::string &name, float v1, float v2, float v3) const
    {
        glUniform3f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3);
    }

    void Shader::SetVec4(const std::string &name, float v1, float v2, float v3, float v4) const
    {
        glUniform4f(glGetUniformLocation(ID, name.c_str()), v1, v2, v3, v4);
    }

    int Shader::getUniformLocation(const std::string &name)
    {
        if (uniformCache.find(name) != uniformCache.end())
            return uniformCache[name];

        int location = glGetUniformLocation(ID, name.c_str());
        uniformCache[name] = location;
        return location;
    }

    void Shader::getUniformMatrix4(const std::string &name, glm::mat4 &ptr)
    {
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(ptr));
    }

    void Shader::checkCompileErrors(unsigned int shader, std::string type)
    {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                          << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                          << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
    }

    void Shader::Unload()
    {
        if (ID != 0)
        {
            glDeleteProgram(ID);
            ID = 0;
        }
    }
}