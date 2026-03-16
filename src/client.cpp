#include <client/udp.hpp>

#include <iostream>

std::string callback(const std::string& str) {
    std::cout << "New message from server : " << str << std::endl;
    return "";
}

int main() {
    cookie::client::udp client("127.0.0.1", 5000);

    client.map_default(callback);

    client.start();

    client.send("hello world");
    client.send("stop");
    return 0;
}