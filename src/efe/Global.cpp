#include "efe/Global.hpp"

namespace efe
{
    Global& Global::getInstance()
    {
        static Global instance;
        return instance;
    }
}
