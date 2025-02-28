CPP = c++

CPPFLAGS = -Wall -Wextra -Werror -std=c++98

NAME = ircserv


SRCS = ./src/main.cpp ./src/Client.cpp ./src/Server.cpp  ./src/Channel.cpp ./src/parse_msj.cpp \
		  ./src/topic.cpp ./src/privmsg.cpp ./src/join.cpp ./src/Invite.cpp ./src/Auth.cpp ./src/Tools.cpp ./src/Mode.cpp 

OBJS = $(SRCS:.cpp=.o)

HEADERS = ./include/Server.hpp  ./include/Client.hpp  ./include/Channel.hpp ./include/Msj.hpp ./include/Reply.hpp ./include/header.hpp 

all: $(NAME)

$(NAME): $(OBJS)
	$(CPP)  -o $@ $^

%.o: %.cpp $(HEADERS)
	$(CPP) $(CPPFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

