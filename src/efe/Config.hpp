#pragma once

#include <drogon/orm/DbConfig.h>
#include <string>
#include <toml++/toml.hpp>

#include "efe/Singleton.hpp"

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

    private:
        friend class Singleton<Config>;
        Config() = default;
        ~Config() = default;

        bool loaded_ = false;
        toml::table config_;

        bool pull();
    };
}
