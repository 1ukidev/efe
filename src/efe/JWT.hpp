#pragma once

#include <cstdint>
#include <string>
#include <utility>

namespace efe
{
    class JWT final
    {
    public:
        JWT() = delete;

        /**
         * @brief Gera um token JWT para o usuário.
         * 
         * @param usuarioId
         * @return std::string
         */
        static std::string generate(std::uint64_t usuarioId);

        /**
         * @brief Verifica se o token JWT é válido.
         * 
         * @param token
         * @return std::string
         */
        static std::string verify(const std::string& token);
   };
}
