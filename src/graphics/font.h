
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
        Font();
        ~Font();
        void LoadFromMemory(unsigned char *data, size_t sizeData, float fontSize, int charCount = 95);
        void LoadFromFile(const char *path, float fontSize, int charCount = 95);
        void SetSmooth(bool enable);
        unsigned int GetId() const;
        const std::map<int, stbtt_packedchar> &GetCharData() const { return m_CharData; }
        float GetTopToBaseline() const;
        float GetLineHeight() const;
        Sizei GetAtlasSize() const { return m_AtlasSize; }
        void Unload();
    


    private:
        std::map<int, stbtt_packedchar> m_CharData; // Mapeia codepoint para dados do glifo
        unsigned int m_FontTextureID = 0;
        stbtt_fontinfo m_FontInfo;
        float m_TopToBaseline = 0.0f;
        float m_LineHeight = 0.0f;
        Sizei m_AtlasSize = {0, 0}; // Tamanho do atlas de textura
        void LoadFont(std::vector<unsigned char>& ttf_buffer, float fontSize, int charCount = 95);
    };

}

#endif