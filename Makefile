#* TARGET *#
TARGET = webserv

#*SOURCE FILES *#
FILES = main.cpp
		
SRC = $(addprefix src/, $(FILES))

#* OBJECT DIRECTORY *#
OBJ_DIR = obj

#* OBJECT FILES *#
OBJS = $(SRC:src/%.cpp=$(OBJ_DIR)/%.o)

#* FLAGS *#
FLAGS = -Wall -Wextra -Werror -std=c++98

#* HEADER FILES *#
SDL_INCLUDE = include/

#* DELETE *#
DEL = rm -rf

all: $(TARGET)

$(OBJ_DIR)/%.o: srcs/%.cpp
	g++ $(FLAGS) -c -I $(INCLUDE) $< -o $@

$(TARGET): $(OBJS) $(INCLUDE)
	g++ $(FLAGS) -I $(INCLUDE) -o $(TARGET)

re: fclean all

clean:
	$(DEL) $(OBJS)

fclean:	clean
	$(DEL) $(TARGET)

.PHONY: clean fclean re all