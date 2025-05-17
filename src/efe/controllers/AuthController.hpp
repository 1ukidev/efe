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
        METHOD_LIST_BEGIN
            METHOD_ADD(AuthController::verifyToken, "/verifyToken", Post);
            METHOD_ADD(AuthController::loginByUsuario, "/loginByUsuario", Post);
        METHOD_LIST_END

        Task<HttpResponsePtr> verifyToken(const HttpRequestPtr req);
        Task<HttpResponsePtr> loginByUsuario(const HttpRequestPtr req);
    };
}
