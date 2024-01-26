all:
	gcc -o sample_web_server src/*.c src/core/*.c

debug:
	gcc -g -o sample_web_server src/*.c src/core/*.c
