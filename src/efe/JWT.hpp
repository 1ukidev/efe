#pragma once

#include <string>
#include <utility>

namespace efe
{
    class JWT
    {
    public:
        JWT() = delete;

        /**
         * @brief Gera um token JWT para o usuário.
         * 
         * @param userId
         * @return std::string
         */
        static std::string generateToken(const std::string& userId);

        /**
         * @brief Verifica se o token JWT é válido.
         * 
         * @param token
         * @return std::pair<bool, std::string>
         */
        static std::pair<bool, std::string> verify(const std::string& token);
   };
}
