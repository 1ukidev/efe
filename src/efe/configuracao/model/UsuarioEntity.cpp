#include <cstdint>
#include <drogon/orm/Field.h>
#include <drogon/orm/Row.h>
#include <string>
#include <unordered_map>

#include "efe/configuracao/model/UsuarioEntity.hpp"
#include "efe/JSON.hpp"

namespace efe::configuracao
{
    const std::unordered_map<std::string, std::string> UsuarioEntity::getColumns() const
    {
        return {
            {"nome", nome},
            {"login", login},
            {"senha", senha}
        };
    }

    void UsuarioEntity::fromRowSet(const drogon::orm::Row& row)
    {
        if (row.size() > 0) {
            if (!row["id"].isNull())
                id = row["id"].as<std::int64_t>();

            if (!row["nome"].isNull())
                nome = row["nome"].as<std::string>();

            if (!row["login"].isNull())
                login = row["login"].as<std::string>();

            if (!row["senha"].isNull())
                senha = row["senha"].as<std::string>();
        }
    }

    std::string UsuarioEntity::toString() const
    {
        return "UsuarioEntity[id=" + std::to_string(id) +
               ", nome=" + nome +
               ", login=" + login + ']';
    }

    JSON UsuarioEntity::toJSON() const
    {
        JSON json{};
        json["id"] = id;
        json["nome"] = nome;
        json["login"] = login;
        return json;
    }
}
