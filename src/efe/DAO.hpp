#pragma once

#include "efe/Entity.hpp"

#include <cstdint>
#include <drogon/HttpAppFramework.h>
#include <drogon/orm/DbClient.h>
#include <drogon/orm/Exception.h>
#include <drogon/utils/coroutine.h>
#include <optional>
#include <string>
#include <trantor/utils/Logger.h>
#include <type_traits>
#include <utility>
#include <vector>

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
            LOG_DEBUG << '(' << entity.getClassName() << ") Salvando entidade...";

            try {
                auto [sql, values] = buildInsertQuery(entity);
                auto result = co_await getDb()->execSqlCoro(sql, values);

                entity.id = result[0]["id"].template as<std::int64_t>();

                LOG_DEBUG << '(' << entity.getClassName() << ") Nova entidade salva com id "
                          << entity.id;
                co_return true;
            } catch (const drogon::orm::DrogonDbException& e) {
                LOG_ERROR << '(' << entity.getClassName() << ") Erro ao salvar entidade: "
                          << e.base().what();
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
            LOG_DEBUG << '(' << entity.getClassName() << ") Atualizando entidade...";

            try {
                auto [sql, values] = buildUpdateQuery(entity);
                auto result = co_await getDb()->execSqlCoro(sql, values);

                if (result.affectedRows() == 0) {
                    LOG_WARN << '(' << entity.getClassName() << ") Nenhum registro atualizado";
                    co_return false;
                }

                LOG_DEBUG << '(' << entity.getClassName() << ") Entidade atualizada com sucesso.";
                co_return true;
            } catch (const drogon::orm::DrogonDbException& e) {
                LOG_ERROR << '(' << entity.getClassName() << ") Erro ao atualizar entidade: "
                          << e.base().what();
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
            T entity;
            LOG_DEBUG << '(' << entity.getClassName() << ") Buscando entidade com id " << id << "...";

            try {
                std::string sql = "SELECT * FROM " + entity.getTable() + " WHERE id = $1;";
                auto result = co_await getDb()->execSqlCoro(sql, id);

                if (result.size() == 0) {
                    LOG_WARN << '(' << entity.getClassName() << ") Nenhum registro encontrado para o id "
                             << id;
                    co_return std::nullopt;
                }

                entity.fromResultSet(result);
                co_return entity;
            } catch (const drogon::orm::DrogonDbException& e) {
                LOG_ERROR << '(' << entity.getClassName() << ") Erro ao buscar entidade com id " << id
                          << ": " << e.base().what();
                co_return std::nullopt;
            }
        }

        /**
         * @brief Busca uma entidade pelo where.
         * 
         * @param where
         * @return std::optional<T>
         */
        virtual drogon::Task<std::optional<T>> findOneCoro(const std::string& where)
        {
            T entity;
            LOG_DEBUG << '(' << entity.getClassName() << ") Buscando entidade com '" << where
                      << "'...";

            try {
                std::string sql = "SELECT * FROM " + entity.getTable() + " WHERE " + where + ';';
                auto result = co_await getDb()->execSqlCoro(sql);

                if (result.size() == 0) {
                    LOG_WARN << '(' << entity.getClassName() << ") Nenhum registro encontrado para '"
                             << where << "'";
                    co_return std::nullopt;
                } else if (result.size() > 1) {
                    LOG_WARN << '(' << entity.getClassName() << ") Mais de um registro encontrado para '"
                             << where << "'";
                    co_return std::nullopt;
                }

                entity.fromResultSet(result);
                co_return entity;
            } catch (const drogon::orm::DrogonDbException& e) {
                LOG_ERROR << '(' << entity.getClassName() << ") Erro ao buscar entidade com '"
                          << where << "': " << e.base().what();
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
        std::pair<std::string, const std::vector<std::string>> buildInsertQuery(const T& entity)
        {
            const auto& columns = entity.getColumns();

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

            std::vector<std::string> values;
            for (const auto& [_, value] : columns) {
                values.push_back(value);
            }

            sql += ") RETURNING id;";
            return {sql, values};
        }

        std::pair<std::string, const std::vector<std::string>> buildUpdateQuery(const T& entity)
        {
            const auto& columns = entity.getColumns();

            std::string sql = "UPDATE " + entity.getTable() + " SET ";

            std::vector<std::string> values;
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
