#include <boost/asio.hpp>
#include <fstream>
#include <iostream>

using boost::asio::ip::tcp;

int main() {
    boost::asio::io_service io_service;
    tcp::socket socket(io_service);

    // Connect to server
    socket.connect(tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 5000));

    // Open audio file
    std::ifstream file("/home/yoojunho/바탕화면/file_example_WAV_1MG.wav", std::ios::binary);
    if (!file) {
        std::cerr << "Could not open audio file.\n";
        return 1;
    }

    // Read and send file content
    char buffer[512];
    while (!file.eof()) {
        file.read(buffer, sizeof(buffer));
        std::streamsize bytes = file.gcount();
        boost::asio::write(socket, boost::asio::buffer(buffer, bytes));
    }

    return 0;
}
