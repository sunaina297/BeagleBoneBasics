all: 
	arm-linux-gnueabihf-gcc -Wall -g -D _POSIX_C_SOURCE=200809L -Werror hello.c -o hello 
	cp hello $(HOME)/cmpt433/public/myApps
