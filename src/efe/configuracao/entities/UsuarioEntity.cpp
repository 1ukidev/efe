#include "efe/configuracao/entities/UsuarioEntity.hpp"
#include "efe/JSON.hpp"

#include <cstdint>
#include <drogon/orm/Field.h>
#include <drogon/orm/Result.h>
#include <string>
#include <unordered_map>

namespace efe::configuracao
{
    const std::unordered_map<std::string, std::string> UsuarioEntity::getColumns() const
    {
        return {
            {"nome", nome}, {"senha", senha}
        };
    }

    void UsuarioEntity::fromResultSet(const drogon::orm::Result& result)
    {
        if (!result.empty()) {
            id = result[0]["id"].as<std::uint64_t>();
            nome = result[0]["nome"].as<std::string>();
            senha = result[0]["senha"].as<std::string>();
        }
    }

    std::string UsuarioEntity::toString() const
    {
        return "UsuarioEntity[id=" + std::to_string(id) +
               ", nome=" + nome +
               ", senha=" + senha + ']';
    }

    JSON UsuarioEntity::toJSON() const
    {
        JSON json;
        json.value["id"] = id;
        json.value["nome"] = nome;
        json.value["senha"] = senha;
        return json;
    }
}
