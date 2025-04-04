#include "JWT.hpp"
#include "efe/Config.hpp"
#include "efe/Constants.hpp"

#include <chrono>
#include <string>
#include <jwt-cpp/jwt.h>
#include <utility>

namespace efe
{
    std::string JWT::generateToken(const std::string& userId)
    {
        auto& config = Config::getInstance();

        return jwt::create()
            .set_type("JWT")
            .set_issuer("efe")
            .set_subject(userId)
            .set_issued_at(std::chrono::system_clock::now())
            .set_expires_at(std::chrono::system_clock::now() + std::chrono::hours(1))
            .sign(jwt::algorithm::hs256{config.jwtKey});
    }

    std::pair<bool, std::string> JWT::verify(const std::string& token)
    {
        auto& config = Config::getInstance();

        try {
            auto decoded = jwt::decode(token);
            auto verifier = jwt::verify()
                .allow_algorithm(jwt::algorithm::hs256{config.jwtKey})
                .with_issuer("efe");

            verifier.verify(decoded);

            std::string sub = decoded.get_subject();

            return {true, sub};
        } catch (const std::exception&) {
            return {false, ""};
        }
    }
}
