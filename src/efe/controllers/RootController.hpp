#pragma once

#include <drogon/HttpSimpleController.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpTypes.h>

namespace efe::controllers
{
    using namespace drogon;

    class RootController final : public HttpSimpleController<RootController>
    {
    public:
        using Callback = std::function<void(const HttpResponsePtr&)>;

        PATH_LIST_BEGIN
            PATH_ADD("/", Get);
        PATH_LIST_END

        void asyncHandleHttpRequest(const HttpRequestPtr& req, Callback&& callback) override;
    };
}
