#include "efe/JSON.hpp"
#include "efe/controllers/UsuarioController.hpp"
#include "efe/configuracao/dao/UsuarioDAO.hpp"
#include "efe/configuracao/model/UsuarioEntity.hpp"

#include <bcrypt/bcrypt.hpp>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpTypes.h>
#include <drogon/utils/coroutine.h>
#include <string>

namespace efe::controllers
{
    Task<> UsuarioController::saveUser(const HttpRequestPtr req, Callback callback)
    {
        auto check = JSON::checkRequest(req);
        if (!check.valid) {
            callback(check.errorResp);
            co_return;
        }

        auto* dao = app().getPlugin<UsuarioDAO>();

        // TODO: Verificar se o usuário pode salvar outro usuário

        // auto auth = JSON::checkAuthorization(req);
        // if (!auth.valid) co_return auth.errorResp;

        auto resp = HttpResponse::newHttpResponse();
        resp->setContentTypeCode(CT_APPLICATION_JSON);

        auto json = req->getJsonObject();
        auto nome = json->get("nome", "").asString();
        auto login = json->get("login", "").asString();
        auto senha = json->get("senha", "").asString();

        std::string faltando;
        if (nome.empty()) faltando += "nome, ";
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

        // TODO: Adicionar mais validações

        senha = bcrypt::generateHash(senha, 10);

        UsuarioEntity entity(nome, login, senha);
        bool ok = co_await dao->saveCoro(entity);

        resp->setStatusCode(ok ? k201Created : k500InternalServerError);
        resp->setBody(JSON::createResponse(
            ok ? "Salvo com sucesso" : "Erro interno ao salvar usuário",
            ok ? jt::success : jt::error
        ));
        callback(resp);
        co_return;
    }
}
