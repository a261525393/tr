objs := $(filter-out tr.o,$(patsubst src/%.c,%.o,$(wildcard src/*.c)))

all:${objs}
	gcc -g src/tr.c ${objs} -lmysqlclient  -o tr

${objs}:%.o:src/%.c
	gcc -c $^ -o $@
clean:
	rm -rf tr *.o

