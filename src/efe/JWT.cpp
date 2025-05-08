#include "JWT.hpp"
#include "efe/Config.hpp"

#include <chrono>
#include <cstdint>
#include <string>
#include <jwt-cpp/jwt.h>
#include <utility>

namespace efe
{
    std::string JWT::generate(std::uint64_t usuarioId)
    {
        auto& config = Config::getInstance();

        return jwt::create()
            .set_type("JWT")
            .set_issuer("efe")
            .set_subject(std::to_string(usuarioId))
            .set_issued_at(std::chrono::system_clock::now())
            .sign(jwt::algorithm::hs256{config.jwtKey});
    }

    std::string JWT::verify(const std::string& token)
    {
        auto& config = Config::getInstance();

        try {
            std::string jwt = token;
            const std::string prefix = "Bearer ";
            if (jwt.rfind(prefix, 0) == 0) {
                jwt = jwt.substr(prefix.length());
            }

            auto decoded = jwt::decode(jwt);
            auto verifier = jwt::verify()
                .allow_algorithm(jwt::algorithm::hs256{config.jwtKey})
                .with_issuer("efe");

            verifier.verify(decoded);

            std::string sub = decoded.get_subject();

            return sub;
        } catch (const std::exception&) {
            return "";
        }
    }
}
