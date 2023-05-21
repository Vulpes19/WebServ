#* TARGET *#
TARGET = webserv

#*SOURCE FILES *#
FILES = main.cpp
		
SRC = $(addprefix srcs/, $(FILES))

#* OBJECT DIRECTORY *#
OBJ_DIR = obj

#* OBJECT FILES *#
OBJS = $(SRC:srcs/%.cpp=$(OBJ_DIR)/%.o)

#* FLAGS *#
FLAGS = -Wall -Wextra -Werror -std=c++98

#* HEADER FILES *#
INCLUDE = include/

#* DELETE *#
DEL = rm -rf

all: $(TARGET)

$(OBJ_DIR)/%.o: srcs/%.cpp
	g++ $(FLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	g++ $(FLAGS) $(OBJS) -o $(TARGET)

re: fclean all

clean:
	$(DEL) $(OBJS)

fclean:	clean
	$(DEL) $(TARGET)

.PHONY: clean fclean re all