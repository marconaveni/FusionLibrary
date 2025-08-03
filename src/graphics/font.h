
#ifndef FONT_H
#define FONT_H

#include <map>
#include <vector>
#include "stb_truetype.h" // Inclua aqui
#include "fusion_math.h"

namespace Fusion
{

    class Font
    {
    public:
        Font(const Font &) = delete;
        Font &operator=(const Font &) = delete;
        Font(const char* path, const float fontSize = 16, int charCount = 95);
        ~Font();
        void LoadFromFile(const char *path, float fontSize, int charCount = 95);
        unsigned int GetId() const;
        const std::map<int, stbtt_packedchar> &GetCharData() const { return m_CharData; }
        float GetTopToBaseline() const;
        Sizei GetAtlasSize() const { return m_AtlasSize; }
    


    private:
        std::map<int, stbtt_packedchar> m_CharData; // Mapeia codepoint para dados do glifo
        unsigned int m_FontTextureID = 0;
        stbtt_fontinfo m_FontInfo;
        float m_TopToBaseline = 0.0f;
        Sizei m_AtlasSize = {0, 0}; // Tamanho do atlas de textura
    };

}

#endif