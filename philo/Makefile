NAME = philo

CFLAGS = -g -Wall -Wextra -Werror

INCLUDES = -I./include

SRC = main.c initialization.c philo_threads.c utils.c philo_actions.c \
		free_and_errors.c argument_check.c


DIR_SRC = src
DIR_OBJ = obj

SRC := ${addprefix ${DIR_SRC}/, ${SRC}}

OBJ = ${subst ${DIR_SRC}/, ${DIR_OBJ}/, ${SRC:.c=.o}}

${NAME}: ${OBJ}
	${CC} ${CFLAGS} $^ -o $@

all: ${NAME}

${OBJ}: ${DIR_OBJ}/%.o: ${DIR_SRC}/%.c
	@mkdir -p ${@D}
	${CC} ${CFLAGS} ${INCLUDES} -c $< -o $@

clean:
	rm -f ${OBJ} rm -d ${DIR_OBJ}

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONY: all clean re