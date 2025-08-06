
#define TEST
#ifdef TEST



/*
#include "window.h"
#include "sprite.h"
#include "text.h"
#include "shader.h"
#include "font.h"
#include <iostream>


int main()
{

    const int screenWidth = 800;
    const int screenHeight = 600;

    Fusion::Window window;
    // Para este exemplo, a resolução virtual é a mesma da janela.
    window.InitWindow("FusionLibrary [Shaders] - Exemplo de Contorno", screenWidth, screenHeight);

    // Carrega a textura e a fonte
    // (Assumindo que os arquivos estão nas pastas corretas relativas ao executável)
    Fusion::Texture fudesumiTexture("../assets/fudesumi.png");
    Fusion::Font defaultFont("../assets/NataSans-Regular.ttf", 20, 224);

    // 2. Carregar o Shader Customizado
    //--------------------------------------------------------------------------------------
    // Carregamos o shader passando uma string vazia para o vertex shader.
    // A nova lógica da sua classe Shader usará o vertex shader padrão automaticamente.
    Fusion::Shader outlineShader(Fusion::DEFAULT_SHADER);
    outlineShader.LoadShader("", "../shaders/outline.fs");


    // 3. Preparar e Enviar os Uniforms Iniciais para o Shader
    //--------------------------------------------------------------------------------------
    float outlineSize = 5.0f;
    Fusion::Color outlineColor = { 1.0f, 0.0f, 0.0f, 1.0f }; // Vermelho
    Fusion::Vector2f textureSize = { (float)fudesumiTexture.GetSize().width, (float)fudesumiTexture.GetSize().height };

    // Ativa o shader para poder enviar os uniforms
    outlineShader.use();
    outlineShader.SetFloat("outlineSize", outlineSize);
    outlineShader.SetVec4("outlineColor", outlineColor.r, outlineColor.g, outlineColor.b, outlineColor.a);
    outlineShader.SetVec2("textureSize", textureSize.x, textureSize.y);


    // Cria o sprite que usará a textura
    Fusion::Sprite fudesumiSprite(fudesumiTexture);
    fudesumiSprite.SetPosition(screenWidth / 2.0f - fudesumiTexture.GetSize().width / 2.0f, 30.0f);

    // Prepara os textos da UI
    Fusion::Text text1(defaultFont);
    text1.SetText("\tShader-based\ntexture outline");
    text1.SetPosition(10, 10);
    text1.SetColor({0.5f, 0.5f, 0.5f, 1.0f}); // GRAY
    text1.SetSpacing(3);
    text1.SetScale(5);

    Fusion::Text text2(defaultFont);
    text2.SetText("Scroll mouse wheel to change outline size");
    text2.SetPosition(10, 72);
    text2.SetColor({0.5f, 0.5f, 0.5f, 1.0f}); // GRAY

    Fusion::Text text3(defaultFont);
    text3.SetColor({0.69f, 0.19f, 0.19f, 1.0f}); // MAROON
    text3.SetPosition(10, 120);

    //--------------------------------------------------------------------------------------

    // Loop principal do jogo
    while (!window.WindowShouldClose())
    {
        // 4. Atualização
        //----------------------------------------------------------------------------------
       // outlineSize += GetMouseWheelMove(); // Simulado
       // if (outlineSize < 1.0f) outlineSize = 1.0f;

        // Envia o valor atualizado do 'outlineSize' para o shader a cada quadro
        outlineShader.use();
        outlineShader.SetFloat("outlineSize", outlineSize);

        // Atualiza o texto que mostra o tamanho
        char buffer[64];
        snprintf(buffer, sizeof(buffer), "Outline size: %i px", (int)outlineSize);
        text3.SetText(buffer);
        //----------------------------------------------------------------------------------

        // 5. Desenho
        //----------------------------------------------------------------------------------
        window.BeginDrawing();

            window.Clear({0.96f, 0.96f, 0.96f, 1.0f}); // RAYWHITE

            // Aplica o shader customizado apenas a esta chamada de desenho
             window.Draw(fudesumiSprite, &outlineShader);
            //window.Draw(fudesumiSprite);

            // Os textos são desenhados com o shader padrão, pois não passamos um customizado
            window.Draw(text1);
           // window.Draw(text2);
           // window.Draw(text3);

            // Nota: FusionLibrary ainda não tem DrawFPS, então foi omitido.

        window.EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // 6. Desinicialização
    //--------------------------------------------------------------------------------------
    // Na FusionLibrary, a limpeza é automática graças ao RAII.
    // Os destrutores de Window, Texture, Font e Shader serão chamados quando 'main' terminar.
    //--------------------------------------------------------------------------------------

    return 0;
}




*/









