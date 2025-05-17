#pragma once

#include "efe/configuracao/dao/UsuarioDAO.hpp"

#include <drogon/HttpController.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpTypes.h>
#include <drogon/utils/coroutine.h>

namespace efe::controllers
{
    using namespace drogon;
    using namespace efe::configuracao;

    class UsuarioController final : public HttpController<UsuarioController>
    {
    public:
        METHOD_LIST_BEGIN
            METHOD_ADD(UsuarioController::saveUser, "", Post);
        METHOD_LIST_END

        Task<HttpResponsePtr> saveUser(const HttpRequestPtr req);
    };
}
