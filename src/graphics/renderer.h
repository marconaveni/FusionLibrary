#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <vector>
#include "shader.h"
#include "fusion_math.h"
#include "vertex.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace Fusion
{

    class Font;
    class Text;
    class Texture;
    class Sprite;

    class Renderer
    {
    public:
        Renderer();
        ~Renderer();

        void BeginRender();
        void EndRender();

        void DrawTexture(const Sprite &sprite);
        void DrawText(const Text &text);

        void BeginScissorMode(int x, int y, int width, int height);
        void EndScissorMode();

        void Init();

    private:
        void InitBatch();
        void Flush();

        // Todo o estado do batch renderer vai para cá
        GLuint m_BatchVAO = 0;
        GLuint m_BatchVBO = 0;
        GLuint m_BatchEBO = 0;

        std::vector<Vertex> m_Vertices;
        uint32_t m_VertexCount = 0;

        GLuint m_CurrentTextureID = 0;
        Shader *m_CurrentShader = nullptr;

        Shader m_TextureShader;
        Shader m_TextShader;

        glm::mat4 m_Projection;

        size_t m_MaxQuads;
        size_t m_MaxVertices;
        size_t m_MaxIndices;
    };

}

#endif