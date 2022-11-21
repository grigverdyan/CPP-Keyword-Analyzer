.PHONY: all re clean fclean debug gcov lcov clean_gcda clean_gcno fclean_glcov
EXECUTABLE	=	analyzer.exe
SRCS		=	sources
SOURCES		=	$(wildcard $(SRCS)/*.cpp)
INCLUDES	=	-Iincludes
OBJECTS		=   $(patsubst $(SRCS)/%.cpp, $(OBJS_DIR)/%.o, $(SOURCES))
OBJS_DIR	=	objs
CC			=	g++
CXXFLAGS	=	-Wall -Wextra -Werror -g -ggdb3
MKDIR		= 	mkdir -p
STANDARD	=	-std=c++17
RM			=	rm -rf
LCOVTEST	=	$(EXECUTABLE:.exe=.info)
BROWSER		=	google-chrome

all : $(EXECUTABLE)

$(EXECUTABLE) : $(OBJECTS)
	@echo "Creating $(EXECUTABLE)..."
	@$(CC) $(OBJECTS) $(INCLUDES) -o $@

$(OBJS_DIR)/%.o : $(SRCS)/%.cpp | $(OBJS_DIR)
	@$(CC) $(STANDARD) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

$(OBJS_DIR) :
	@$(MKDIR) $(OBJS_DIR)

re : fclean all

clean :
	@$(RM) $(OBJS_DIR)

fclean : clean
	@$(RM) $(EXECUTABLE)
	@echo "Cleaned all"

debug :
	gdb $(EXECUTABLE) input.cpp core

gcov :
	$(CC) $(SOURCES) --coverage -o $(EXECUTABLE)
	./$(EXECUTABLE) input.cpp

lcov :
	lcov -t "program" -o $(LCOVTEST) -c -d .
	genhtml -o report $(LCOVTEST)
	$(RM) *.gcda
	$(RM) *.gcno
	$(RM) $(LCOVTEST)
	$(RM) $(EXECUTABLE)
	$(BROWSER) report/index.html

clean_gcda :
	$(RM) *.gcda

clean_gcno :
	$(RM) *.gcno

fclean_glcov : clean_gcda clean_gcno
	$(RM) $(LCOVTEST)
	$(RM) -fr report
	$(RM) $(EXECUTABLE)

