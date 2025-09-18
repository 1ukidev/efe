#include <charconv>
#include <string_view>
#include <system_error>

#include "efe/Util.hpp"

namespace efe
{
    bool Util::isNumber(std::string_view str)
    {
        double d;
        auto result = std::from_chars(str.data(), str.data() + str.size(), d);
        return result.ec == std::errc() && result.ptr == str.data() + str.size();
    }
}
