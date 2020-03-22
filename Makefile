all: compile

compile:
	gcc -Wall -Werror main.c -o fileCompressor

clean:
	rm -rf fileCompressor; rm -rf *.hcz