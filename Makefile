all: myshell.c
	gcc -o main myshell.c
	./shell
	
clean:
	$(RM) shell