/*
#include "window.h"
#include "sprite.h"
#include "camera2D.h" // Inclua a câmera

int main()
{
    Fusion::Window window;
    window.InitWindow("Camera Test", 800, 600);
    
    Fusion::Texture tex("../assets/test2.png");
    Fusion::Sprite sprite(tex);
    sprite.SetPosition(200, 200);

    // Cria e configura a câmera
    Fusion::Camera2D camera;
    camera.target = { sprite.GetPosition().x, sprite.GetPosition().y }; // Câmera segue o sprite
    camera.offset = { 400, 300 }; // Centraliza a visão no meio da tela 800x600
    camera.zoom = 1.0f;

    while (!window.WindowShouldClose())
    {
        // --- LÓGICA DE ATUALIZAÇÃO ---
        // Gira a câmera lentamente
        camera.rotation += 0.5f; 
        // Aumenta e diminui o zoom
        camera.zoom = sin(window.GetTime()) + 1.5f; 

        // --- DESENHO ---
        window.BeginDrawing();
        window.Clear({0.1f, 0.1f, 0.1f, 1.0f});

        // Inicia o modo de câmera
        window.BeginMode2D(camera);

            // Desenhe aqui tudo que deve ser afetado pela câmera (o "mundo" do jogo)
            window.Draw(sprite);

            // Desenha um "chão" para vermos a rotação
            Fusion::Sprite floor(tex);
            floor.SetPosition(0, 400);
            floor.SetSize(800, 50);
            window.Draw(floor);
        
        // Termina o modo de câmera
        window.EndMode2D();
        
        // Desenhe aqui a UI e outros elementos que NÃO devem ser afetados pela câmera
        // Por exemplo:
        // Fusion::Text scoreText(...);
        // scoreText.SetPosition(10, 10);
        // window.Draw(scoreText);

        window.EndDrawing();
    }
    
    return 0;
}
*/



#include "window.h"
#include "sprite.h"
#include "text.h"
#include "render_texture.h" // Inclua o novo header

int main()
{
    Fusion::Window window;
    window.InitWindow("Texture Mode Test", 800, 600);
    
    Fusion::Texture playerTexture("../assets/test2.png");
    Fusion::Sprite player(playerTexture);
    player.SetPosition(50, 50);

    // 1. Crie o alvo da renderização (com metade do tamanho da tela)
    Fusion::RenderTexture target(400, 300);

    // 2. Crie um sprite para mostrar o resultado da textura
    Fusion::Sprite screenSprite(*target.GetTexture());

    Fusion::Font font("../assets/NataSans-Regular.ttf", 20, 224);
    Fusion::Text text(font);

    while (!window.WindowShouldClose())
    {
        // --- DESENHAR NA TEXTURA ---
        window.BeginTextureMode(target);
            // Limpa a textura com uma cor de fundo
            window.Clear(Fusion::Color{0.2f, 0.2f, 0.8f, 1.0f});
            // Desenha o jogador dentro da textura
            window.BeginBlendMode(Fusion::BLEND_MULTIPLIED);
                window.Draw(player);
            window.EndBlendMode();
            window.Draw(text);
            text.SetText("Texto renderizado no texture render");
        window.EndTextureMode();
        // --- FIM DO DESENHO NA TEXTURA ---


        // --- DESENHAR NA TELA ---
        window.BeginDrawing();
            window.Clear(Fusion::Color{0.1f, 0.4f, 0.3f, 1.0f});
            
            // Desenha a textura que acabamos de renderizar na tela.
            // NOTA: Texturas de FBO ficam de cabeça para baixo.
            // Para corrigir, invertemos a coordenada Y da fonte do sprite. obs: isso não é mais necessário
           // screenSprite.SetSource({0, (float)target.GetTexture()->GetSize().height, 
           //                         (float)target.GetTexture()->GetSize().width, -(float)target.GetTexture()->GetSize().height});  

            screenSprite.SetPosition(200, 150);
            screenSprite.SetSize(600, 500);

            window.Draw(screenSprite);
            window.Draw(player);

        window.EndDrawing();
    }
   
    return 0;
}



