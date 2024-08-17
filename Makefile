NAME				:=	libclarg.a

PREFIX			:=	/usr/local

LIBDIR			:=	$(PREFIX)/lib

INCLUDEDIR	:=	$(PREFIX)/include

SRCS				:=	clarg.c

EXMPL				:=	example

EXMPL_SRCS	:=	example.c

OBJS				:=	$(SRCS:%.c=%.o)

CFLAGS			:=	-Wall -Wextra -Werror -Wno-unused-function -O2

CC					:=	gcc

$(NAME): $(OBJS)
	ar rcs $(NAME) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

install: $(NAME)
	install -d $(DESTDIR)$(LIBDIR)
	install -m 644 $(NAME) $(DESTDIR)$(LIBDIR)
	install -d $(DESTDIR)$(INCLUDEDIR)
	install -m 644 clarg.h $(DESTDIR)$(INCLUDEDIR)

$(EXMPL): $(NAME)
	$(CC) $(CFLAGS) $(EXMPL_SRCS) -o $(EXMPL) -L. -lclarg 

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)
	rm -f example

re: fclean all

.PHONY:	all clean fclean re
