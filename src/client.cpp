#include <client/udp.hpp>
#include <gpio/chip.hpp>

#include <iostream>

#include <unistd.h>

cookie::gpio::Chip chip("/dev/gpiochip0");

std::string up(const std::string&) {
    chip.send_line_value(47, true);
    return "";
}

std::string down(const std::string&) {
    chip.send_line_value(47, false);
    return "";
}

int main() {
    chip.add_line_request(23, "cookie_locomotive", true);
    chip.send_line_value(23, true);
}
