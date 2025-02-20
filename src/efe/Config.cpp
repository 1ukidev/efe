#include "efe/Config.hpp"
#include "efe/Util.hpp"

#include <drogon/orm/DbConfig.h>
#include <filesystem>
#include <string>
#include <fstream>
#include <trantor/utils/Logger.h>

namespace efe
{
    Config& Config::getInstance()
    {
        static Config instance;
        return instance;
    }

    bool Config::load()
    {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
        std::string path  = std::getenv("HOMEPATH");
#else
        std::string path = std::getenv("HOME");
#endif
        path += "/efe.properties";

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

        if (!initDatabase())
            return false;

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

    bool Config::initDatabase()
    {
        database.name = "default";
        database.isFast = get("db.isFast") == "true";
        database.host = get("db.host");
        database.port = std::stoul(get("db.port"));
        database.username = get("db.user");
        database.password = get("db.password");
        database.databaseName = get("db.databaseName");

        std::string port = get("db.port");
        database.port = Util::isNumber(port) ? std::stoul(port) : 0;

        std::string connectionNumber = get("db.connectionNumber");
        database.connectionNumber = Util::isNumber(connectionNumber) ? std::stoul(connectionNumber) : 0;

        if (database.host.empty() || database.port == 0 || database.username.empty() || database.password.empty()
                || database.databaseName.empty() || database.connectionNumber == 0) {
            LOG_ERROR << "Arquivo de configuração incompleto";
            return false;
        }

        return true;
    }
}
