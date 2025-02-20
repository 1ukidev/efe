#include "efe/controllers/RootController.hpp"
#include "html/index.hpp"

#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpTypes.h>

namespace efe::controllers
{
    void RootController::asyncHandleHttpRequest(const HttpRequestPtr&, Callback&& callback)
    {
        auto resp = HttpResponse::newHttpResponse(k200OK, CT_TEXT_HTML);
        resp->setBody(reinterpret_cast<const char*>(html::index_html));
        callback(resp);
    }
}
