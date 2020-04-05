all: compile

minheap:
	gcc -c libs/minheap/minheap.c -o libs/minheap/minheap.o

bintree:
	gcc -c libs/bintree/bintree.c -o libs/bintree/bintree.o

compile: minheap bintree
	gcc libs/bintree/bintree.o libs/minheap/minheap.o main.c -o fileCompressor

clean:
	rm -rf fileCompressor; rm -rf *.hcz; rm libs/minheap/minheap.o; rm libs/bintree/bintree.o

test_buildcodebook: clean compile
	./fileCompressor -b testFolder/rTest1

test_buildcodebook_recursive: clean compile 
	./fileCompressor -R -b testFolder/

test_compress: clean compile test_buildcodebook
	./fileCompressor -c ./testFolder/rTest1 ./HuffmanCodebook

run: compile
	./fileCompressor -R -b testFolder/