CPP = c++

CPPFLAGS = -Wall -Wextra -Werror -std=c++98

NAME = ircbot

SRCS = bot.cpp main.cpp

OBJS = $(SRCS:.cpp=.o)

HEADERS = bot.hpp ../src/header.hpp

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


