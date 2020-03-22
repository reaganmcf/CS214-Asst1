all: compile

compile:
	gcc -Wall -Werror libs/bintree/bintree.c libs/minheap/minheap.c main.c -o fileCompressor

clean:
	rm -rf fileCompressor; rm -rf *.hcz