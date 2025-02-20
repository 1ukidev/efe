#pragma once

#include <drogon/orm/DbConfig.h>
#include <string>
#include <unordered_map>

namespace efe
{
    class Config final
    {
    public:
        // Singleton
        Config(const Config&) = delete;
        Config& operator=(const Config&) = delete;
        static Config& getInstance();

        drogon::orm::PostgresConfig database;

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
        std::string get(const std::string& key, const std::string& default_value = "") const;

    private:
        Config() = default;
        ~Config() = default;

        std::unordered_map<std::string, std::string> config_;
        std::string trim(const std::string& str);
        bool initDatabase();
    };
}
