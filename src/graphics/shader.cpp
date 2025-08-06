#include "shader.h"
#include <glad/glad.h> // include glad to get all the required OpenGL headers

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

Shader::Shader()
    : ID(0)
{
}

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath)
    : ID(0)
{
    LoadShader(CUSTOM_SHADER, vertexPath, fragmentPath);
}

Shader::~Shader()
{
    Unload();
}

void Shader::LoadShader(DefaultShader typeShader, const std::string &vertexPath, const std::string &fragmentPath)
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

    if (typeShader == DEFAULT_SHADER)
    {
        fragmentCode = fsDefaultShader;
        vertexCode = vsDefaultShader;
    }
    else if (typeShader == TEXT_DEFAULT_SHADER)
    {
        fragmentCode = fsDefaultTextShader;
        vertexCode = vsDefaultTextShader;
    }
    else
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure &e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
        }
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

void Shader::setBool(const std::string &name, bool value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
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
