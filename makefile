all: ex3

ex3:
	gcc -w BoundedBuffer.c main.c News.c UnBoundedBuffer.c -o ex3.out -pthread

rm:
	rm *.out