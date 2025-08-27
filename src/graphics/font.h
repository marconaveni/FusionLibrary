
#ifndef FONT_H
#define FONT_H

#include <map>
#include <vector>

#include "fusion_math.h"
#include "stb_truetype.h" // Inclua aqui

namespace Fusion
{

    class Font
    {
    public:

        Font();
        ~Font();
        Font(const char* path, const float fontSize = 16, int charCount = 95);
        Font(const Font&) = delete;
        Font& operator=(const Font&) = delete;
        bool LoadFromFile(const char* path, float fontSize, int charCount = 95);
        bool LoadFromMemory(unsigned char* data, size_t sizeData, float fontSize, int charCount = 95);
        unsigned int GetId() const;
        void SetSmooth(bool enable);
        const std::map<int, stbtt_packedchar>& GetCharData() const { return m_CharData; }
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
        bool Load(std::vector<unsigned char>& ttf_buffer, float fontSize, int charCount = 95);
    };

} // namespace Fusion

#endif