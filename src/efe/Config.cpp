#include "efe/Config.hpp"

#include <drogon/orm/DbConfig.h>
#include <filesystem>
#include <string>
#include <trantor/utils/Logger.h>
#include <utility>
#include <vector>
#include <yaml-cpp/exceptions.h>
#include <yaml-cpp/node/parse.h>

namespace efe
{
    bool Config::getLoaded()
    {
        return loaded_;
    }

    bool Config::load()
    {
        if (loaded_) {
            LOG_INFO << "Configurações já estão carregadas";
            return true;
        }

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
        std::string path  = std::getenv("USERPROFILE");
        path += "\\efe.yaml";
#else
        std::string path = std::getenv("HOME");
        path += "/efe.yaml";
#endif

        if (!std::filesystem::exists(path)) {
            LOG_ERROR << "Arquivo de configuração não encontrado: " << path;
            return false;
        }

        try {
            config_ = YAML::LoadFile(path);
        } catch (const YAML::Exception& e) {
            LOG_ERROR << "Erro ao carregar o arquivo de configuração: " << e.what();
            return false;
        }

        if (!pull())
            return false;

        loaded_ = true;

        return true;
    }

    bool Config::pull()
    {
        database.name = "default";
        database.isFast = true;

        const std::vector<std::pair<std::string, std::vector<std::string>>> requiredKeys = {
            {"db", {"host", "port", "user", "password", "databaseName", "connectionNumber"}},
            {"jwt", {"key"}}
        };

        for (const auto& [section, keys] : requiredKeys) {
            if (!config_[section]) {
                LOG_ERROR << "Configuração ausente: " << section;
                return false;
            }
            for (const auto& key : keys) {
                if (!config_[section][key]) {
                    LOG_ERROR << "Configuração ausente: " << section << '.' << key;
                    return false;
                }
            }
        }

        try {
            const auto& db = config_["db"];
            database.host = db["host"].as<std::string>();
            database.port = db["port"].as<unsigned short>();
            database.username = db["user"].as<std::string>();
            database.password = db["password"].as<std::string>();
            database.databaseName = db["databaseName"].as<std::string>();
            database.connectionNumber = db["connectionNumber"].as<size_t>();

            const auto& jwt = config_["jwt"];
            jwtKey = jwt["key"].as<std::string>();
        } catch (const YAML::Exception& e) {
            LOG_ERROR << "Erro ao tentar pegar valores da configuração: " << e.what();
            return false;
        }

        return true;
    }
}
