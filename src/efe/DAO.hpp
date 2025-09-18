#pragma once

#include <cstdint>
#include <drogon/HttpAppFramework.h>
#include <drogon/orm/DbClient.h>
#include <drogon/orm/Exception.h>
#include <drogon/utils/coroutine.h>
#include <optional>
#include <string>
#include <trantor/utils/Logger.h>
#include <type_traits>
#include <unordered_map>
#include <utility>
#include <vector>

#include "efe/Entity.hpp"

namespace efe
{
    template <class T>
    concept IsEntity = std::is_base_of_v<Entity, T>;

    template <IsEntity T>
    class DAO
    {
    public:
        virtual ~DAO() = default;

        /**
         * @brief Salva a entidade no banco de dados.
         * 
         * @param entity
         * @return bool
         */
        virtual drogon::Task<bool> saveCoro(T& entity)
        {
            const auto [sql, values] = buildInsertQuery(entity);

            try {
                auto result = co_await getDb()->execSqlCoro(sql, values);

                entity.id = result[0]["id"].template as<std::int64_t>();

                co_return true;
            } catch (const drogon::orm::DrogonDbException& e) {
                LOG_ERROR << e.base().what();
                co_return false;
            }
        }

        /**
         * @brief Atualiza a entidade no banco de dados.
         * 
         * @param entity
         * @return bool
         */
        virtual drogon::Task<bool> updateCoro(T& entity)
        {
            const auto [sql, values] = buildUpdateQuery(entity);

            try {
                auto result = co_await getDb()->execSqlCoro(sql, values);

                if (result.affectedRows() == 0)
                    co_return false;

                co_return true;
            } catch (const drogon::orm::DrogonDbException& e) {
                LOG_ERROR << e.base().what();
                co_return false;
            }
        }

        /**
         * @brief Busca uma entidade pelo id.
         * 
         * @param id
         * @return std::optional<T>
         */
        virtual drogon::Task<std::optional<T>> findByIdCoro(std::int64_t id)
        {
            T entity{};
            std::string sql = "SELECT * FROM " + entity.getTable() + " WHERE id = $1;";

            try {
                auto result = co_await getDb()->execSqlCoro(sql, id);

                if (result.size() == 0)
                    co_return std::nullopt;

                entity.fromRowSet(result[0]);
                co_return entity;
            } catch (const drogon::orm::DrogonDbException& e) {
                LOG_ERROR << e.base().what();
                co_return std::nullopt;
            }
        }

        // TODO: Implementar mais métodos

    protected:
        inline drogon::orm::DbClientPtr getDb()
        {
            return drogon::app().getFastDbClient();
        }

    private:
        std::pair<std::string, std::vector<std::string>> buildInsertQuery(const T& entity)
        {
            std::unordered_map<std::string, std::string> columns = entity.getColumns();

            std::string sql = "INSERT INTO " + entity.getTable() + " (";

            bool first = true;
            for (const auto& [column, _] : columns) {
                if (!first) sql += ", ";
                sql += column;
                first = false;
            }

            sql += ") VALUES (";
            first = true;
            for (size_t i = 0; i < columns.size(); ++i) {
                if (!first) sql += ", ";
                sql += '$' + std::to_string(i + 1);
                first = false;
            }

            std::vector<std::string> values{};
            for (const auto& [_, value] : columns)
                values.push_back(value);

            sql += ") RETURNING id;";
            return {sql, values};
        }

        std::pair<std::string, std::vector<std::string>> buildUpdateQuery(const T& entity)
        {
            std::unordered_map<std::string, std::string> columns = entity.getColumns();

            std::string sql = "UPDATE " + entity.getTable() + " SET ";

            std::vector<std::string> values{};
            size_t index = 1;
            for (const auto& [column, value] : columns) {
                if (index > 1) sql += ", ";
                sql += column + " = $" + std::to_string(index++);
                values.push_back(value);
            }

            sql += " WHERE id = $" + std::to_string(index);
            values.push_back(std::to_string(entity.id));

            return {sql, values};
        }
    };
}
