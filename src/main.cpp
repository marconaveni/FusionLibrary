
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

void DrawTexturePro()
{

}

void RenderText(stbtt_bakedchar* cdata, Shader& shader, const std::string& text, float x, float y, GLuint fontTex, float scale, glm::mat4& projection, GLuint textVAO, GLuint textVBO)
{
    shader.use();
    glBindTexture(GL_TEXTURE_2D, fontTex);

    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

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
            { q.x0, q.y0, q.s0, q.t0 },
            { q.x1, q.y0, q.s1, q.t0 },
            { q.x1, q.y1, q.s1, q.t1 },

            { q.x0, q.y0, q.s0, q.t0 },
            { q.x1, q.y1, q.s1, q.t1 },
            { q.x0, q.y1, q.s0, q.t1 }
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
    stbtt_BakeFontBitmap(ttf_buffer, 0, 32.0f, bitmap, 512, 512, 32, 96, cdata);

    // Cria a textura OpenGL
    glGenTextures(1, &fontTexture);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


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









    // load and create a texture 
    // -------------------------
    unsigned int texture;
    // texture 1
    // ---------
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture); 
     // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;
    //stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

    unsigned char *data = stbi_load("../test.png", &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        //glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);


    // load and create a texture 
    // -------------------------
    unsigned int texture2;
    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2); 
     // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load image, create texture and generate mipmaps
    int width2, height2, nrChannels2;
    //stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

    unsigned char *data2 = stbi_load("../wall.jpg", &width2, &height2, &nrChannels2, 0);
    if (data2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, data2);
        //glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data2);




    // Desvincular VBO (mas não EBO!)
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(WIDTH), static_cast<float>(HEIGHT), 0.0f); // tela 800x600
    glm::mat4 view = glm::mat4(1.0f); // câmera fixa


    unsigned int modelLoc = glGetUniformLocation(ourShader.ID, "model");
    unsigned int viewLoc = glGetUniformLocation(ourShader.ID, "view");
    unsigned int projLoc = glGetUniformLocation(ourShader.ID, "projection");
    unsigned int uvLoc  = glGetUniformLocation(ourShader.ID, "uvRegion");

    Rectangle position1{0.0f, 0.0f, 100.0f, 100.0f}, position2{400.0f, 300.0f, 100.0f, 100.0f};

    float u1 = 0.0f;
    float v1 = 0.0f;
    float u2 = 0.0f;
    float v2 = 0.0f;

    // Loop principal
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        glClearColor(0.1f, 0.4f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        
        
        glEnable(GL_SCISSOR_TEST);
        glScissor(100, HEIGHT - (100 + 200), 200, 200); // área de recorte
        // ... desenhar aqui
        glClearColor(0.6f, 0.4f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);   // volta ao normal
        
        
        // create transformations         
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(position1.x + position1.width / 2, position1.y + position1.height / 2, 0.0f)); 
        model = glm::scale(model, glm::vec3(100.0f, 100.0f, 1.0f));     
        
        
        
        glm::mat4 model2 = glm::mat4(1.0f);
        model2 = glm::translate(model2, glm::vec3(position2.x + position2.width / 2, position2.y + position2.height / 2, 0.0f));  
        model2 = glm::scale(model2, glm::vec3(100.0f, 100.0f, 1.0f));     
        
        
        
        ourShader.use();

        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        
        //glBindVertexArray(VAO);
        
        u1 = -20.0f / width;
        v1 = 0.0f / height;
        u2 = (100.0f + -20.0f) / width;
        v2 = 100.0f / height;
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniform4f(uvLoc, u1, v1, u2, v2);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // <-- aqui usamos o EBO
        glBindVertexArray(0);
        
        
        u1 = 0.0f / width2;
        v1 = 0.0f / height2;
        u2 = 100.0f / width2;
        v2 = 100.0f / height2;
        
        glBindTexture(GL_TEXTURE_2D, texture2);
        glBindVertexArray(VAO);
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model2));
        glUniform4f(uvLoc, u1, v1, u2, v2);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // <-- aqui usamos o EBO
        glBindVertexArray(0);
        
        RenderText(cdata, textShader, "Hello World! 3", 50.0f, 50.0f, fontTexture, 1.0f, projection, textVAO, textVBO);
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