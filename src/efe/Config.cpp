#include <drogon/orm/DbConfig.h>
#include <filesystem>
#include <string>
#include <toml++/toml.hpp>
#include <trantor/utils/Logger.h>

#include "efe/Config.hpp"

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
        path += "\\efe.toml";
#else
        std::string path = std::getenv("HOME");
        path += "/efe.toml";
#endif

        if (!std::filesystem::exists(path)) {
            LOG_ERROR << "Arquivo de configuração não encontrado: " << path;
            return false;
        }

        try {
            config_ = toml::parse_file(path);
        } catch (const toml::parse_error& e) {
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

        const auto& db = config_["db"];
        database.host = db["host"].value_or("");
        database.port = db["port"].value_or(0);
        database.username = db["user"].value_or("");
        database.password = db["password"].value_or("");
        database.databaseName = db["databaseName"].value_or("");
        database.connectionNumber = db["connectionNumber"].value_or(0);

        const auto& jwt = config_["jwt"];
        jwtKey = jwt["key"].value_or("");

        // TODO: Implementar verificações

        return true;
    }
}
