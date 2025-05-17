#include "efe/controllers/AuthController.hpp"
#include "bcrypt/bcrypt.hpp"
#include "efe/JSON.hpp"
#include "efe/JWT.hpp"

#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpTypes.h>
#include <drogon/utils/coroutine.h>
#include <string>

namespace efe::controllers
{
    Task<HttpResponsePtr> AuthController::verifyToken(const HttpRequestPtr req)
    {
        auto check = JSON::checkRequest(req);
        if (!check.valid) co_return check.errorResp;

        auto resp = HttpResponse::newHttpResponse();
        resp->setContentTypeCode(CT_APPLICATION_JSON);

        auto json = req->getJsonObject();
        auto token = json->get("token", "").asString();

        if (token.empty()) {
            resp->setStatusCode(k400BadRequest);
            resp->setBody(JSON::createResponse("Token é obrigatório", jt::error));
            co_return resp;
        }

        std::string usuarioId = JWT::verify(token);
        bool valid = !usuarioId.empty();

        JSON jsonResp;
        jsonResp.value["valid"] = valid;

        resp->setStatusCode(valid ? k200OK : k401Unauthorized);
        resp->setBody(jsonResp.toString());
        co_return resp;
    }

    Task<HttpResponsePtr> AuthController::loginByUsuario(const HttpRequestPtr req)
    {
        auto check = JSON::checkRequest(req);
        if (!check.valid) co_return check.errorResp;

        auto* usuarioDAO = app().getPlugin<UsuarioDAO>();

        auto resp = HttpResponse::newHttpResponse();
        resp->setContentTypeCode(CT_APPLICATION_JSON);

        auto json = req->getJsonObject();
        auto login = json->get("login", "").asString();
        auto senha = json->get("senha", "").asString();

        std::string faltando;
        if (login.empty()) faltando += "login, ";
        if (senha.empty()) faltando += "senha, ";

        if (!faltando.empty()) {
            faltando = faltando.substr(0, faltando.size() - 2);
            std::string msg = "Campo(s) obrigatórios ausentes: " + faltando;
            resp->setStatusCode(k400BadRequest);
            resp->setBody(JSON::createResponse(msg, jt::error));
            co_return resp;
        }

        auto usuario = co_await usuarioDAO->findByLogin(login);
        bool ok = usuario.has_value() && bcrypt::validatePassword(senha, usuario->senha);

        if (!ok) {
            resp->setStatusCode(k401Unauthorized);
            resp->setBody(JSON::createResponse("Login ou senha inválidos", jt::error));
            co_return resp;
        }

        std::string jwt = JWT::generate(usuario->id);

        JSON jsonResp;
        jsonResp.value["token"] = jwt;

        resp->setStatusCode(k200OK);
        resp->setBody(jsonResp.toString());
        co_return resp;
    }
}
