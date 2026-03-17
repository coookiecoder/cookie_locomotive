#include <server/udp.hpp>
#include <gpio/chip.hpp>

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
    cookie::gpio::Chip chip("/dev/gpiochip0");
    chip.add_line_request(47, "cookie_locomotive", true);

    cookie::server::udp server(5000);

    server.map_default(callback);
    server.map_response_word("stop", stop);

    server.start();

    while (run) {
        std::cout << "[INFO] | still alive" << std::endl;

        chip.send_line_value(47, true);
	    usleep(500000);
        chip.send_line_value(47, false);
        usleep(500000);
    }
}
