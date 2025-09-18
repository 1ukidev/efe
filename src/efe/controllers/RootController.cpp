#include <chrono>
#include <drogon/HttpAppFramework.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpTypes.h>
#include <drogon/HttpViewData.h>
#include <string>
#include <vector>

#include "efe/Global.hpp"
#include "efe/controllers/RootController.hpp"

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

        std::string rotas = "<h3>Rotas disponíveis:</h3>\n    <ul>";
        std::vector<HttpHandlerInfo> handlers = app().getHandlersInfo();
        for (const auto& handler : handlers) {
            rotas += "\n        <li><b>Caminho:</b> " + std::get<0>(handler)
                  + " <b>(" + std::get<2>(handler) + ")</b></li>";
        }
        rotas += "\n    </ul>";

        HttpViewData data{};
        data.insert("uptime", uptime);
        data.insert("rotas", rotas);

        auto resp = HttpResponse::newHttpViewResponse("Index.csp", data);
        callback(resp);
    }
}
