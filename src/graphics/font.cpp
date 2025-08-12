#include "font.h"
#include <glad/glad.h>
#include <iostream>
#include <vector>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

namespace Fusion
{
    Font::Font(const char *path, const float fontSize, int charCount)
        : m_FontInfo()
    {
        LoadFromFile(path, fontSize, charCount);
    }

    Font::Font()
    {
    }

    Font::~Font()
    {
        Unload();
    }

    void Font::SetSmooth(bool enable)
    {

        glBindTexture(GL_TEXTURE_2D, m_FontTextureID);

        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (enable) ? GL_LINEAR : GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (enable) ? GL_LINEAR : GL_NEAREST);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void Font::LoadFromMemory(unsigned char *data, size_t sizeData, float fontSize, int charCount)
    {
        std::vector<unsigned char> ttf_buffer;
        for (size_t i = 0; i < sizeData; i++)
        {
            ttf_buffer.push_back(data[i]);
        }
        LoadFont(ttf_buffer, fontSize, charCount);
    }

    void Font::LoadFromFile(const char *path, const float fontSize, int charCount)
    {

        std::vector<unsigned char> ttf_buffer;

        FILE *fontFile = fopen(path, "rb");
        if (!fontFile)
        {
            std::cerr << "Erro ao abrir arquivo de fonte: " << path << std::endl;
            return;
        }

        fseek(fontFile, 0, SEEK_END);
        long fileSize = ftell(fontFile);
        fseek(fontFile, 0, SEEK_SET);

        ttf_buffer.resize(fileSize);

        fread(ttf_buffer.data(), 1, fileSize, fontFile);
        fclose(fontFile);

        LoadFont(ttf_buffer, fontSize, charCount);
    }

    void Font::LoadFont(std::vector<unsigned char> &ttf_buffer, float fontSize, int charCount)
    {
        if (!stbtt_InitFont(&m_FontInfo, ttf_buffer.data(), 0))
        {
            std::cerr << "Erro ao inicializar informações da fonte stbtt." << std::endl;
            return;
        }

        // o tamanho do atlas está sendo definido com um tamanho fixo e isso
        // precisa ser corrigido no futuro calculando o tamanho dinamicamente
        // baseado no tamanho da fonte fontSize definido pelo usuário.
        m_AtlasSize.width = 1024;
        m_AtlasSize.height = 1024;
        std::vector<unsigned char> bitmap(m_AtlasSize.width * m_AtlasSize.height);
        stbtt_pack_context pack_context;

        if (!stbtt_PackBegin(&pack_context, bitmap.data(), m_AtlasSize.width, m_AtlasSize.height, 0, 1, nullptr))
        {
            std::cerr << "Erro ao inicializar o stb_truetype pack context." << std::endl;
            return;
        }

        stbtt_PackSetOversampling(&pack_context, 2, 2);

        // Usa a variável 'fontSize' que definimos acima para consistência.
        std::vector<stbtt_packedchar> packedCharData(charCount);
        stbtt_PackFontRange(&pack_context, ttf_buffer.data(), 0, fontSize, 32, charCount, packedCharData.data());
        float minYOffset = 0.0f;

        // Mapeia os dados dos caracteres para fácil acesso
        for (int i = 0; i < charCount; ++i)
        {
            m_CharData[32 + i] = packedCharData[i];
        }

        float scale = stbtt_ScaleForPixelHeight(&m_FontInfo, fontSize);
        int ascent, descent, lineGap;
        stbtt_GetFontVMetrics(&m_FontInfo, &ascent, &descent, &lineGap);

        m_TopToBaseline = ascent * scale;
        m_LineHeight = (ascent - descent + lineGap) * scale;

        stbtt_PackEnd(&pack_context); // Boa prática chamar PackEnd.

        // --- PARTE 4: CRIAR A TEXTURA OPENGL ---
        glDeleteTextures(1, &m_FontTextureID); // deleta se já houver outra textura carregada
        glGenTextures(1, &m_FontTextureID);
        glBindTexture(GL_TEXTURE_2D, m_FontTextureID);
        
        
#if defined(FUSION_PLATFORM_WEB)
    // Na Web (WebGL 2), para uma textura de 1 canal, usamos GL_R8 como formato interno.
#define CHANNEL  GL_R8
#else
    // No Desktop, GL_RED funciona perfeitamente.
#define CHANNEL  GL_RED

#endif

        glTexImage2D(GL_TEXTURE_2D, 0, CHANNEL, m_AtlasSize.width, m_AtlasSize.height, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.data());

        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    unsigned int Font::GetId() const
    {
        return m_FontTextureID;
    }

    float Font::GetTopToBaseline() const
    {
        return m_TopToBaseline;
    }

    float Font::GetLineHeight() const
    {
        return m_LineHeight;
    }

    void Font::Unload()
    {
        if (m_FontTextureID != 0)
        {
            glDeleteTextures(1, &m_FontTextureID);
            m_FontTextureID = 0; // Evita dupla liberação
        }
    }

} // namespace Fusion
