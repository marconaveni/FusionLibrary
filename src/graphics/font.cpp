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

    Font::~Font()
    {
        glDeleteTextures(1, &m_FontTextureID);
    }

    // Substitua sua função Font::LoadFromFile por esta versão completa:

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

        if (!stbtt_InitFont(&m_FontInfo, ttf_buffer.data(), 0))
        {
            std::cerr << "Erro ao inicializar informações da fonte stbtt." << std::endl;
            return;
        }

        std::vector<unsigned char> bitmap(512 * 512);
        stbtt_pack_context pack_context;

        if (!stbtt_PackBegin(&pack_context, bitmap.data(), 512, 512, 0, 1, nullptr))
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

        stbtt_PackEnd(&pack_context); // Boa prática chamar PackEnd.

        // --- PARTE 4: CRIAR A TEXTURA OPENGL ---
        glDeleteTextures(1, &m_FontTextureID); // deleta se já houver outra textura carregada
        glGenTextures(1, &m_FontTextureID);
        glBindTexture(GL_TEXTURE_2D, m_FontTextureID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.data());
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }

    // void Font::LoadFromFile(const char *path)
    // {
    //     std::vector<unsigned char> bitmap(512 * 512);
    //     std::vector<unsigned char> ttf_buffer; // Será redimensionado após ler o arquivo
    //     stbtt_fontinfo m_FontInfo;

    //     FILE *fontFile = fopen(path, "rb");
    //     if (!fontFile)
    //     {
    //         std::cerr << "Erro ao abrir arquivo de fonte: " << path << std::endl;
    //         return;
    //     }

    //     // Uma forma mais segura de ler o arquivo
    //     fseek(fontFile, 0, SEEK_END);
    //     long fileSize = ftell(fontFile);
    //     fseek(fontFile, 0, SEEK_SET);

    //     ttf_buffer.resize(fileSize);

    //     fread(ttf_buffer.data(), 1, fileSize, fontFile);
    //     fclose(fontFile);

    //     stbtt_pack_context pack_context;
    //     if (!stbtt_PackBegin(&pack_context, bitmap.data(), 512, 512, 0, 1, nullptr))
    //     {
    //         std::cerr << "Erro ao inicializar o stb_truetype pack context." << std::endl;
    //         return;
    //     }

    //     stbtt_PackSetOversampling(&pack_context, 2, 2); // Melhora a qualidade da rasterização

    //     // 1. Carrega a faixa ASCII básica (caracteres imprimíveis)
    //     stbtt_packedchar ascii_chars[254]; // 95
    //     stbtt_PackFontRange(&pack_context, ttf_buffer.data(), 0, 32.0f, 32, 254, ascii_chars);

    //     // Mapeia os dados dos caracteres para fácil acesso
    //     for (int i = 0; i < 254; ++i)
    //     {
    //         m_CharData[32 + i] = ascii_chars[i];
    //     }

    //     glGenTextures(1, &m_FontTextureID);
    //     glBindTexture(GL_TEXTURE_2D, m_FontTextureID);
    //     glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, 512, 512, 0, GL_RED, GL_UNSIGNED_BYTE, bitmap.data());
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    //     glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // }

    unsigned int Font::GetId() const
    {
        return m_FontTextureID;
    }

    float Font::GetTopToBaseline() const
    {
        return m_TopToBaseline;
    }

} // namespace Fusion
