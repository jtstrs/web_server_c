all:
	gcc -o sample_web_server src/*.c src/core/*.c src/containers/*.c

debug:
	gcc -g -o sample_web_server src/*.c src/core/*.c src/containers/*.c
