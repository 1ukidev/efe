#include "efe/controllers/AuthController.hpp"
#include "efe/JSON.hpp"
#include "efe/JWT.hpp"

#include <bcrypt/bcrypt.hpp>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpTypes.h>
#include <drogon/utils/coroutine.h>
#include <string>

namespace efe::controllers
{
    Task<> AuthController::verifyToken(const HttpRequestPtr req, Callback callback)
    {
        auto check = JSON::checkRequest(req);
        if (!check.valid) {
            callback(check.errorResp);
            co_return;
        }

        auto resp = HttpResponse::newHttpResponse();
        resp->setContentTypeCode(CT_APPLICATION_JSON);

        auto json = req->getJsonObject();
        auto token = json->get("token", "").asString();

        if (token.empty()) {
            resp->setStatusCode(k400BadRequest);
            resp->setBody(JSON::createResponse("Token é obrigatório", jt::error));
            callback(resp);
            co_return;
        }

        std::string usuarioId = JWT::verify(token);
        bool valid = !usuarioId.empty();

        JSON jsonResp;
        jsonResp["valid"] = valid;

        resp->setStatusCode(valid ? k200OK : k401Unauthorized);
        resp->setBody(jsonResp.toString());
        callback(resp);
        co_return;
    }

    Task<> AuthController::loginByUsuario(const HttpRequestPtr req, Callback callback)
    {
        auto check = JSON::checkRequest(req);
        if (!check.valid) {
            callback(check.errorResp);
            co_return;
        }

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
            callback(resp);
            co_return;
        }

        auto usuario = co_await usuarioDAO->findByLogin(login);
        bool ok = usuario.has_value() && bcrypt::validatePassword(senha, usuario->senha);

        if (!ok) {
            resp->setStatusCode(k401Unauthorized);
            resp->setBody(JSON::createResponse("Login ou senha inválidos", jt::error));
            callback(resp);
            co_return;
        }

        std::string jwt = JWT::generate(usuario->id);

        JSON jsonResp;
        jsonResp["token"] = jwt;

        resp->setStatusCode(k200OK);
        resp->setBody(jsonResp.toString());
        callback(resp);
        co_return;
    }
}
