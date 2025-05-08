#pragma once

#include "efe/Singleton.hpp"

#include <drogon/orm/DbConfig.h>
#include <string>
#include <unordered_map>

namespace efe
{
    class Config final : public Singleton<Config>
    {
    public:
        drogon::orm::PostgresConfig database;
        std::string jwtKey;

        /**
         * @brief Retoran true se as configurações foram carregadas.
         *
         * @return bool
         */
        bool getLoaded();

        /**
         * @brief Carrega as configurações do arquivo de configuração.
         * 
         * @return bool
         */
        bool load();

        /**
         * @brief Retorna o valor associado a chave informada.
         * 
         * @param key
         * @param default_value
         * @return std::string
         */
        std::string get(const std::string& key,
                        const std::string& default_value = "") const;

    private:
        friend class Singleton<Config>;
        Config() = default;
        ~Config() = default;

        bool loaded_ = false;
        std::unordered_map<std::string, std::string> config_;

        std::string trim(const std::string& str);
        bool pull();
    };
}
