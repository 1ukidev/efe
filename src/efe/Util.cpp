#include "efe/Util.hpp"
#include "efe/Config.hpp"

#include <algorithm>
#include <drogon/HttpAppFramework.h>
#include <drogon/HttpTypes.h>
#include <drogon/orm/DbClient.h>
#include <string_view>

namespace efe
{
    using namespace drogon;

    orm::DbClientPtr Util::getDbClient()
    {
        auto& config = Config::getInstance();
        return config.database.isFast ? app().getFastDbClient() : app().getDbClient();
    }

    bool Util::isNumber(std::string_view str)
    {
        return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
    }
}
