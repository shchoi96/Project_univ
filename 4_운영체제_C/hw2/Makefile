a.out : main.o Thread.o Init.o Scheduler.o TestCase1.o TestCase2.o TestCase3.o TestCase4.o
	gcc -o a.out main.o Thread.o Init.o Scheduler.o TestCase1.o TestCase2.o TestCase3.o TestCase4.o
main.o : main.c
	gcc -c -o main.o main.c
Thread.o : Thread.c
	gcc -c -o Thread.o Thread.c
Init.o : Init.c
	gcc -c -o Init.o Init.c
Scheduler.o : Scheduler.c
	gcc -c -o Scheduler.o Scheduler.c
TestCase1.o : TestCase1.c
	gcc -c -o TestCase1.o TestCase1.c
TestCase2.o : TestCase2.c
	gcc -c -o TestCase2.o TestCase2.c
TestCase3.o : TestCase3.c
	gcc -c -o TestCase3.o TestCase3.c
TestCase4.o : TestCase4.c
	gcc -c -o TestCase4.o TestCase4.c
clean :
	rm *.o a.out
