#pragma once

#include "efe/Entity.hpp"
#include "efe/Util.hpp"

#include <cstdint>
#include <drogon/orm/DbClient.h>
#include <drogon/orm/Exception.h>
#include <drogon/utils/coroutine.h>
#include <optional>
#include <string>
#include <trantor/utils/Logger.h>
#include <type_traits>
#include <unordered_map>

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
                std::string sql = buildInsertQuery(entity);
                auto result = co_await getDb()->execSqlCoro(sql);

                entity.id = result[0]["id"].template as<std::uint64_t>();

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
                std::string sql = buildUpdateQuery(entity);
                auto result = co_await getDb()->execSqlCoro(sql);

                if (result.affectedRows() == 0) {
                    LOG_WARN << '(' << entity.getClassName() << ") Nenhum registro atualizado.";
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
        virtual drogon::Task<std::optional<T>> findByIdCoro(std::uint64_t id)
        {
            T entity;
            LOG_DEBUG << '(' << entity.getClassName() << ") Buscando entidade com id " << id << "...";

            try {
                std::string sql = "SELECT * FROM " + T().getTable() + " WHERE id = $1;";
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
        virtual drogon::Task<std::optional<T>> findOne(const std::string& where)
        {
            T entity;
            LOG_DEBUG << '(' << entity.getClassName() << ") Buscando entidade com " << where
                      << "...";

            try {
                std::string sql = "SELECT * FROM " + T().getTable() + " WHERE " + where + ';';
                auto result = co_await getDb()->execSqlCoro(sql);

                if (result.size() == 0) {
                    LOG_WARN << '(' << entity.getClassName() << ") Nenhum registro encontrado para "
                             << where;
                    co_return std::nullopt;
                }

                entity.fromResultSet(result);
                co_return entity;
            } catch (const drogon::orm::DrogonDbException& e) {
                LOG_ERROR << '(' << entity.getClassName() << ") Erro ao buscar entidade com "
                          << where << ": " << e.base().what();
                co_return std::nullopt;
            }
        }

    protected:
        inline drogon::orm::DbClientPtr getDb()
        {
            return Util::getDbClient();
        }

    private:
        std::string buildInsertQuery(const T& entity)
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
            for (const auto& [_, value] : columns) {
                if (!first) sql += ", ";
                sql += Util::isNumber(value) ? value : ("'" + value + "'");
                first = false;
            }

            sql += ") returning id;";
            return sql;
        }

        std::string buildUpdateQuery(const T& entity)
        {
            std::unordered_map<std::string, std::string> columns = entity.getColumns();

            std::string sql = "UPDATE " + entity.getTable() + " SET ";

            bool first = true;
            for (const auto& [column, value] : columns) {
                if (column == "id") continue;
                if (!first) sql += ", ";
                sql += column + " = " + (Util::isNumber(value) ? value : ("'" + value + "'"));
                first = false;
            }

            sql += " WHERE id = " + std::to_string(entity.id) + ';';
            return sql;
        }
    };
}
