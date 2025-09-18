// Copyright (c) 1ukidev <leo.monteiro06@live.com>. Licensed under the MIT Licence.
// See the LICENCE file in the repository root for full licence text.

#include <boost/program_options.hpp>
#include <chrono>
#include <cstdlib>
#include <drogon/drogon.h>
#include <iostream>
#include <string>
#include <string_view>
#include <thread>
#include <trantor/utils/Logger.h>

#include "efe/Config.hpp"
#include "efe/Constants.hpp"
#include "efe/Global.hpp"
#include "efe/JSON.hpp"

using namespace drogon;
using namespace trantor;
using namespace efe;
namespace po = boost::program_options;

static constexpr std::string_view art = R"(
       __
  ___ / _| ___
 / _ \ |_ / _ \
|  __/  _|  __/
 \___|_|  \___|
)";

void exceptionHandler(const std::exception& e,
                      const HttpRequestPtr& req,
                      std::function<void(const HttpResponsePtr&)>&& callback)
{
    LOG_ERROR << "Ocorreu uma exceção: " << e.what() << " (" << req->getPath() << ')';
    auto resp = HttpResponse::newHttpResponse(k500InternalServerError, CT_APPLICATION_JSON);
    resp->setBody(JSON::createResponse(e.what(), jt::error));
    callback(resp);
}

int main(int argc, char** argv)
{
    unsigned int threadsCount = std::thread::hardware_concurrency();
    po::options_description desc{"Opções disponíveis"};

    desc.add_options()
        ("help,h", "Mostra essa mensagem de ajuda")
        ("version,v", "Exibe a versão do servidor")
        ("url,u", po::value<std::string>()->default_value("0.0.0.0"), "URL do servidor")
        ("port,p", po::value<short>()->default_value(9999), "Porta do servidor")
        ("threads,t", po::value<short>()->default_value(threadsCount), "Número de threads");

    po::variables_map vm{};

    try {
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);
    } catch (const po::error&) {
        std::cerr << "Opção inválida. Use '--help' para mais informações.\n";
        return EXIT_FAILURE;
    }

    if (vm.count("help")) {
        std::cout << desc << '\n';
        return EXIT_SUCCESS;
    } else if (vm.count("version")) {
        std::cout << Constants::version << '\n';
        return EXIT_SUCCESS;
    }

    std::set_terminate([]() {
        LOG_ERROR << "Ops...";
        LOG_ERROR << "std::terminate() chamado";

        auto ex = std::current_exception();
        if (ex) {
            try {
                std::rethrow_exception(ex);
            } catch (const std::exception& e) {
                LOG_ERROR << "Exceção não capturada: " << e.what();
            } catch (...) {
                LOG_ERROR << "Exceção desconhecida";
            }
        } else {
            LOG_ERROR << "std::terminate sem exceção ativa";
        }
    });

    std::atexit([]() {
        LOG_WARN << "std::exit() chamado. Saindo...";
    });

    auto startTime = std::chrono::steady_clock::now();

    auto& global = Global::getInstance();
    global.startTime = startTime;

    auto url = vm["url"].as<std::string>();
    auto port = vm["port"].as<short>();
    auto threads = vm["threads"].as<short>();

    if (threads <= 0) threads = threadsCount;
    if (port <= 0) port = 9999;

    std::cout << art << '\n';

    auto& config = Config::getInstance();
    if (!config.load()) {
        return EXIT_FAILURE;
    }

    app().registerBeginningAdvice([port, threads, startTime]() {
        auto endTime = std::chrono::steady_clock::now();
        auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();

        LOG_INFO << "Servidor inicializado com sucesso na porta " << port << " com " << threads << " thread(s)";
        LOG_INFO << "Levou " << elapsedTime << " ms para inicializar";
    });

    app().setExceptionHandler(exceptionHandler)
         .addDbClient(config.database)
         .setLogPath("", "", 100000000, 0, true)
         .setLogLevel(Logger::kDebug)
         .addListener(url, port)
         .setThreadNum(threads)
         .enableGzip(true)
         .enableBrotli(true)
         .run();
}
