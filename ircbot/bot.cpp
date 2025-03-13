#include "bot.hpp"

const std::string Bot::quotes[20] = {
    "The only way to do great work is to love what you do. - Steve Jobs",
    "In the middle of every difficulty lies opportunity. - Albert Einstein",
    "The future belongs to those who believe in the beauty of their dreams. - Eleanor Roosevelt",
    "The only limit to our realization of tomorrow is our doubts of today. - Franklin D. Roosevelt",
    "Be the change you wish to see in the world. - Mahatma Gandhi",
    "Success is not final, failure is not fatal: It is the courage to continue that counts. - Winston Churchill",
    "It does not matter how slowly you go as long as you do not stop. - Confucius",
    "The journey of a thousand miles begins with one step. - Lao Tzu",
    "You miss 100% of the shots you don't take. - Wayne Gretzky",
    "The greatest glory in living lies not in never falling, but in rising every time we fall. - Nelson Mandela",
    "Life is what happens when you're busy making other plans. - John Lennon",
    "The only true wisdom is in knowing you know nothing. - Socrates",
    "Do not go where the path may lead, go instead where there is no path and leave a trail. - Ralph Waldo Emerson",
    "Twenty years from now you will be more disappointed by the things you didn't do than by the ones you did do. - Mark Twain",
    "People will forget what you said, people will forget what you did, but people will never forget how you made them feel. - Maya Angelou",
    "The biggest adventure you can take is to live the life of your dreams. - Oprah Winfrey",
    "Life is either a daring adventure or nothing at all. - Helen Keller",
    "Change is the only constant in life. - Heraclitus",
    "The successful warrior is the average man, with laser-like focus. - Bruce Lee",
    "Have no fear of perfection - you'll never reach it. - Salvador Dali"
};

int getRandom20()
{
    return rand() % 20;
}

Bot::Bot()
{
    registred = false;
    srand(time(0)); // Seed random number generator
}

void Bot::setSocketFd(int fd)
{
    this->fd = fd;
}

bool Bot::isRegistred()
{
    return registred;
}

void Bot::setRegistred(bool registred)
{
    this->registred = registred;
}

int Bot::getSocketFd()
{
    return this->fd;
}

void Bot::setServerInfos(std::string ip, std::string port)
{
    //needs more protections here !!!
    server_addr.sin_family = AF_INET;
    if (port.empty())
    {
        std::cerr << "Port is empty" << std::endl;
        close(fd);
        exit(1);
    }
    server_addr.sin_port = htons(atoi(port.c_str()));
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());
}

void Bot::connectToServer()
{
    if (connect(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "connect() failed" << std::endl;
        close(fd);
        exit(1);
    }
    std::cout << "Connected to server" << std::endl;
}

void Bot::checkRegistred(std::string response)
{
    if (response.find("464") != std::string::npos)
    {
        std::cerr << "Password incorrect" << std::endl;
        close(fd);
        exit(1);
    }
    else if (response.find("001") != std::string::npos)
    {
       std::cout << "Registered Successfully" << std::endl;
       setRegistred(true);
    }
}

void Bot::registerBot(std::string password)
{
    std::string message = "PASS " + password + "\r\n";
    if (send(fd, message.c_str(), message.size(), 0) < 0)
    {
        std::cerr << "send() failed" << std::endl;
        close(fd);
        exit(1);
    }
    message = "NICK bot\r\n";
    if (send(fd, message.c_str(), message.size(), 0) < 0)
    {
        std::cerr << "send() failed" << std::endl;
        close(fd);
        exit(1);
    }
    message = "USER bot 0 * :bot\r\n";
    if (send(fd, message.c_str(), message.size(), 0) < 0)
    {
        std::cerr << "send() failed" << std::endl;
        close(fd);
        exit(1);
    }
}

std::string trim(const std::string &str)
{
	size_t first = str.find_first_not_of(" \t\r\0");
	if (first == std::string::npos) return "";
	size_t last = str.find_last_not_of(" \t\n");
	return str.substr(first, last - first + 1);
}

std::string extractCommand(std::string message)
{
    message = message.substr(message.find(" "));
    std::string command;
    for (size_t i = 0; i < message.size() && message[i] != '\r' && message[i] != '\n'; i++)
    {
        command += message[i];
    }
    return command;
}

void Bot::handle_message(std::string message)
{
    std::string target = message.substr(0, message.find(" ")); 
    std::string command = extractCommand(message);
    command = trim(command);
    if (command == "!HELP")
    {
        std::string help_message = "PRIVMSG " + target + " :Hello, I am a Utility bot. I can help you with the following commands:\r\n";
        send(fd, help_message.c_str(), help_message.size(), 0);
        help_message = "PRIVMSG " + target + " :!HELP: Show this help message.\r\n";
        send(fd, help_message.c_str(), help_message.size(), 0);
        help_message = "PRIVMSG " + target + " :!TIME: Show the current time.\r\n";
        send(fd, help_message.c_str(), help_message.size(), 0);
        help_message = "PRIVMSG " + target + " :!QUOTE: Show a random quote.\r\n";
        send(fd, help_message.c_str(), help_message.size(), 0);
    }
    else if (command == "!TIME")
    {
        time_t now = time(0); 
        tm *local_time = localtime(&now);
        std::string time_message = "PRIVMSG " + target + " :The current time is " + std::to_string(local_time->tm_hour) + ":" + std::to_string(local_time->tm_min) + ":" + std::to_string(local_time->tm_sec) + "\r\n";
        if (send(fd, time_message.c_str(), time_message.size(), 0) < 0)
        {
            std::cerr << "send() failed" << std::endl;
        }
    }
    else if (command == "!QUOTE")
    {
        int random_index = getRandom20();
        std::string quote_message = "PRIVMSG " + target + " :" + quotes[random_index] + "\r\n";
        if (send(fd, quote_message.c_str(), quote_message.size(), 0) < 0)
        {
            std::cerr << "send() failed" << std::endl;
        }
    }
}

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cerr << "Usage: <Server IP> <Server Port> <Server Password>" << std::endl;
        return 1;
    }
    std::string ip(argv[1]);
    std::string port(argv[2]);
    std::string password(argv[3]);
    Bot bot;
    bot.setSocketFd(socket(AF_INET, SOCK_STREAM, 0));
    if (bot.getSocketFd() < 0)
    {
        std::cerr << "socket() failed" << std::endl;
        return 1;
    }
    bot.setServerInfos(ip, port);
    bot.connectToServer();
    bot.registerBot(password);
    while (true)
    {
        char buffer[1024];
        int bytes_received = recv(bot.getSocketFd(), buffer, sizeof(buffer), 0);
        if (bytes_received <= 0)
        {
            std::cerr << "Connection closed" << std::endl;
            break;
        }
        std::string response(buffer, bytes_received);
        if (!bot.isRegistred())
            bot.checkRegistred(response);
        if (bot.isRegistred())
            bot.handle_message(response);
    }
    close(bot.getSocketFd());
}
