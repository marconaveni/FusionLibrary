
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include "shader.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

constexpr int WIDTH = 800; 
constexpr int HEIGHT = 600; 


struct Rectangle {
    float x;
    float y;
    float width;
    float height;
};

struct Texture2D
{
    unsigned int id;
    int width; 
    int height; 
    int nrChannels;
};

struct Vector2
{
    float x;
    float y;
};

struct Font
{
    unsigned int fontTexture;
    unsigned char ttf_buffer[1 << 20]; // 1MB de buffer pra fonte
    unsigned char bitmap[512 * 512];   // espaço pra gerar os caracteres
    stbtt_bakedchar cdata[96];         // de ' ' até '~'
};


Vector2 GetWindowSize(GLFWwindow* window)
{
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    return Vector2{float(width), float(height)};
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, height - HEIGHT, WIDTH, HEIGHT);
}


void getOpenGLVersionInfo()
{
    std::cout << "Vendor " << glGetString(GL_VENDOR) << "\n";
    std::cout << "Renderer " << glGetString(GL_RENDERER) << "\n";
    std::cout << "Version " << glGetString(GL_VERSION) << "\n";
    std::cout << "Shading Language " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";
}

Texture2D LoadTexture(const char *fileName)
{
    Texture2D texture;

    glGenTextures(1, &texture.id);
    glBindTexture(GL_TEXTURE_2D, texture.id); 
     // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    unsigned char *data = stbi_load(fileName, &texture.width, &texture.height, &texture.nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        //glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    return texture;
}

void RenderTexture(Texture2D texture, Rectangle source, Rectangle dest, Shader& shader, glm::mat4 projection, glm::mat4 view, GLuint VAO)
{
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(dest.x + dest.width / 2, dest.y + dest.height / 2, 0.0f)); 
    model = glm::scale(model, glm::vec3(100.0f, 100.0f, 1.0f));   

    shader.use();
   
    glUniformMatrix4fv(shader.getUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(shader.getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
    
    const float u1 = source.x / texture.width;
    const float v1 = source.y / texture.height;
    const float u2 = (source.width + source.x) / texture.width;
    const float v2 = source.height / texture.height;
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glBindVertexArray(VAO);
    glUniformMatrix4fv(shader.getUniformLocation("model"), 1, GL_FALSE, glm::value_ptr(model));
    glUniform4f(shader.getUniformLocation("uvRegion"), u1, v1, u2, v2);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); 
    glBindVertexArray(0);

}

void RenderText(stbtt_bakedchar* cdata, Shader& shader, const std::string& text, float x, float y, GLuint fontTex, float scale, glm::mat4& projection, GLuint textVAO, GLuint textVBO)
{
    shader.use();
    glBindTexture(GL_TEXTURE_2D, fontTex);

    glUniformMatrix4fv(shader.getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(textVAO);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float xpos = x;
    float ypos = y;

    for (char c : text)
    {
        if (c < 32 || c >= 128) continue;

        stbtt_aligned_quad q;
        stbtt_GetBakedQuad(cdata, 512, 512, c - 32, &xpos, &ypos, &q, 1);

        float vertices[6][4] = {
            { q.x0 * scale, q.y0 * scale, q.s0, q.t0 },
            { q.x1 * scale, q.y0 * scale, q.s1, q.t0 },
            { q.x1 * scale, q.y1 * scale, q.s1, q.t1 },

            { q.x0 * scale, q.y0 * scale, q.s0, q.t0 },
            { q.x1 * scale, q.y1 * scale, q.s1, q.t1 },
            { q.x0 * scale, q.y1 * scale, q.s0, q.t1 }
        };

        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glUseProgram(0);
}



int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "Engine", NULL, NULL);
    if (!window)
    {
        std::cerr << "Erro ao criar janela\n";
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Erro ao inicializar GLAD\n";
        return -1;
    }

    getOpenGLVersionInfo();
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    //======================font load====================================

    unsigned int fontTexture;
    unsigned char ttf_buffer[1 << 20]; // 1MB de buffer pra fonte
    unsigned char bitmap[512 * 512];   // espaço pra gerar os caracteres
    stbtt_bakedchar cdata[96];         // de ' ' até '~'

    // Lê o arquivo da fonte
    FILE *fontFile = fopen("../roboto.ttf", "rb");
    fread(ttf_buffer, 1, 1 << 20, fontFile);
    fclose(fontFile);

    // Gera o bitmap com os caracteres (ASCII de 32 a 128)
    stbtt_BakeFontBitmap(ttf_buffer, 0, 68.0f, bitmap, 512, 512, 32, 96, cdata);
    //stbtt_BakeFontBitmap(ttf_buffer, 0, 32.0f, bitmap, 512, 512, 32, 96, cdata);

    // Cria a textura OpenGL
    glGenTextures(1, &fontTexture);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


    //==============================================================

    // ====== Shader program ======
    Shader ourShader("../shaders/shader.vs","../shaders/shader.fs");
    Shader textShader("../shaders/text.vs","../shaders/text.fs");

    // ====== Dados do triângulo + índices ======
    float vertices[] = {
        // pos               // cor              // tex coords
        0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
        0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    unsigned int indices[] = {
        // note that we start from 0!
        0, 1, 3, // first Triangle
        1, 2, 3  // second Triangle
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // ===== Enviar dados pra GPU =====
    glBindVertexArray(VAO);

    // Enviar dados pra GPU Vértices
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Enviar dados pra GPU Índices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Configura atributo de posição
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);


    unsigned int textVAO, textVBO;
    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    
    glBindVertexArray(textVAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW); // 6 vértices, 4 atributos (x, y, u, v)

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    
    // Desvincular VBO (mas não EBO!)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    Texture2D texture = LoadTexture("../test.png");
    Texture2D texture2 = LoadTexture("../wall.jpg");

    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(WIDTH), static_cast<float>(HEIGHT), 0.0f); // tela 800x600
    glm::mat4 view = glm::mat4(1.0f); // câmera fixa


    // Loop principal
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.1f, 0.4f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

            
        glEnable(GL_SCISSOR_TEST);
        glScissor(10, (GetWindowSize(window).y - 100) - 10, 100, 100); // área de recorte
        glClearColor(0.6f, 0.4f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // ... desenhar aqui
        // create transformations     
        Rectangle sourceRec = {0.0f, 0.0f, 100, 100};
        Rectangle destRec = {0, 0, 100, 100};
        RenderTexture(texture, sourceRec, destRec, ourShader, projection,  view,  VAO);

        glDisable(GL_SCISSOR_TEST);   // volta ao normal
        
        
        
        sourceRec = {0.0f, 0.0f, 100, 100};
        destRec = { 400 - 100 / 2, 300 - 100 / 2, 100, 100};
        RenderTexture(texture, sourceRec, destRec, ourShader, projection,  view,  VAO);

        sourceRec = {0.0f, 0.0f, 100, 100};
        destRec = {300, 50, 100, 100};
        RenderTexture(texture2, sourceRec, destRec, ourShader, projection,  view,  VAO);
        
        RenderText(cdata, textShader, "Hello World! 3 Ç", 50.0f, 50.0f, fontTexture, 1.0f, projection, textVAO, textVBO);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    //glDeleteProgram(shaderProgram);
    
    glfwTerminate();
    return 0;
}