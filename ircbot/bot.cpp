#include "bot.hpp"

// make some quotes
std::string quotes[20] = {
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

void handle_message(std::string message, int fd)
{
    std::string target = message.substr(0, message.find(" "));
    std::cout << "target: [" << target << "]" << std::endl;
    std::cout << "message: [" << message << "]" << std::endl;  
    if (message.find("!HELP") != std::string::npos)
    {
        std::string help_message = "PRIVMSG " + target + " :Hello, I am the bot. I can help you with the following commands:\n";
        help_message += "!HELP: Show this help message\n";
        help_message += "!TIME: Show the current time\n";
        help_message += "!QUOTE: Show a random quote\n";
        send(fd, help_message.c_str(), help_message.size() + 10, 0);
    }
    else if (message.find("!TIME") != std::string::npos)
    {
        time_t now = time(0);
        tm *local_time = localtime(&now);
        std::string time_message = "PRIVMSG " + target + " :The current time is " + std::to_string(local_time->tm_hour) + ":" + std::to_string(local_time->tm_min) + ":" + std::to_string(local_time->tm_sec) + "\r\n";
        if (send(fd, time_message.c_str(), time_message.size(), 0) < 0)
        {
            std::cerr << "send() failed" << std::endl;
        }
    }
    else if (message.find("!QUOTE") != std::string::npos)
    {
        int random_index = getRandom20();
        std::string quote_message = "PRIVMSG " + target + " :" + quotes[random_index] + "\r\n";
        if (send(fd, quote_message.c_str(), quote_message.size(), 0) < 0)
        {
            std::cerr << "send() failed" << std::endl;
        }
    }

}

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
    sleep(1);
    message = "NICK bot\r\n";
    if (send(fd, message.c_str(), message.size(), 0) < 0)
    {
        std::cerr << "send() failed" << std::endl;
        return 1;
    }
    sleep(1);
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
            handle_message(response, fd);
        }
    }
}

/*
./ircbot <ip> <port> <password>
client
socket();
connect();
*/