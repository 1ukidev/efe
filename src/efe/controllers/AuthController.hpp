#pragma once

#include "efe/configuracao/dao/UsuarioDAO.hpp"

#include <drogon/HttpController.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/utils/coroutine.h>

namespace efe::controllers
{
    using namespace drogon;
    using namespace efe::configuracao;

    class AuthController final : public HttpController<AuthController>
    {
    public:
        using Callback = std::function<void(const HttpResponsePtr&)>;

        METHOD_LIST_BEGIN
            METHOD_ADD(AuthController::verifyToken, "/verifyToken", Post);
            METHOD_ADD(AuthController::loginByUsuario, "/loginByUsuario", Post);
        METHOD_LIST_END

        Task<> verifyToken(const HttpRequestPtr req, Callback callback);
        Task<> loginByUsuario(const HttpRequestPtr req, Callback callback);
    };
}
