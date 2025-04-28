#include "efe/controllers/UsuarioController.hpp"
#include "efe/JSON.hpp"
#include "efe/configuracao/entities/UsuarioEntity.hpp"

#include <cstdint>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpTypes.h>
#include <drogon/utils/coroutine.h>
#include <optional>
#include <string>

namespace efe::controllers
{
    Task<HttpResponsePtr> UsuarioController::saveUser(const HttpRequestPtr req)
    {
        auto json = req->getJsonObject();
        if (!json) co_return JSON::invalidRequest();

        auto resp = HttpResponse::newHttpResponse();
        resp->setContentTypeCode(CT_APPLICATION_JSON);

        std::string nome = json->get("nome", "").asString();
        std::string senha = json->get("senha", "").asString();

        if (nome.empty() || senha.empty()) {
            resp->setStatusCode(k400BadRequest);
            resp->setBody(JSON::createResponse("Nome e senha são obrigatórios", jt::error));
            co_return resp;
        }

        // TODO: Aplicar hash na senha

        UsuarioEntity entity(nome, senha);
        bool ok = co_await dao.saveCoro(entity);

        resp->setStatusCode(ok ? k201Created : k500InternalServerError);
        resp->setBody(
            ok ? entity.toJSON().getId().value()
               : JSON::createResponse("Erro ao salvar usuário", jt::error)
        );
        co_return resp;
    }

    Task<HttpResponsePtr> UsuarioController::getUser(const HttpRequestPtr, std::uint64_t id)
    {
        auto resp = HttpResponse::newHttpResponse(k200OK, CT_APPLICATION_JSON);

        std::optional<UsuarioEntity> entity = co_await dao.findByIdCoro(id);

        if (!entity.has_value()) {
            resp->setStatusCode(k404NotFound);
            resp->setBody(JSON::createResponse("Usuário não encontrado", jt::error));
            co_return resp;
        }

        resp->setBody(entity->toJSON().toString());
        co_return resp;
    }
}
