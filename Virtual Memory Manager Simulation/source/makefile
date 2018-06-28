all: assign3part1.c assign3part2.c pagetable.c phypages.c
	gcc -g -o assign3part1 assign3part1.c
	gcc -g -o assign3part2 assign3part2.c pagetable.c phypages.c
	gcc -g -o assign3part3 assign3part3.c pagetableEX.c phypagesEX.c

part1: assign3part1.c
	gcc -g -o assign3part1 assign3part1.c

part2: assign3part2.c pagetable.c phypages.c
	gcc -g -o assign3part2 assign3part2.c pagetable.c phypages.c

part3: assign3part3.c pagetableEX.c phypagesEX.c
	gcc -g -o assign3part3 assign3part3.c pagetableEX.c phypagesEX.c

clean:
	rm -f assign3part? *.o *.txt
	rm -f output-part??

testpart1:
	assign3part1 part1testsequence

testpart2:
	assign3part2 part1testsequence

testpart3:
	assign3part3 128 4096 1024 part2sequence

runpart1:
	assign3part1 part1sequence

runpart2:
	assign3part2 part2sequence

runpart3:
	assign3part3 256 4096 1024 part2sequence
	cp output-part3 output-part3a
	assign3part3 128 4096 2048 part2sequence	
	cp output-part3 output-part3b
	rm output-part3