#include "efe/configuracao/entities/UsuarioEntity.hpp"
#include "efe/JSON.hpp"

#include <cstdint>
#include <drogon/orm/Field.h>
#include <drogon/orm/Result.h>
#include <string>
#include <unordered_map>

namespace efe::configuracao
{
    const std::unordered_map<std::string, std::string>& UsuarioEntity::getColumns() const
    {
        columnsCache = {
            {"nome", nome},
            {"login", login},
            {"senha", senha}
        };
        return columnsCache;
    }

    void UsuarioEntity::fromResultSet(const drogon::orm::Result& result)
    {
        if (!result.empty()) {
            if (!result[0]["id"].isNull()) {
                id = result[0]["id"].as<std::int64_t>();
            }
            if (!result[0]["nome"].isNull()) {
                nome = result[0]["nome"].as<std::string>();
            }
            if (!result[0]["login"].isNull()) {
                login = result[0]["login"].as<std::string>();
            }
            if (!result[0]["senha"].isNull()) {
                senha = result[0]["senha"].as<std::string>();
            }
        }
    }

    std::string UsuarioEntity::toString() const
    {
        return "UsuarioEntity[id=" + std::to_string(id) +
               ", nome=" + nome +
               ", login=" + login + "]";
    }

    JSON UsuarioEntity::toJSON() const
    {
        JSON json;
        json.value["id"] = id;
        json.value["nome"] = nome;
        json.value["login"] = login;
        return json;
    }
}
