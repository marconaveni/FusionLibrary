#include "fusion_utf8.h"

// O único lugar no seu projeto que inclui a biblioteca de terceiros
#include "utf8.h" 

namespace Fusion
{
    namespace Utf8
    {
        int32_t DecodeNext(const std::string& str, size_t& iterator)
        {
            const char* c_str = str.c_str();
            const utf8_int8_t* p = reinterpret_cast<const utf8_int8_t*>(c_str + iterator);

            if (*p == 0) 
            { 
                return 0;
            }

            int32_t codepoint = 0;
            const utf8_int8_t* next_p = utf8codepoint(p, &codepoint);
            
            // Atualiza o iterador com o número de bytes lidos
            iterator += (next_p - p);

            return codepoint;
        }
    }
}