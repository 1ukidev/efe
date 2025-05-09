#include "efe/Config.hpp"
#include "efe/Util.hpp"

#include <drogon/orm/DbConfig.h>
#include <filesystem>
#include <string>
#include <fstream>
#include <trantor/utils/Logger.h>

namespace efe
{
    bool Config::getLoaded()
    {
        return loaded_;
    }

    bool Config::load()
    {
        if (loaded_) {
            LOG_INFO << "Configurações já estão carregadas.";
            return true;
        }

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
        std::string path  = std::getenv("USERPROFILE");
        path += "\\efe.properties";
#else
        std::string path = std::getenv("HOME");
        path += "/efe.properties";
#endif

        if (!std::filesystem::exists(path)) {
            LOG_ERROR << "Arquivo de configuração não encontrado: " << path;
            return false;
        }

        std::ifstream file(path);
        if (!file.is_open()) {
            LOG_ERROR << "Não foi possível abrir o arquivo de configuração: " << path;
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#')
                continue;

            std::stringstream ss(line);
            std::string key, value;

            if (std::getline(ss, key, '=') && std::getline(ss, value)) {
                key = trim(key);
                value = trim(value);
                config_[key] = value;
            }
        }

        if (!pull())
            return false;

        loaded_ = true;

        return true;
    }

    std::string Config::get(const std::string& key, const std::string& defaultValue) const
    {
        auto it = config_.find(key);
        return it != config_.end() ? it->second : defaultValue;
    }

    std::string Config::trim(const std::string& str)
    {
        size_t first = str.find_first_not_of(" \t\n\r");
        size_t last = str.find_last_not_of(" \t\n\r");
        return first == std::string::npos || last == std::string::npos ? "" : str.substr(first, last - first + 1);
    }

    bool Config::pull()
    {
        database.name = "default";
        database.isFast = true;
        database.host = get("db.host");

        std::string port = get("db.port");
        database.port = Util::isNumber(port) ? std::stoul(port) : 0;

        database.username = get("db.user");
        database.password = get("db.password");
        database.databaseName = get("db.databaseName");

        std::string connectionNumber = get("db.connectionNumber");
        database.connectionNumber = Util::isNumber(connectionNumber) ? std::stoul(connectionNumber) : 0;

        jwtKey = get("jwt.key");

        if (database.host.empty()) LOG_ERROR << "Configuração ausente: db.host";
        if (database.port == 0) LOG_ERROR << "Configuração inválida ou ausente: db.port";
        if (database.username.empty()) LOG_ERROR << "Configuração ausente: db.user";
        if (database.password.empty()) LOG_ERROR << "Configuração ausente: db.password";
        if (database.databaseName.empty()) LOG_ERROR << "Configuração ausente: db.databaseName";
        if (database.connectionNumber == 0) LOG_ERROR << "Configuração inválida ou ausente: db.connectionNumber";
        if (jwtKey.empty()) LOG_ERROR << "Configuração ausente: jwt.key";

        if (database.host.empty() || database.port == 0 || database.username.empty() || database.password.empty()
                || database.databaseName.empty() || database.connectionNumber == 0 || jwtKey.empty())
            return false;

        return true;
    }
}
