#pragma once

#include <chrono>

namespace efe
{
    class Global final
    {
    public:
        // Singleton
        Global(const Global&) = delete;
        Global& operator=(const Global&) = delete;
        static Global& getInstance();

        std::chrono::steady_clock::time_point startTime;

    private:
        Global() = default;
        ~Global() = default;
    };
}
