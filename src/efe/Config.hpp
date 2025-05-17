#pragma once

#include "efe/Singleton.hpp"

#include <drogon/orm/DbConfig.h>
#include <string>
#include <yaml-cpp/yaml.h>

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
        YAML::Node config_;

        bool pull();
    };
}
