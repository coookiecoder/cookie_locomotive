#include <server/udp.hpp>

#include <iostream>
#include <mutex>

cookie::server::udp server(5000);
std::mutex mutex;

std::string callback(std::string msg, sockaddr_in client) {
    std::lock_guard lock(mutex);

    std::cout << "received client on port : " << client.sin_port << std::endl;

    for (const auto& address : server.client_addresses)
        if (address.sin_addr.s_addr == client.sin_addr.s_addr && address.sin_port == client.sin_port)
            return msg;

    server.client_addresses.push_back(client);

    std::cout << "added client on port : " << client.sin_port << std::endl;

    return msg;
}

std::atomic_bool run = true;

std::string stop(std::string str, sockaddr_in) {
    run = false;
    return str;
}

int main() {
    server.map_default(callback);
    server.map_response_word("stop", stop);

    server.start();

    std::unique_lock lock(mutex);
    lock.unlock();

    while (run) {
        lock.lock();
        server.broadcast("up");
        lock.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));

        lock.lock();
        server.broadcast("down");
        lock.unlock();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}
