#include "Server.hpp"
#include "Client.hpp"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <port> <password>" << std::endl;
        return 1;
    }

    //port to num
    int port;
    std::stringstream(argv[1]) >> port;
    std::string password = argv[2];

    try {
        Server ircServer(SERVER_NAME, port, password);
        ircServer.start();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
