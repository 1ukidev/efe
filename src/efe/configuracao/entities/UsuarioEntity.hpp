#pragma once

#include "efe/Entity.hpp"
#include "efe/JSON.hpp"

#include <drogon/orm/Result.h>
#include <string>
#include <unordered_map>

namespace efe::configuracao
{
    class UsuarioEntity final : public Entity
    {
    public:
        UsuarioEntity() = default;
        ~UsuarioEntity() = default;

        UsuarioEntity(const std::string& nome, const std::string& senha)
            : nome(nome), senha(senha) {}

        std::string getClassName() const override { return "UsuarioEntity"; }
        std::string getTable() const override { return "usuario"; }

        const std::unordered_map<std::string, std::string> getColumns() const override;

        void fromResultSet(const drogon::orm::Result& result) override;

        std::string toString() const override;
        JSON toJSON() const override;

        std::string nome;
        std::string senha;
    };
}
