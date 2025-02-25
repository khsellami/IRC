CPP = c++

CPPFLAGS = -Wall -Wextra -Werror -std=c++98

NAME = ircserv


SRCS = ./src/main.cpp ./src/Client.cpp ./src/Server.cpp  ./src/Channel.cpp ./src/Msj.cpp ./src/parse_msg.cpp ./src/Auth.cpp

OBJS = $(SRCS:.cpp=.o)

HEADERS = ./include/Server.hpp  ./include/Client.hpp  ./include/Channel.hpp ./include/Msj.hpp ./include/Reply.hpp 

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP)  -o $@ $^

%.o: %.cpp $(HEADERS)
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(NAME)

fclean: clean
	rm -f $(NAME)

re: fclean all

