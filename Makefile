all: compile

minheap:
	gcc -c libs/minheap/minheap.c -o libs/minheap/minheap.o

bintree:
	gcc -c libs/bintree/bintree.c -o libs/bintree/bintree.o

compile: minheap bintree
	gcc libs/bintree/bintree.o libs/minheap/minheap.o main.c -o fileCompressor

clean:
	rm -rf fileCompressor; rm -rf *.hcz; rm libs/minheap/minheap.o; rm libs/bintree/bintree.o

test_buildcodebook: compile
	./fileCompressor -b aaaa_testfolder/rTest1

test_buildcodebook_recursive: compile 
	./fileCompressor -R -b aaaa_testfolder/

test_compress: compile test_buildcodebook
	./fileCompressor -c aaaa_testfolder/rTest1 ./HuffmanCodebook

test_compress_recursive: compile test_buildcodebook_recursive 
	./fileCompressor -R -c aaaa_testfolder/ ./HuffmanCodebook

test_decompress: compile test_compress
	./fileCompressor -d aaaa_testfolder/rTest1.hcz ./HuffmanCodebook

test_decompress_recursive: compile
	./fileCompressor -R -d ./aaaa_testfolder ./HuffmanCodebook

run: compile
	./fileCompressor -R -b testFolder/