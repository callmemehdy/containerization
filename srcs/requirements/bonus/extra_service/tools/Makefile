serv_srcs	=	Server.cpp Client.cpp Channel.cpp Server_tools.cpp Request_handling.cpp
cmd_srcs	=	Auth.cpp PrivMsg.cpp Invite.cpp Join.cpp Kick.cpp Mode.cpp \
				Topic.cpp Auth_toolbox.cpp
fsrcs		=	main.cpp
srcs		=	$(addprefix srcs/, $(fsrcs)) \
				$(addprefix srcs/server/, $(serv_srcs)) \
				$(addprefix srcs/commands/, $(cmd_srcs)) 
objs		=	$(addprefix objs/, $(fsrcs:.cpp=.o)) \
				$(addprefix objs/, $(serv_srcs:.cpp=.o)) \
				$(addprefix objs/, $(cmd_srcs:.cpp=.o)) 
flags		=	-MMD -Iincs -g3  #-fsanitize=address #-Wshadow  -Wall -Wextra -Werror -std=c++98 -g3
NAME		=	ircserv

#bonus part file and variables...
BNAME		=	AsayadBot
bfsrcs		=	asayadBot.cpp BotMethods.cpp BotParseTools.cpp\
				BotCommands.cpp
bsrcs		=	$(addprefix srcs/, $(bfsrcs))
bobjs		=	$(addprefix objs/, $(bfsrcs:.cpp=.o))

all:	$(NAME)

bonus:	$(BNAME)

run:	all
	@if [ -f $(NAME) ]; \
		then clear && /bin/bash -c './$(NAME) "$$@"' __ "1337" "pass"; \
	fi

run_bonus:	bonus
	@if [ -f $(BNAME) ]; \
		then clear && /bin/bash -c './$(BNAME) "$$@"' __ "5555" "127.0.0.1" "pass"; \
	fi

$(NAME): $(objs)
	c++ $(flags) $^ -o $@

$(BNAME): $(bobjs)
	c++ $(flags) $^ -o $@

objs/%.o:	srcs/%.cpp
	mkdir -p objs/
	c++ -c $(flags) $< -o $@

objs/%.o:	srcs/bonus/%.cpp
	mkdir -p objs/
	c++ -c $(flags) $< -o $@

objs/%.o:	srcs/server/%.cpp
	mkdir -p objs/
	c++ -c $(flags) $< -o $@

objs/%.o:	srcs/commands/%.cpp
	mkdir -p objs/
	c++ -c $(flags) $< -o $@

clean:
	rm -rf objs
	
fclean: clean
	rm -f $(NAME) $(BNAME)

re: fclean all

.PHONY: clean

-include $(objs:.o=.d)