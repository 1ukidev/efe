#pragma once

#include "efe/Singleton.hpp"

#include <chrono>

namespace efe
{
    class Global final : public Singleton<Global>
    {
    public:
        std::chrono::steady_clock::time_point startTime;

    private:
        friend class Singleton<Global>;
        Global() = default;
        ~Global() = default;
    };
}