/*

#include "window.h"
#include "sprite.h"
#include "text.h"

int main(int argc, char const *argv[])
{
    Fusion::Window window;
    
    
    window.InitWindow("teste", 800, 600);
    
    Fusion::Texture texture("../assets/test2.png");
    Fusion::Sprite sprite(texture);
   

    Fusion::Font font("../assets/NataSans-Regular.ttf", 32, 224);
    Fusion::Text text(font);
    text.SetScale(1.0f);
    text.SetSpacing(1.5f);
    
    sprite.SetPosition(50.0f, 50.0f);


    text.SetText("Oi este texto está na posição certa!!! OK");
    float x = 0;

while (!window.WindowShouldClose())
{
    window.BeginDrawing();
    window.Clear(Fusion::Color {0.1f, 0.4f, 0.3f, 1.0f});

    // Desenha um sprite normal
    sprite.SetPosition(150.0f, 250.0f);
    sprite.SetRotation(45.0f);
    window.Draw(sprite);

    x += 0.3f;
    
    // --- NOVO MODO SCISSOR ---
    // Ativa uma área de recorte de 200x200 no canto superior esquerdo
    window.BeginScissorMode(10, 10, 200, 200);

        // Você pode até limpar a área do Scissor com uma cor diferente
        window.Clear(Fusion::Color {0.6f, 0.4f, 0.3f, 1.0f});

        // Este sprite só será visível (e será cortado) dentro do retângulo de 200x200
        sprite.SetPosition(x, 50.0f);
        sprite.SetRotation(0);
        sprite.SetSize(150.0f, 150.0f);
        window.Draw(sprite);

    window.EndScissorMode();
    // -------------------------

    // Desenha outro texto/sprite fora do modo Scissor
    text.SetPosition(300.0f, 300.0f);
    window.Draw(text);
    
    window.EndDrawing();
}

*/


    // while (!window.WindowShouldClose())
    // {
    //     xx += 0.5f ;
    //     const float x = text.MeasureText().x;
    //     const float y = text.MeasureText().y;
    //     sprite.SetRotation( 0);
    //     text.SetPosition(400.0f - x / 2 , (300.0f - y /2));
    //     text.SetRotation( text.GetRotation() +  1.0f);
    //     window.BeginDrawing();
    //     window.Clear(Fusion::Color {0.1f, 0.4f, 0.3f, 1.0f});
    //     sprite.SetPosition(xx, 0.0f);
    //     sprite.SetSize(100.0f, 100.0f);
    //     // sprite.SetPosition(250.0f, 50.0f);
        
    //     // sprite.SetSize(100.0f, 100.0f);
    //     // window.Draw(sprite);
    //     window.Draw(sprite);
    //     window.Draw(text);
    //     window.EndDrawing();
    // }
    
//    return 0;
//}  



#else



#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <math.h>
#include <vector>
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

struct Vertex
{
    glm::vec3 position;
    glm::vec4 color;
    glm::vec2 texCoords;
};

// --- Batch Renderer State ---
constexpr size_t MAX_QUADS = 1000;
constexpr size_t MAX_VERTICES = MAX_QUADS * 4;
constexpr size_t MAX_INDICES = MAX_QUADS * 6;

GLuint batchVAO = 0;
GLuint batchVBO = 0;
GLuint batchEBO = 0;

Vertex vertices[MAX_VERTICES];
uint32_t vertexCount = 0;
GLuint currentTextureID = 0;
Shader* currentShader = nullptr; 


