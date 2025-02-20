#include "efe/Config.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <string_view>
#include <thread>
#include <boost/program_options.hpp>
#include <drogon/drogon.h>
#include <trantor/utils/Logger.h>

using namespace drogon;
using namespace efe;
namespace po = boost::program_options;

static std::string_view art = R"(
       __
  ___ / _| ___
 / _ \ |_ / _ \
|  __/  _|  __/
 \___|_|  \___|
)";

static std::string_view version = "0.0.1";

int main(int argc, char** argv)
{
    unsigned int threadsCount = std::thread::hardware_concurrency();
    po::options_description desc("Opções disponíveis");

    desc.add_options()
        ("help,h", "Mostra essa mensagem de ajuda")
        ("version,v", "Exibe a versão do servidor")
        ("url,u", po::value<std::string>()->default_value("0.0.0.0"), "URL do servidor")
        ("port,p", po::value<unsigned short>()->default_value(9999), "Porta do servidor")
        ("threads,t", po::value<unsigned short>()->default_value(threadsCount), "Número de threads")
        ("debug,d", po::bool_switch()->default_value(false), "Exibe mensagens de debug");

    po::variables_map vm;

    try {
        store(parse_command_line(argc, argv, desc), vm);
        notify(vm);
    } catch (const po::error& e) {
        std::cerr << "Opção inválida. Use '--help' para mais informações.\n";
        return EXIT_FAILURE;
    }

    if (vm.count("help")) {
        std::cout << desc << '\n';
        return EXIT_SUCCESS;
    } else if (vm.count("version")) {
        std::cout << version << '\n';
        return EXIT_SUCCESS;
    }

    auto url = vm["url"].as<std::string>();
    auto port = vm["port"].as<unsigned short>();
    auto threads = vm["threads"].as<unsigned short>();
    auto debug = vm["debug"].as<bool>();

    std::cout << "\033c" << std::flush;
    std::cout << art << '\n';

    auto& config = Config::getInstance();
    if (!config.load()) {
        LOG_ERROR << "Erro ao carregar arquivo de configuração";
        return EXIT_FAILURE;
    }

    app().registerBeginningAdvice([port, threads]() {
        LOG_INFO << "Servidor inicializado com sucesso na porta " << port << " com " << threads << " thread(s)";
    });

    app().addDbClient(config.database)
         .setLogPath("", "", 100000000, 0, false)
         .setLogLevel(debug ? trantor::Logger::kDebug : trantor::Logger::kInfo)
         .addListener(url, port)
         .setThreadNum(threads)
         .enableGzip(false)
         .enableBrotli(true)
         .run();
}
