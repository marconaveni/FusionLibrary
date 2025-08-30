#include "font.h"

#include <glad/glad.h>
#include <iostream>
#include <vector>

#define STB_TRUETYPE_IMPLEMENTATION
#include "stb_truetype.h"

namespace Fusion
{
    Font::Font(const char* path, const float fontSize, int charCount)
        : m_fontInfo()
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

        glBindTexture(GL_TEXTURE_2D, m_fontTextureID);

        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (enable) ? GL_LINEAR : GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (enable) ? GL_LINEAR : GL_NEAREST);

        glBindTexture(GL_TEXTURE_2D, 0);
    }

    bool Font::LoadFromFile(const char* path, const float fontSize, int charCount)
    {

        std::vector<unsigned char> ttf_buffer;

        FILE* fontFile = fopen(path, "rb");
        if (!fontFile)
        {
            std::cerr << "Erro ao abrir arquivo de fonte: " << path << std::endl;
            return false;
        }

        fseek(fontFile, 0, SEEK_END);
        long fileSize = ftell(fontFile);
        fseek(fontFile, 0, SEEK_SET);

        ttf_buffer.resize(fileSize);

        fread(ttf_buffer.data(), 1, fileSize, fontFile);
        fclose(fontFile);

        return Load(ttf_buffer, fontSize, charCount);
    }

    bool Font::LoadFromMemory(unsigned char* data, size_t sizeData, float fontSize, int charCount)
    {
        std::vector<unsigned char> ttf_buffer;
        for (size_t i = 0; i < sizeData; i++)
        {
            ttf_buffer.push_back(data[i]);
        }
        return Load(ttf_buffer, fontSize, charCount);
    }

    bool Font::Load(std::vector<unsigned char>& ttf_buffer, float fontSize, int charCount)
    {
        if (!stbtt_InitFont(&m_fontInfo, ttf_buffer.data(), 0))
        {
            std::cerr << "Erro ao inicializar informações da fonte stbtt." << std::endl;
            return false;
        }

        // o tamanho do atlas está sendo definido com um tamanho fixo e isso
        // precisa ser corrigido no futuro calculando o tamanho dinamicamente
        // baseado no tamanho da fonte fontSize definido pelo usuário.
        m_atlasSize.width = 1024;
        m_atlasSize.height = 1024;
        std::vector<unsigned char> bitmap(m_atlasSize.width * m_atlasSize.height);
        stbtt_pack_context pack_context;

        if (!stbtt_PackBegin(&pack_context, bitmap.data(), m_atlasSize.width, m_atlasSize.height, 0, 1, nullptr))
        {
            std::cerr << "Erro ao inicializar o stb_truetype pack context." << std::endl;
            return false;
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

        float scale = stbtt_ScaleForPixelHeight(&m_fontInfo, fontSize);
        int ascent, descent, lineGap;
        stbtt_GetFontVMetrics(&m_fontInfo, &ascent, &descent, &lineGap);

        m_topToBaseline = ascent * scale;
        m_lineHeight = (ascent - descent + lineGap) * scale;

        stbtt_PackEnd(&pack_context); // Boa prática chamar PackEnd.

        // --- PARTE 4: CRIAR A TEXTURA OPENGL ---
        glDeleteTextures(1, &m_fontTextureID); // deleta se já houver outra textura carregada
        glGenTextures(1, &m_fontTextureID);
        glBindTexture(GL_TEXTURE_2D, m_fontTextureID);


#if defined(FUSION_PLATFORM_WEB)
        // Na Web (WebGL 2), para uma textura de 1 canal, usamos GL_R8 como formato interno.
#define CHANNEL GL_R8
#else
        // No Desktop, GL_RED funciona perfeitamente.
#define CHANNEL GL_RED

#endif

        glTexImage2D(GL_TEXTURE_2D, 0, CHANNEL, m_atlasSize.width, m_atlasSize.height, 0, GL_RED, GL_UNSIGNED_BYTE,
                     bitmap.data());

        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        glBindTexture(GL_TEXTURE_2D, 0);

        return true;
    }

    unsigned int Font::GetId() const
    {
        return m_fontTextureID;
    }

    float Font::GetTopToBaseline() const
    {
        return m_topToBaseline;
    }

    float Font::GetLineHeight() const
    {
        return m_lineHeight;
    }

    void Font::Unload()
    {
        if (m_fontTextureID != 0)
        {
            glDeleteTextures(1, &m_fontTextureID);
            m_fontTextureID = 0; // Evita dupla liberação
        }
    }

} // namespace Fusion
