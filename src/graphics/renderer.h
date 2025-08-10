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

        void Shutdown();

        void BeginRender();
        void EndRender();

        void DrawTexture(const Sprite &sprite, Shader *customShader = nullptr);
        void DrawText(const Text &text, Shader *customShader = nullptr);

        void DrawRectangle(int x, int y, int width, int height, Color color);
        void DrawCircle(Vector2f center, float radius, Color color);
        void DrawCircleLines(Vector2f center, float radius, Color color); 
        void DrawTriangle(Vector2f v1, Vector2f v2, Vector2f v3, Color color); 
        void DrawLine(Vector2f startPos, Vector2f endPos, float thick, Color color);   
        void DrawRectangleLines(int x, int y, int width, int height, float thick, Color color);

        void BeginScissorMode(int x, int y, int width, int height);
        void EndScissorMode();

        void BeginBlendMode(BlendMode mode);
        void EndBlendMode();

        void Init(int width, int height);
        void SetProjection(const glm::mat4 &projection);

        void SetViewMatrix(const glm::mat4 &view) { m_View = view; }
        void ResetViewMatrix() { m_View = glm::mat4(1.0f); }

    private:
        void InitBatch();
        void CreateDefaultTexture();
        void Flush();
        void CheckFlushShape();

        // Todo o estado do batch renderer vai para cá
        GLuint m_BatchVAO = 0;
        GLuint m_BatchVBO = 0;
        GLuint m_BatchEBO = 0;

        std::vector<Vertex> m_Vertices;
        uint32_t m_VertexCount = 0;

        GLuint m_DefaultTextureID = 0;
        GLuint m_CurrentTextureID = 0;
        Shader *m_CurrentShader = nullptr;

        Shader m_TextureShader;
        Shader m_TextShader;

        glm::mat4 m_Projection;
        glm::mat4 m_View;
        bool m_IsCameraActive = false;

        size_t m_MaxQuads;
        size_t m_MaxVertices;
        size_t m_MaxIndices;
    };

}

#endif