#include "JSON.hpp"
#include "efe/JWT.hpp"

#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <json/value.h>
#include <json/writer.h>
#include <string>

namespace efe
{
    using namespace drogon;

    std::string JSON::toString() const
    {
        Json::FastWriter writer;
        return writer.write(value);
    }

    std::string JSON::createResponse(const std::string& msg, const jt type)
    {
        switch (type) {
            case jt::message:
                return "{\"success\":true,{\"message\":\"" + msg + "\"}\n";
            case jt::success:
                return "{\"success\":true,\"message\":\"" + msg + "\"}\n";
            case jt::error:
                return "{\"success\":false,\"message\":\"" + msg + "\"}\n";
            default:
                return "{\"success\":false,\"message\":\"Tipo de resposta inválido\"}\n";
        }
    }

    RequestResult JSON::checkRequest(const HttpRequestPtr& req)
    {
        auto json = req->getJsonObject();
        if (!json) {
            auto resp = HttpResponse::newHttpResponse(k400BadRequest, CT_APPLICATION_JSON);
            resp->setBody(createResponse("Corpo da requisição inválido", jt::error));
            return {
                .valid = false,
                .errorResp = resp
            };
        }

        return {
            .valid = true,
            .errorResp = nullptr
        };
    }

    AuthorizationResult JSON::checkAuthorization(const HttpRequestPtr& req)
    {
        auto token = req->getHeader("Authorization");
        if (token.empty()) {
            return {
                .valid = false,
                .errorResp = [] {
                    auto r = HttpResponse::newHttpResponse(k401Unauthorized, CT_APPLICATION_JSON);
                    r->setBody(createResponse("Token de autorização não encontrado", jt::error));
                    return r;
                }(),
                .usuarioId = ""
            };
        }

        std::string usuarioId = JWT::verify(token);
        bool valid = !usuarioId.empty();
        if (!valid) {
            return {
                .valid = false,
                .errorResp = [] {
                    auto r = HttpResponse::newHttpResponse(k401Unauthorized, CT_APPLICATION_JSON);
                    r->setBody(createResponse("Token de autorização inválido", jt::error));
                    return r;
                }(),
                .usuarioId = ""
            };
        }

        return {
            .valid = true,
            .errorResp = nullptr,
            .usuarioId = usuarioId
        };
    }
}
