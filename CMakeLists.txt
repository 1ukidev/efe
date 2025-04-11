cmake_minimum_required(VERSION 3.25)

project(efe LANGUAGES CXX VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -g")

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(VIEWS_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/views")

file(GLOB_RECURSE PROJECT_SOURCES
    "${SRC_DIR}/*.cpp"
)

file(MAKE_DIRECTORY ${VIEWS_OUTPUT_DIR})

file(GLOB CSP_LIST ${CMAKE_CURRENT_SOURCE_DIR}/views/*.csp)
set(VIEW_SOURCES "")

foreach(csp_file ${CSP_LIST})
    get_filename_component(filename ${csp_file} NAME_WE)

    message(STATUS "Processing CSP file: ${csp_file}")
    message(STATUS "View classname: ${filename}")

    set(h_file "${VIEWS_OUTPUT_DIR}/${filename}.h")
    set(cc_file "${VIEWS_OUTPUT_DIR}/${filename}.cc")

    add_custom_command(
        OUTPUT ${h_file} ${cc_file}
        COMMAND drogon_ctl create view ${csp_file}
        WORKING_DIRECTORY ${VIEWS_OUTPUT_DIR}
        DEPENDS ${csp_file}
        COMMENT "Generating view files for ${filename}"
        VERBATIM
    )

    list(APPEND VIEW_SOURCES ${cc_file})
endforeach()

add_executable(
    efe
    ${PROJECT_SOURCES}
    ${VIEW_SOURCES}
)

target_include_directories(
    efe PRIVATE
    ${SRC_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    ${VIEWS_OUTPUT_DIR}
)

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
