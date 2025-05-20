#pragma once

#include "efe/JSON.hpp"

#include <cstdint>
#include <drogon/orm/Result.h>
#include <json/value.h>
#include <string>
#include <unordered_map>

namespace efe
{
    class Entity
    {
    public:
        virtual ~Entity() = default;

        /**
         * @brief Retorna o nome da classe.
         * 
         * @return std::string
         */
        virtual std::string getClassName() const = 0;

        /**
         * @brief Retorna o nome da tabela no banco de dados.
         * 
         * @return std::string
         */
        virtual std::string getTable() const = 0;

        /**
         * @brief Retorna um mapa com os nomes das colunas e seus respectivos valores.
         * 
         * @return const std::unordered_map<std::string, std::string>&
         */
        virtual const std::unordered_map<std::string, std::string>& getColumns() const = 0;

        /**
         * @brief Preenche a entidade com os valores do resultado de uma consulta.
         * 
         * @param result
         * @return void
         */
        virtual void fromResultSet(const drogon::orm::Result& result) = 0;

        /**
         * @brief Carrega as relações da entidade.
         *
         * @return void
         */
        virtual void loadRelations() {};

        /** 
         * @brief Retorna uma representação em string da entidade.
         * 
         * @return std::string
         */
        virtual std::string toString() const = 0;

        /**
         * @brief Retorna uma representação em JSON da entidade.
         * 
         * @return JSON
         */
        virtual JSON toJSON() const = 0;

        std::int64_t id{};
    };
}
