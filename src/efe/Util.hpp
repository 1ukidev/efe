#pragma once

#include <drogon/HttpTypes.h>
#include <drogon/orm/DbClient.h>
#include <string_view>

namespace efe
{
    class Util final
    {
    public:
        Util() = delete;

        /**
         * @brief Verifica se a string é um número.
         * 
         * @param str
         * @return bool
         */
        static bool isNumber(std::string_view str);
    };
}
