all: assign4part1.c assign4part2.c assign4part3.c assign4part4.c
	gcc -g -o assign4part1 assign4part1.c -pthread
	gcc -g -o assign4part2 assign4part2.c -pthread
	gcc -g -o assign4part3 assign4part3.c -pthread
	gcc -g -o assign4part4 assign4part4.c -pthread

part1: assign4part1.c
	gcc -g -o assign4part1 assign4part1.c -pthread

part2: assign4part2.c
	gcc -g -o assign4part2 assign4part2.c -pthread

part3: assign4part3.c
	gcc -g -o assign4part3 assign4part3.c -pthread

part4: assign4part4.c
	gcc -g -o assign4part4 assign4part4.c -pthread

clean:
	rm -f assign4part? *.o

testpart1:
	assign4part1 5
	assign4part1 50

runpart1:
	assign4part1 50

testpart2:
	assign4part2 5
	assign4part2 50

runpart2:
	assign4part2 50

runpart3:
	assign4part3 50

runpart4:
	assign4part4 50

