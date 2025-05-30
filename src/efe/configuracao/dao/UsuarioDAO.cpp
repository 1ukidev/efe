#include "efe/configuracao/dao/UsuarioDAO.hpp"

#include <drogon/utils/coroutine.h>
#include <optional>
#include <string>

namespace efe::configuracao
{
    using namespace drogon;

    Task<std::optional<UsuarioEntity>> UsuarioDAO::findByLogin(const std::string& login)
    {
        try {
            std::string sql = "SELECT * FROM usuario WHERE login = $1;";
            auto result = co_await getDb()->execSqlCoro(sql, login);

            if (result.size() == 0)
                co_return std::nullopt;

            UsuarioEntity usuario;
            usuario.fromResultSet(result);
            co_return usuario;
        } catch (const drogon::orm::DrogonDbException& e) {
            LOG_ERROR << e.base().what();
            co_return std::nullopt;
        }
    }
}
