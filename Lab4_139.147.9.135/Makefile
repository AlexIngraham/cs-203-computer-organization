
test_stack: stack.c test_stack.c
	gcc -c stack.c
	gcc -c test_stack.c
	gcc -o test_stack test_stack.o stack.o

test_list:
	gcc -c list.c
	gcc -c stack.c
	gcc -c test_list.c
	gcc -o test_list test_list.o stack.o list.o

lab4:
	gcc -c main.c stack.c list.c pda.c
	gcc -o lab4 main.o stack.o list.o pda.o

clean:
	rm -f lab4 test_stack test_list
	rm -f *~
	rm -f *.o

tar: clean
	cd ..;tar cvfz lab4_yourid.tgz lab4_yourid
