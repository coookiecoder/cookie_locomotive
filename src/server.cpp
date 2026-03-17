#include <server/udp.hpp>
#include <gpio/chip.hpp>

#include <iostream>

#include <unistd.h>

cookie::gpio::Chip chip("/dev/gpiochip0");

std::string up(std::string str) {
    chip.send_line_value(47, true);
    return str;
}

std::string down(std::string str) {
    chip.send_line_value(47, false);
    return str;
}

std::atomic_bool run = true;

std::string stop(std::string str) {
    run = false;
    return str;
}

int main() {
    chip.add_line_request(47, "cookie_locomotive", true);

    cookie::server::udp server(5000);

    server.map_response("up", up);
    server.map_response("down", down);
    server.map_response("stop", stop);

    server.start();

    while (run) {
        std::cout << "[INFO] | still alive" << std::endl;
	    usleep(1000000);
    }
}
