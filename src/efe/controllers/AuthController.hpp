#pragma once

#include <drogon/HttpController.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/utils/coroutine.h>

namespace efe::controllers
{
    using namespace drogon;

    class AuthController final : public HttpController<AuthController>
    {
    public:
        METHOD_LIST_BEGIN
            METHOD_ADD(AuthController::login, "/login", Post);
            METHOD_ADD(AuthController::verify, "/verify", Post);
        METHOD_LIST_END

        Task<HttpResponsePtr> login(const HttpRequestPtr req);
        Task<HttpResponsePtr> verify(const HttpRequestPtr req);
    };
}