glm::mat4 projection;
// --------------------------


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

struct Color
{
    float r;
    float g;
    float b;
    float a;
};


#include <map>
#include "utf8.h"
#include <algorithm> // Para std::min e std::max

struct FontData
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
    //glViewport(0, height - HEIGHT, WIDTH, HEIGHT);
    glViewport(0, 0, width, height);
    projection = glm::ortho(0.0f, static_cast<float>(width), static_cast<float>(height), 0.0f); 
}


void getOpenGLVersionInfo()
{
    std::cout << "Vendor " << glGetString(GL_VENDOR) << "\n";
    std::cout << "Renderer " << glGetString(GL_RENDERER) << "\n";
    std::cout << "Version " << glGetString(GL_VERSION) << "\n";
    std::cout << "Shading Language " << glGetString(GL_SHADING_LANGUAGE_VERSION) << "\n";

    int value;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &value);   //Returns 1 value
    std::cout << "Max Texture Size " << value << "\n";

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
        GLenum format = (texture.nrChannels == 4) ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, texture.width, texture.height, 0, format, GL_UNSIGNED_BYTE, data);
        //glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    return texture;
}


void InitBatchRenderer()
{
    glGenVertexArrays(1, &batchVAO);
    glBindVertexArray(batchVAO);

    // VBO (Vertex Buffer Object) - Para os dados dos vértices
    glGenBuffers(1, &batchVBO);
    glBindBuffer(GL_ARRAY_BUFFER, batchVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), nullptr, GL_DYNAMIC_DRAW); // Usamos DYNAMIC_DRAW

    // EBO (Element Buffer Object) - Para os índices
    glGenBuffers(1, &batchEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batchEBO);

    // Gerar os índices para todos os quads possíveis. O padrão é 0,1,2, 2,3,0 -> 4,5,6, 6,7,4, etc.
    GLuint indices[MAX_INDICES];
    uint32_t offset = 0;
    for (size_t i = 0; i < MAX_INDICES; i += 6)
    {
        indices[i + 0] = 0 + offset;
        indices[i + 1] = 1 + offset;
        indices[i + 2] = 2 + offset;
        indices[i + 3] = 2 + offset;
        indices[i + 4] = 3 + offset;
        indices[i + 5] = 0 + offset;
        offset += 4;
    }
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    // Configura os atributos do vértice
    // Posição
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    // cores
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color)); 
    // Coordenadas de Textura
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));

    glBindVertexArray(0);
}


void Flush(); // Declaração antecipada

void BeginDrawing()
{
    vertexCount = 0;
    currentTextureID = 0;
}

void EndDrawing()
{
    if (vertexCount > 0)
    {
        Flush();
    }
}

