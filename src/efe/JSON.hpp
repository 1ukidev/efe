#pragma once

#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <json/value.h>
#include <string>

namespace efe
{
    enum class jt
    {
        success = 0,
        error = 1
    };

    struct RequestResult final
    {
        bool valid;
        drogon::HttpResponsePtr errorResp;
    };

    struct AuthorizationResult final
    {
        bool valid;
        drogon::HttpResponsePtr errorResp;
        std::string usuarioId;
    };

    class JSON final
    {
    public:
        JSON() = default;
        ~JSON() = default;

        Json::Value& operator[](const std::string& key) {
            return value[key];
        }

        const Json::Value& operator[](const std::string& key) const {
            return value[key];
        }

        /**
         * @brief Retorna uma representação em string do JSON.
         * 
         * @return std::string
         */
        std::string toString() const;

        /**
         * @brief Cria uma resposta JSON.
         * 
         * @param msg
         * @param type
         * @return std::string
         */
        static std::string createResponse(const std::string& msg,
                                          const jt type = jt::success);

        /**
         * @brief Verifica se o corpo da requisição é um JSON válido.
         * 
         * @param req
         * @return RequestResult
         */
        static RequestResult checkRequest(const drogon::HttpRequestPtr& req);

        /**
         * @brief Verifica se o usuário está autorizado a acessar o recurso.
         * 
         * @param req
         * @return AuthorizationResult
         */
        static AuthorizationResult checkAuthorization(const drogon::HttpRequestPtr& req);

    private:
        Json::Value value{Json::objectValue};
    };
}
