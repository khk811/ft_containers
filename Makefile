NAME = ft_containers
CPP = c++
RM = rm -rf

SRCS = main.cpp

OBJ_DIR = objs/
OBJS = $(addprefix $(OBJ_DIR), $(SRCS:.cpp=.o))

CPPFLAGS := -Wall -Wextra -Werror -std=c++98 #-pedantic

all : $(NAME)

clean:
	$(RM) $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean
	make all

$(OBJ_DIR) :
	mkdir $(OBJ_DIR)

$(OBJS) : | $(OBJ_DIR)

$(addprefix $(OBJ_DIR), %.o) : %.cpp
	$(CPP) $(CPPFLAGS) -c $< -o $@

$(NAME) : $(OBJS)
	$(CPP) $(CPPFLAGS) -o $@ $^

.PHONY : all clean fclean re