void Flush()
{
    if (vertexCount == 0) return;

    if (currentShader) currentShader->use();


    glBindVertexArray(batchVAO);
    glBindTexture(GL_TEXTURE_2D, currentTextureID);

    // Envia os dados acumulados para o VBO na GPU
    glBindBuffer(GL_ARRAY_BUFFER, batchVBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, vertexCount * sizeof(Vertex), vertices);

    // Desenha tudo de uma vez!
    // O número de índices é (vertexCount / 4) * 6
    glDrawElements(GL_TRIANGLES, (vertexCount / 4) * 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    
    // Reseta o contador para o próximo lote
    vertexCount = 0;
}


void RenderTexture(Texture2D texture, Rectangle source, Rectangle dest, Vector2 origin, float rotation, Color color)
{
    // A lógica de flush por mudança de textura ou shader permanece a mesma
    if (vertexCount >= MAX_VERTICES || (texture.id != currentTextureID && currentTextureID != 0))
    {
        Flush();
    }
    
    
    // Define a textura para o novo lote (se necessário)
    if (vertexCount == 0)
    {
        currentTextureID = texture.id;
    }

    // --- MÁGICA DA TRANSFORMAÇÃO ---

    // 1. Construir a matriz 'model'
    glm::mat4 model = glm::mat4(1.0f);
    // A ordem importa: primeiro movemos para a posição final, depois rotacionamos em torno da origem
    model = glm::translate(model, glm::vec3(dest.x, dest.y, 0.0f));
    model = glm::translate(model, glm::vec3(origin.x, origin.y, 0.0f)); // Move o pivô de rotação para a origem do objeto
    model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)); // Rotaciona
    model = glm::translate(model, glm::vec3(-origin.x, -origin.y, 0.0f)); // Move o pivô de volta
    model = glm::scale(model, glm::vec3(dest.width, dest.height, 1.0f)); // Aplica a escala (tamanho)

    // 2. Definir os cantos de um quad local (de 0,0 a 1,1)
    glm::vec4 positions[4] = {
        { 0.0f, 0.0f, 0.0f, 1.0f }, // Top-left
        { 1.0f, 0.0f, 0.0f, 1.0f }, // Top-right
        { 1.0f, 1.0f, 0.0f, 1.0f }, // Bottom-right
        { 0.0f, 1.0f, 0.0f, 1.0f }  // Bottom-left
    };

    // 3. Transformar cada canto pela matriz 'model'
    for (int i = 0; i < 4; i++)
    {
        positions[i] = model * positions[i];
    }
    
    // Coordenadas de textura (UVs) - não mudam
    const float u1 = source.x / texture.width;
    const float v1 = source.y / texture.height;
    const float u2 = (source.x + source.width) / texture.width;
    const float v2 = (source.y + source.height) / texture.height;
    glm::vec2 uv1 = { u1, v1 }; // UV Top-left
    glm::vec2 uv2 = { u2, v1 }; // UV Top-right
    glm::vec2 uv3 = { u2, v2 }; // UV Bottom-right
    glm::vec2 uv4 = { u1, v2 }; // UV Bottom-left

    // Cor
    glm::vec4 glmColor = { color.r, color.g, color.b, color.a };

    // 4. Adicionar os vértices JÁ TRANSFORMADOS ao lote
    vertices[vertexCount++] = { glm::vec3(positions[3]), glmColor, uv4 }; // Vértice 0: Bottom-left
    vertices[vertexCount++] = { glm::vec3(positions[2]), glmColor, uv3 }; // Vértice 1: Bottom-right
    vertices[vertexCount++] = { glm::vec3(positions[1]), glmColor, uv2 }; // Vértice 2: Top-right
    vertices[vertexCount++] = { glm::vec3(positions[0]), glmColor, uv1 }; // Vértice 3: Top-left
}

