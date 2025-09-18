#pragma once

#include <drogon/orm/Row.h>
#include <string>
#include <unordered_map>

#include "efe/Entity.hpp"
#include "efe/JSON.hpp"

namespace efe::configuracao
{
    class UsuarioEntity final : public Entity
    {
    public:
        UsuarioEntity() = default;
        ~UsuarioEntity() = default;

        UsuarioEntity(const std::string& nome, const std::string& login, const std::string& senha)
            : nome(nome), login(login), senha(senha) {}

        std::string getClassName() const override { return "UsuarioEntity"; }
        std::string getTable() const override { return "usuario"; }

        const std::unordered_map<std::string, std::string> getColumns() const override;
        void fromRowSet(const drogon::orm::Row& result) override;

        std::string toString() const override;
        JSON toJSON() const override;

        std::string nome;
        std::string login;
        std::string senha;
    };
}
