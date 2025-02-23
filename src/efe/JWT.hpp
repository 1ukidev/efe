#pragma once

#include <string>

namespace efe
{
    class JWT
    {
    public:
        JWT() = delete;

        std::string generateToken(const std::string& userId);
    };
}
