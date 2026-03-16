#include <server/udp.hpp>

#include <iostream>

#include <unistd.h>

std::string callback(std::string str) {
    std::cout << "New message from client : " << str << std::endl;
    return str;
}

std::atomic_bool run = true;

std::string stop(std::string str) {
    run = false;
    return str;
}

int main() {
    cookie::server::udp server(5000);

    server.map_default(callback);
    server.map_response_word("stop", stop);

    server.start();

    while (run) {
        std::cout << "[INFO] | still alive" << std::endl;

	usleep(1000000);
    }
}
