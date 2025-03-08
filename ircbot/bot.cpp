#include "bot.hpp"






//do not forget to check and marque the nick name of the bot as NICKINUSE
int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: <ip> <port> <password>" << std::endl;
        return 1;
    }
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0)
    {
        std::cerr << "socket() failed" << std::endl;
        return 1;
    }
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(atoi(argv[2]));
    server_addr.sin_addr.s_addr = inet_addr(argv[1]);
    if (connect(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "connect() failed" << std::endl;
        return 1;
    }
    //send the auth messages
    
    std::string message = "PASS " + std::string(argv[3]) + "\r\n";
    if (send(fd, message.c_str(), message.size(), 0) < 0)
    {
        std::cerr << "send() failed" << std::endl;
        return 1;
    }
    sleep(2);
    message = "NICK bot\r\n";
    if (send(fd, message.c_str(), message.size(), 0) < 0)
    {
        std::cerr << "send() failed" << std::endl;
        return 1;
    }
    sleep(2);
    message = "USER bot 0 * :bot\r\n";
    if (send(fd, message.c_str(), message.size(), 0) < 0)
    {
        std::cerr << "send() failed" << std::endl;
        return 1;
    }
    while (true)
    {
        char buffer[1024];
        int bytes_received = recv(fd, buffer, sizeof(buffer), 0);
        std::cout << "bytes_received: " << bytes_received << std::endl;
        if (bytes_received <= 0) {
            std::cerr << "Connection closed" << std::endl;
            break;
        }
        std::string response(buffer, bytes_received);
        //check password
        if (response.find("464") != std::string::npos)
        {
            std::cerr << "Password incorrect" << std::endl;
            break;
        }
        else if (response.find("001") != std::string::npos)
        {
            std::cout << "Connected to server" << std::endl;
        }
        else
        {
            std::cout << "Received message: " << response << std::endl;
            // handle_message(response);
        }
    }
}

/*
./ircbot <ip> <port> <password>
client
socket();
connect();
*/