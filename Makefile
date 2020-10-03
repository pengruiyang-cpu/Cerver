cerver: daemon.c core.c accept.c
	gcc daemon.c core.c accept.c -o cerver -lpthread


debug: daemon.c core.c accept.c
	gcc daemon.c core.c accept.c -o cerver -lpthread -g
	gdb ./cerver
