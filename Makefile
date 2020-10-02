cerver: daemon.c
	gcc daemon.c -o cerver -lpthread

pushit: cerver
	git add .
	git commit -am 'commited'
	git push https://github.com/pengruiyang-cpu/Cerver.git master


