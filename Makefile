CPP = c++

# CPPFLAGS = -Wall -Wextra -Werror 

NAME = ircserv

SRCS = ./src/main.cpp ./src/Client.cpp ./src/Server.cpp   ./src/commande.cpp

OBJS = $(SRCS:.cpp=.o)

HEADERS = ./include/Server.hpp  ./include/Client.hpp  ./include/User.hpp  

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

