#include "efe/configuracao/dao/UsuarioDAO.hpp"

#include <drogon/utils/coroutine.h>
#include <optional>
#include <string>

namespace efe::configuracao
{
    using namespace drogon;

    Task<std::optional<UsuarioEntity>> UsuarioDAO::findByLogin(const std::string& login)
    {
        std::string where = "login = '" + login + "'";
        auto result = co_await findOneCoro(where);

        if (result.has_value()) {
            co_return result.value();
        } else {
            co_return std::nullopt;
        }
    }
}
