#ifndef FUSION_UTF8_H
#define FUSION_UTF8_H

#include <string>
#include <cstdint>

namespace Fusion
{
    namespace Utf8
    {

        int32_t DecodeNext(const std::string& str, size_t& iterator);
        
    }
}

#endif