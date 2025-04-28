#include "JSON.hpp"

#include <drogon/HttpResponse.h>
#include <json/value.h>
#include <json/writer.h>
#include <optional>
#include <string>

namespace efe
{
    using namespace drogon;

    std::string JSON::toString() const
    {
        Json::FastWriter writer;
        return writer.write(value);
    }

    std::optional<std::string> JSON::getId() const
    {
        if (value.isMember("id")) {
            return "{\"id\":\"" + value["id"].asString() + "\"}\n";
        }
        return std::nullopt;
    }

    std::string JSON::createResponse(const std::string& msg, const jt type)
    {
        return type == jt::error ? 
            "{\"error\":\"" + msg + "\"}\n" : 
            "{\"message\":\"" + msg + "\"}\n";
    }

    HttpResponsePtr JSON::invalidRequest()
    {
        auto resp = HttpResponse::newHttpResponse(k400BadRequest, CT_APPLICATION_JSON);
        resp->setBody(createResponse("Corpo da requisição inválido", jt::error));
        return resp;
    }
}
