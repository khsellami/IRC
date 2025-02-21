#include "IRCServer.hpp"
// int main(int argc, char **argv) {
//     if (argc != 3) {
//         std::cerr << "Usage: ./ircserv <port> <password>" << std::endl;
//         return 1;
//     }
//     int port = std::stoi(argv[1]);
//     std::string password = argv[2];

//     // Initialisation du serveur
//     IRCServer server(port, password);
//     server.start();
// }


int main() {
    IRCServer server(6667, "secret");
    server.start();
    return 0;
}
