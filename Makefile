all: compile

minheap:
	gcc -c libs/minheap/minheap.c -o libs/minheap/minheap.o

bintree:
	gcc -c libs/bintree/bintree.c -o libs/bintree/bintree.o

compile: minheap bintree
	gcc -Wall -Werror libs/bintree/bintree.o libs/minheap/minheap.o main.c -o fileCompressor

clean:
	rm -rf fileCompressor; rm -rf *.hcz; rm libs/minheap/minheap.o; rm libs/bintree/bintree.o