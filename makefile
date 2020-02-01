objs = help.o db.o task.o

all:${objs}
	gcc -g src/tr.c ${objs} -lmysqlclient  -o tr

${objs}:%.o:src/%.c
	gcc -c $^ -o $@
clean:
	rm -rf tr *.o

