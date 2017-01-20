NAME=		server

SRC= 		source/server.cc		\
		source/graph/graph.cc		\
		source/parser/parse_csv.cc


OBJ=		$(SRC:.cc=.o)

CXXFLAGS=	-std=c++11

CXX=		g++

RM=		rm -f

%.o	: 	%.c
		$(CXX) $@ -c $<

all	: 	$(NAME)

$(NAME)	: 	$(OBJ)
		$(CXX) $(OBJ) $(CXXFLAGS) -o $(NAME)

clean	:
		$(RM) $(OBJ)

fclean	: 	clean
		$(RM) $(NAME)

re	: 	fclean all
