NAME		= Webserv
SRC			= main.cpp srcs/utils.cpp srcs/socket.cpp srcs/response.cpp srcs/request.cpp srcs/conf.cpp srcs/confData.cpp srcs/location.cpp srcs/autoindex.cpp srcs/webServ.cpp srcs/cgi.cpp
OBJS	= $(SRC:.cpp=.o)
CXX			= c++ 
RM			= rm -f
CPPFLAGS		= -Wall -Wextra -Werror -Iinclude -g -std=c++98 

all:		$(NAME)

$(NAME):	$(OBJS)
			$(CXX) $(CPPFLAGS) -o $(NAME) $(OBJS)

clean:
			@$(RM) $(OBJS)

fclean:		clean
			$(RM) $(NAME)
			$(RM) $(OBJS)

test: $(NAME)
	@./Webserv conf/nginx.conf

re:			fclean all

save:
	git commit -am "$(MAKECMDGOALS)";
	git push https://github.com/4b93f/webserv.git

.PHONY:		all clean fclean re test save
