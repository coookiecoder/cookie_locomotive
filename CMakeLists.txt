cmake_minimum_required(VERSION 3.14)

project(cookie_template VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

message(STATUS "Fetching Dependencies...")

# --- cookie library ---
FetchContent_Declare(cookie GIT_REPOSITORY https://gitlab.cookie-coder.com/cookiecoder/cookie.git)
FetchContent_MakeAvailable(cookie)

set(SOURCES_SERVER src/server.cpp)
set(HEADERS_SERVER)

set(SOURCES_CLIENT src/client.cpp)
set(HEADERS_CLIENT)

add_executable(server ${SOURCES_SERVER} ${HEADERS_SERVER})
add_executable(client ${SOURCES_CLIENT} ${HEADERS_CLIENT})

target_link_libraries(server PRIVATE cookie)
target_link_libraries(client PRIVATE cookie)