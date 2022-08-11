NAME		= Webserv
SRC			= main.cpp srcs/utils.cpp srcs/socket.cpp srcs/response.cpp srcs/request.cpp srcs/conf.cpp srcs/confData.cpp srcs/location.cpp srcs/autoindex.cpp
OBJS	= $(SRC:.cpp=.o)
CC			= c++
RM			= rm -f
FLAGS		= -g3 -std=c++98 -I   .

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CC) $(FLAGS) -o $(NAME) $(OBJS)

clean:
			@$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)
			$(RM) $(OBJS)

re:			fclean all

test: $(NAME)
	@./Webserv conf/nginx.conf

save:
	git commit -am "$(MAKECMDGOALS)";
	git push https://github.com/4b93f/webserv.git

.PHONY:		all clean fclean re test