FontData LoadFont(const char* fontPath)
{
    FontData font ;

    
    std::vector<unsigned char> bitmap(512 * 512);
    std::vector<unsigned char> ttf_buffer; // Será redimensionado após ler o arquivo
    
    FILE* fontFile = fopen(fontPath, "rb");
    if (!fontFile) {
        std::cerr << "Erro ao abrir arquivo de fonte: " << fontPath << std::endl;
        return font;
    }

    // Uma forma mais segura de ler o arquivo
    fseek(fontFile, 0, SEEK_END);
    long fileSize = ftell(fontFile);
    fseek(fontFile, 0, SEEK_SET);

    ttf_buffer.resize(fileSize);


    fread(ttf_buffer.data(), 1, fileSize, fontFile);
    fclose(fontFile);

    stbtt_pack_context pack_context;
    if (!stbtt_PackBegin(&pack_context, bitmap.data(), 512, 512, 0, 1, nullptr)) {
        std::cerr << "Erro ao inicializar o stb_truetype pack context." << std::endl;
        return font;
    }

    stbtt_PackSetOversampling(&pack_context, 2, 2); // Melhora a qualidade da rasterização

    // 1. Carrega a faixa ASCII básica (caracteres imprimíveis)
    stbtt_packedchar ascii_chars[254]; //95
    stbtt_PackFontRange(&pack_context, ttf_buffer.data(), 0, 32.0f, 32, 254, ascii_chars);
 
    
    // Mapeia os dados dos caracteres para fácil acesso
    for(int i = 0; i < 254; ++i) 
    {
        font.charData[32 + i] = ascii_chars[i];
    }


    glGenTextures(1, &font.fontTexture);
    glBindTexture(GL_TEXTURE_2D, font.fontTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    return font;
}


void RenderText(Shader& shader, FontData& font, const std::string& text, float x, float y, float scale, Vector2 origin, float rotation, Color color)
{
    // Se o buffer estiver cheio, ou se a textura da fonte for diferente da textura atual, dá flush.
    // Isso vai acontecer naturalmente ao alternar entre desenhar sprites e texto.
    if (vertexCount >= MAX_VERTICES || (font.fontTexture != currentTextureID && currentTextureID != 0))
    {
        Flush();
    }

    currentShader = &shader;
    currentTextureID = font.fontTexture;

    // Define os uniformes específicos do shader de texto
    shader.use();

    glUniformMatrix4fv(shader.getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniform4f(shader.getUniformLocation("textColor"), color.r, color.g, color.b, color.a);

    glm::vec3 pivot = glm::vec3(x + origin.x, y + origin.y, 0.0f);

    // 2. Cria a matriz de rotação que gira em torno do pivô
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::translate(transform, pivot);
    transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
    transform = glm::translate(transform, -pivot);

    // Converte a cor da engine para glm::vec4
    glm::vec4 glmColor = { color.r, color.g, color.b, color.a };

    float xpos = x;
    float ypos = y;
    const utf8_int8_t* p = reinterpret_cast<const utf8_int8_t*>(text.c_str());

    while (*p) 
    {
        int32_t codepoint = 0;
        const utf8_int8_t* next_p = utf8codepoint(p, &codepoint);
        if (codepoint == 0) break;

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
        stbtt_GetPackedQuad(&pc, 512, 512, 0, &xpos, &ypos, &q, 0);

        // Checa se ainda há espaço no buffer para mais um quad
        if (vertexCount >= MAX_VERTICES) Flush();

        // Calcula os 4 vértices para o caractere atual e adiciona ao lote
        glm::vec4 p1 = { q.x0 * scale, q.y0 * scale, 0.0f, 1.0f }; // Top-left
        glm::vec4 p2 = { q.x1 * scale, q.y0 * scale, 0.0f, 1.0f }; // Top-right
        glm::vec4 p3 = { q.x1 * scale, q.y1 * scale, 0.0f, 1.0f }; // Bottom-right
        glm::vec4 p4 = { q.x0 * scale, q.y1 * scale, 0.0f, 1.0f }; // Bottom-left

        p1 = transform * p1;
        p2 = transform * p2;
        p3 = transform * p3;
        p4 = transform * p4;

        glm::vec2 uv1 = { q.s0, q.t0 }; // UV Top-left
        glm::vec2 uv2 = { q.s1, q.t0 }; // UV Top-right
        glm::vec2 uv3 = { q.s1, q.t1 }; // UV Bottom-right
        glm::vec2 uv4 = { q.s0, q.t1 }; // UV Bottom-left

        vertices[vertexCount++] = { glm::vec3(p4), glmColor, uv4 }; // Vértice 0: Bottom-left
        vertices[vertexCount++] = { glm::vec3(p3), glmColor, uv3 }; // Vértice 1: Bottom-right
        vertices[vertexCount++] = { glm::vec3(p2), glmColor, uv2 }; // Vértice 2: Top-right
        vertices[vertexCount++] = { glm::vec3(p1), glmColor, uv1 }; // Vértice 3: Top-left


        p = next_p;
    }
}

Vector2 MeasureText(FontData& font, const std::string& text, float scale)
{
    Vector2 size = {0.0f, 0.0f};
    if (text.empty()) {
        return size;
    }

    float xpos = 0.0f;
    float ypos = 0.0f;

    // Variáveis para rastrear a caixa delimitadora vertical do texto
    float minY = 0.0f, maxY = 0.0f;

    const utf8_int8_t* p = reinterpret_cast<const utf8_int8_t*>(text.c_str());

    while (*p) 
    {
        int32_t codepoint = 0; 
        const utf8_int8_t* next_p = utf8codepoint(p, &codepoint);

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

    InitBatchRenderer();

    // --- ESTADO DE RENDERIZAÇÃO PADRÕES ---
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // --------------------------------------------------


    //======================font load====================================
 
    FontData myFont = LoadFont("../roboto.ttf");  
    //==============================================================
    
    // ====== Shader program ======
    //Shader ourShader("../shaders/shader.vs","../shaders/shader.fs");
    //Shader textShader("../shaders/text.vs","../shaders/text.fs");
    Shader ourShader;
    Shader textShader;
    ourShader.LoadShader(DEFAULT_SHADER);
    textShader.LoadShader(TEXT_DEFAULT_SHADER);
    //==============================================================
    
    
    //======= load textures =======================
    Texture2D texture = LoadTexture("../assets/test.png");
    Texture2D texture2 = LoadTexture("../assets/wall.jpg");
    Texture2D texture3 = LoadTexture("../assets/test2.png");
    //==============================================================
    
    projection = glm::ortho(0.0f, static_cast<float>(WIDTH), static_cast<float>(HEIGHT), 0.0f); // tela 800x600
    //glm::mat4 view = glm::mat4(1.0f); // câmera fixa


    
    float x = 0;
    // Loop principal
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);

        
        x += 0.5f;
        
        BeginDrawing(); // Prepara para um novo frame
        
            glClearColor(0.1f, 0.4f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // TODAS AS SUAS CHAMADAS DE DESENHO VÃO AQUI
            // Elas não desenham nada ainda, só acumulam vértices.
            Rectangle sourceRec = {0.0f, 0.0f, 100 , 100};
            
            Rectangle destRec = {300, 0, 100, 100};
            Color color = {1.0f, 1.0f, 1.0f, 1.0f};

            ourShader.use();
            glUniformMatrix4fv(ourShader.getUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));

            if (currentShader == nullptr || ourShader.ID != currentShader->ID)
            {
                currentShader = &ourShader;
            }

            destRec = {300, 0, 100, 100};
            color = {1.0f, 1.0f, 1.0f, 1.0f};
            RenderTexture(texture2, sourceRec, destRec,Vector2{0,0}, 0, color);


            Flush();
            glEnable(GL_SCISSOR_TEST);
            glScissor(10, (GetWindowSize(window).y - 100) - 10, 100, 100); // área de recorte
            glClearColor(0.6f, 0.4f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
            // ... desenhar aqui
            destRec = {0, 0, 100, 100};
            color = {1.0f, 1.0f, 1.0f, 0.3f};
            RenderTexture(texture3, sourceRec, destRec,Vector2{0,0}, 0, color); // renderiza algo dentro da area de recorte
            
            Flush();
            glDisable(GL_SCISSOR_TEST);   // volta ao normal
            

            destRec = { 400, 200, 150, 150};
            color = {1.0f, 0.5f, 0.5f, 1.0f};
            RenderTexture(texture3, sourceRec, destRec,Vector2{0,0}, 0, color);
            
            // Tenta desenhar com a mesma textura, será adicionado ao mesmo lote
            destRec = {100, 300, 50, 50};
            color = {1.0f, 1.0f, 1.0f, 0.3f};
            
            // Rotaciona continuamente com base no tempo
            float rotation = (float)glfwGetTime() * 45.0f; // 45 graus por segundo
            RenderTexture(texture3, sourceRec, destRec,Vector2{25, 25}, rotation, color);

            
            const char* texto = "Um texto centralizado ficou legal !!!";  // mensurar texto 
            Vector2 tamanhoDoTexto = MeasureText(myFont, texto, 1.0f);
            float x = (WIDTH / 2.0f) - (tamanhoDoTexto.x / 2.0f);
            float y = (HEIGHT /2.0f) - (tamanhoDoTexto.y / 2.0f);
            color = {1.0f, 1.0f, 1.0f, 1.0f};
            
            RenderText(textShader, myFont, texto, x, y, 1.0f, Vector2{0, 0}, rotation, color);

        EndDrawing(); // Desenha tudo que foi acumulado!

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    // Cleanup
    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
    //glDeleteProgram(shaderProgram);
    //glDisable(GL_BLEND);
    
    glfwTerminate();
    return 0;
}

#endif