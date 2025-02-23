#include "JWT.hpp"
#include "efe/Constants.hpp"

#include <chrono>
#include <string>
#include <jwt-cpp/jwt.h>

namespace efe
{
    std::string generateToken(const std::string& userId)
    {
        return jwt::create()
            .set_type("JWT")
            .set_issuer("efe")
            .set_subject(userId)
            .set_issued_at(std::chrono::system_clock::now())
            .set_expires_at(std::chrono::system_clock::now() + std::chrono::hours(1))
            .sign(jwt::algorithm::hs256{secretKey.data()});
    }
}
