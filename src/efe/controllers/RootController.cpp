#include "efe/Global.hpp"
#include "efe/controllers/RootController.hpp"

#include <chrono>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpTypes.h>
#include <drogon/HttpViewData.h>
#include <string>

namespace efe::controllers
{
    void RootController::asyncHandleHttpRequest(const HttpRequestPtr&, Callback&& callback)
    {
        auto& global = Global::getInstance();
        auto startTime = global.startTime;

        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - startTime).count();

        int hours = duration / 3600;
        int minutes = (duration % 3600) / 60;
        int seconds = duration % 60;

        std::string uptime = std::to_string(hours) + "h " +
                             std::to_string(minutes) + "m " +
                             std::to_string(seconds) + "s";

        HttpViewData data;
        data.insert("uptime", uptime);

        auto resp = HttpResponse::newHttpViewResponse("Index.csp", data);
        callback(resp);
    }
}
