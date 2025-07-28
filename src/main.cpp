
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

#include <map>
#include "utf8.h"
#include <algorithm> // Para std::min e std::max

struct Font
{
    unsigned int fontTexture;
    stbtt_fontinfo fontInfo;
    std::map<int, stbtt_packedchar> charData; // Mapeia codepoint para dados do glifo
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

Font LoadFont(const char* fontPath)
{
    Font font = {};
    unsigned char ttf_buffer[1 << 20]; // 1MB buffer para o arquivo .ttf
    unsigned char bitmap[512 * 512];   // 512x512 bitmap para o atlas

    FILE* fontFile = fopen(fontPath, "rb");
    if (!fontFile) {
        std::cerr << "Erro ao abrir arquivo de fonte: " << fontPath << std::endl;
        return font;
    }
    fread(ttf_buffer, 1, 1 << 20, fontFile);
    fclose(fontFile);

    stbtt_pack_context pack_context;
    if (!stbtt_PackBegin(&pack_context, bitmap, 512, 512, 0, 1, nullptr)) {
        std::cerr << "Erro ao inicializar o stb_truetype pack context." << std::endl;
        return font;
    }

    stbtt_PackSetOversampling(&pack_context, 2, 2); // Melhora a qualidade da rasterização

    // 1. Carrega a faixa ASCII básica (caracteres imprimíveis)
    stbtt_packedchar ascii_chars[254]; //95
    stbtt_PackFontRange(&pack_context, ttf_buffer, 0, 32.0f, 32, 254, ascii_chars);
 
    
    // Mapeia os dados dos caracteres para fácil acesso
    for(int i = 0; i < 254; ++i) 
    {
        font.charData[32 + i] = ascii_chars[i];
    }


    glGenTextures(1, &font.fontTexture);
    glBindTexture(GL_TEXTURE_2D, font.fontTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return font;
}


void RenderText(Font& font, Shader& shader, const std::string& text, float x, float y, float scale, glm::mat4& projection, GLuint textVAO, GLuint textVBO)
{
    shader.use();
    glBindTexture(GL_TEXTURE_2D, font.fontTexture);
    glUniformMatrix4fv(shader.getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(textVAO);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    float xpos = x;
    float ypos = y;

    const char* p = text.c_str();

    while (*p) 
    {
        // 1. Variável para RECEBER o codepoint
        int32_t codepoint = 0; 
        
        // 2. CORREÇÃO: A função retorna o ponteiro para o PRÓXIMO caractere.
        //    O codepoint é escrito em '&codepoint'.
        const char* next_p = utf8codepoint(p, &codepoint);

        if (codepoint == 0) {
            break;
        }

        auto itGlyph = font.charData.find(codepoint);
        if (itGlyph == font.charData.end()) {
            codepoint = '?'; 
            itGlyph = font.charData.find(codepoint);
            if (itGlyph == font.charData.end()) {
                p = next_p; // Avança o ponteiro mesmo se o glifo não for encontrado
                continue;
            }
        }
        
        stbtt_packedchar pc = itGlyph->second;
        stbtt_aligned_quad q;
        stbtt_GetPackedQuad(&pc, 512, 512, 0, &xpos, &ypos, &q, 0);

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

        // 3. CORREÇÃO: Avançamos o ponteiro para a posição retornada pela função.
        p = next_p;
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_BLEND);
    glUseProgram(0);
}

Vector2 MeasureText(Font& font, const std::string& text, float scale)
{
    Vector2 size = {0.0f, 0.0f};
    if (text.empty()) {
        return size;
    }

    float xpos = 0.0f;
    float ypos = 0.0f;

    // Variáveis para rastrear a caixa delimitadora vertical do texto
    float minY = 0.0f, maxY = 0.0f;

    const char* p = text.c_str();

    while (*p) 
    {
        int32_t codepoint = 0; 
        const char* next_p = utf8codepoint(p, &codepoint);

        if (codepoint == 0) {
            break;
        }

        auto itGlyph = font.charData.find(codepoint);
        if (itGlyph == font.charData.end()) {
            codepoint = '?'; 
            itGlyph = font.charData.find(codepoint);
            if (itGlyph == font.charData.end()) {
                p = next_p;
                continue;
            }
        }
        
        stbtt_packedchar pc = itGlyph->second;
        stbtt_aligned_quad q;

        // A MÁGICA ACONTECE AQUI:
        // Esta função calcula a posição do caractere (q) e atualiza o xpos com o avanço.
        // É a mesma função usada para renderizar, mas aqui só nos importam as métricas.
        stbtt_GetPackedQuad(&pc, 512, 512, 0, &xpos, &ypos, &q, 0);

        // Atualiza a altura máxima baseada no caractere mais alto e mais baixo
        minY = std::min(minY, q.y0 * scale);
        maxY = std::max(maxY, q.y1 * scale);

        p = next_p;
    }

    size.x = xpos * scale; // A largura final é a posição final de x
    size.y = (maxY - minY); // A altura é a diferença entre o ponto mais alto e o mais baixo

    return size;
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
 
    Font myFont = LoadFont("../roboto.ttf");  
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
        RenderTexture(texture, sourceRec, destRec, ourShader, projection,  view,  VAO); // renderiza algo dentro da area de recorte

        glDisable(GL_SCISSOR_TEST);   // volta ao normal
        
        
        sourceRec = {0.0f, 0.0f, 100, 100};
        destRec = { 400 - 100 / 2, 300 - 100 / 2, 100, 100}; // testando renderizar por textura
        RenderTexture(texture, sourceRec, destRec, ourShader, projection,  view,  VAO);

        sourceRec = {0.0f, 0.0f, 100, 100};
        destRec = {300, 50, 100, 100};
        RenderTexture(texture2, sourceRec, destRec, ourShader, projection,  view,  VAO);
        
        RenderText(myFont, textShader, "Olá Mundo com ç!", 50.0f, 50.0f, 1.0f, projection, textVAO, textVBO);
        
        const char* texto = "Um texto centralizado ficou legal !!!";  // mensurar texto 
        Vector2 tamanhoDoTexto = MeasureText(myFont, texto, 1.0f);
        float x = (WIDTH / 2.0f) - (tamanhoDoTexto.x / 2.0f);
        float y = (HEIGHT /2.0f) - (tamanhoDoTexto.y / 2.0f);
        RenderText(myFont, textShader, texto, x, y, 1.0f, projection, textVAO, textVBO);

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