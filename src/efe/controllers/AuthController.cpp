#include "efe/controllers/AuthController.hpp"
#include "efe/JSON.hpp"
#include "efe/JWT.hpp"

#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpTypes.h>
#include <drogon/utils/coroutine.h>
#include <string>

namespace efe::controllers
{
    Task<HttpResponsePtr> AuthController::verify(const HttpRequestPtr req)
    {
        auto json = req->getJsonObject();
        if (!json) co_return JSON::invalidRequest();

        auto resp = HttpResponse::newHttpResponse();
        resp->setContentTypeCode(CT_APPLICATION_JSON);

        std::string token = json->get("token", "").asString();
        if (token.empty()) {
            resp->setStatusCode(k400BadRequest);
            resp->setBody(JSON::createResponse("Token é obrigatório", true));
            co_return resp;
        }

        auto [valid, userId] = JWT::verify(token);

        JSON jsonResp;
        jsonResp.value["valid"] = valid;
        jsonResp.value["userId"] = userId;

        resp->setStatusCode(k200OK);
        resp->setBody(jsonResp.toString());
        co_return resp;
    }
}
