#include <client/udp.hpp>
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

    cookie::client::udp client("192.168.1.100", 5000);

    client.map_response("up", up);
    client.map_response("down", down);
    client.map_response("stop", stop);

    client.start();

    client.send("INIT");

    while (run) {
        std::cout << "[INFO] | still alive" << std::endl;
	    usleep(1000000);
    }
}
