all: assign2.c pslibrary.c
	gcc -g -o assign2 assign2.c pslibrary.c

assign2: assign2.c pslibrary.c
	gcc -g -o assign2 assign2.c pslibrary.c

runall: runall.c pslibrary.c
	gcc -g -o runall runall.c pslibrary.c

clean:
	rm -f assign2 runall *.o *.txt

runtest:
	assign2 3 4 2 7 3 6 5 > res1.txt
	assign2 3 4 9 5 6 3 7 > res2.txt
	assign2 3 8 7 3 6 3 2 > res3.txt
	assign2 3 8 7 3 6 7 2 > res4.txt
	assign2 4 8 7 3 6 1 2 > res5.txt
	assign2 3 3 3 4 2 1 2 > res6.txt
	assign2 3 3 2 3 2 1 2 > res7.txt
	assign2 3 5 2 5 4 1 1 > res8.txt
	cat res?.txt > results.txt
	rm -f res?.txt
