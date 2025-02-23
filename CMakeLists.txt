cmake_minimum_required(VERSION 3.25)

project(efe LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -g")

add_executable(
    efe
    "src/main.cpp"
    "src/efe/Config.cpp"
    "src/efe/Config.hpp"
    "src/efe/Constants.hpp"
    "src/efe/JSON.cpp"
    "src/efe/JSON.hpp"
    "src/efe/JWT.cpp"
    "src/efe/JWT.hpp"
    "src/efe/Util.cpp"
    "src/efe/Util.hpp"
    "src/efe/controllers/RootController.cpp"
    "src/efe/controllers/RootController.hpp"
    "src/efe/controllers/UsuarioController.cpp"
    "src/efe/controllers/UsuarioController.hpp"
    "src/efe/configuracao/entities/UsuarioEntity.cpp"
    "src/efe/configuracao/entities/UsuarioEntity.hpp"
    "src/efe/configuracao/dao/UsuarioDAO.cpp"
    "src/efe/configuracao/dao/UsuarioDAO.hpp"
    "src/efe/DAO.hpp"
    "src/efe/Entity.hpp"
)

target_include_directories(efe PRIVATE src resources)

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)

find_package(Drogon CONFIG REQUIRED)
find_package(Boost CONFIG REQUIRED COMPONENTS headers program_options)
find_package(jwt-cpp CONFIG REQUIRED)

target_link_libraries(
    efe PRIVATE
    Drogon::Drogon
    Boost::headers
    Boost::program_options
    jwt-cpp::jwt-cpp
)
