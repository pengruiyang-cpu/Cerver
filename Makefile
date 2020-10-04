cerver: daemon.c core.c accept.c settings.h cerver.h
	gcc daemon.c core.c accept.c -o cerver -lpthread


debug: daemon.c core.c accept.c settings.h cerver.h
	gcc daemon.c core.c accept.c -o cerver -lpthread -g
	gdb ./cerver
