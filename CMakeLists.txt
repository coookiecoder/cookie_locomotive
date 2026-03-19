cmake_minimum_required(VERSION 3.14)

project(cookie_locomotive VERSION 0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

message(STATUS "Fetching Dependencies...")

# --- cookie library ---
FetchContent_Declare(cookie GIT_REPOSITORY https://gitlab.cookie-coder.com/cookiecoder/cookie.git)
FetchContent_MakeAvailable(cookie)

set(SOURCES_CLIENT src/client.cpp)
set(HEADERS_CLIENT)

set(SOURCES_SERVER src/server.cpp)
set(HEADERS_SERVER)

add_executable(cookie_locomotive ${SOURCES_CLIENT} ${HEADERS_CLIENT})
add_executable(cookie_central ${SOURCES_SERVER} ${HEADERS_SERVER})

target_link_libraries(cookie_locomotive PRIVATE cookie -static-libstdc++ -static-libgcc)
target_compile_options(cookie_locomotive PRIVATE -Os)

target_link_libraries(cookie_central PRIVATE cookie -static-libstdc++ -static-libgcc)
target_compile_options(cookie_central PRIVATE -Os)

set(DOCKER_IMAGE_NAME "cookie_locomotive")
add_custom_target(docker-build COMMAND docker build -t ${DOCKER_IMAGE_NAME} ${CMAKE_SOURCE_DIR} WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} COMMENT "Building inside docker using alpine ARMHF target")
