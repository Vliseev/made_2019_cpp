CC=g++
FLAGS=-O2 -fsanitize=address -fsanitize=undefined

test: build_test
	@./test

build_test: test.o linear_allocator.o
	$(CC) $(FLAGS) -o test linear_allocator.o test.o

linear_allocator.o: linear_allocator.cpp
	$(CC) $(FLAGS) -c linear_allocator.cpp

test.o:
	$(CC) $(FLAGS) -c test.cpp