#pragma once

#include "efe/DAO.hpp"
#include "efe/configuracao/entities/UsuarioEntity.hpp"

#include <drogon/plugins/Plugin.h>
#include <drogon/utils/coroutine.h>
#include <optional>
#include <string>

namespace efe::configuracao
{
    class UsuarioDAO final : public DAO<UsuarioEntity>, public drogon::Plugin<UsuarioDAO>
    {
    public:
        UsuarioDAO() = default;
        ~UsuarioDAO() = default;

        virtual void initAndStart(const Json::Value&) override {};
        virtual void shutdown() override {};

        /**
         * @brief Busca um usuário pelo login.
         * 
         * @param login
         * @return std::optional<UsuarioEntity>
         */
        drogon::Task<std::optional<UsuarioEntity>> findByLogin(const std::string& login);
    };
}
