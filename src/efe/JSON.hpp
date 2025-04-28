#pragma once

#include <drogon/HttpResponse.h>
#include <json/value.h>
#include <json/writer.h>
#include <optional>
#include <string>

namespace efe
{
    enum class jt
    {
        message = 0,
        error = 1
    };

    class JSON final
    {
    public:
        JSON() = default;
        ~JSON() = default;

        Json::Value value{Json::objectValue};

        /**
         * @brief Converte o JSON para string.
         * 
         * @return std::string
         */
        std::string toString() const;

        /**
         * @brief Retorna o valor do campo "id" do JSON.
         * 
         * @return std::optional<std::string>
         */
        std::optional<std::string> getId() const;

        /**
         * @brief Cria uma resposta JSON.
         * 
         * @param msg
         * @param type
         * @return std::string
         */
        static std::string createResponse(const std::string& msg,
                                          const jt type = jt::message);

        /**
         * @brief Cria uma resposta JSON de requisição inválida.
         * 
         * @return drogon::HttpResponsePtr
         */                                 
        static drogon::HttpResponsePtr invalidRequest();
    };
}
