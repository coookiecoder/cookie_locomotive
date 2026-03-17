cmake_minimum_required(VERSION 3.14)

project(cookie_locomotive VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

message(STATUS "Fetching Dependencies...")

# --- cookie library ---
FetchContent_Declare(cookie GIT_REPOSITORY https://gitlab.cookie-coder.com/cookiecoder/cookie.git)
FetchContent_MakeAvailable(cookie)

set(SOURCES_SERVER src/server.cpp)
set(HEADERS_SERVER)

add_executable(init ${SOURCES_SERVER} ${HEADERS_SERVER})

target_link_libraries(init PRIVATE cookie -static-libstdc++ -static-libgcc)