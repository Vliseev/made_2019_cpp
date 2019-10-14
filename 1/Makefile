CC=g++

run: calc
	@./calc

test: build_test
	@./test

calc: Interpreter.o Lexer.o main.o
	$(CC) -o calc Interpreter.o Lexer.o main.o

build_test: Interpreter.o Lexer.o test.o
	$(CC) -o test Interpreter.o Lexer.o test.o

Interpreter.o:
	$(CC) -c Interpreter.cpp

Lexer.o:
	$(CC) -c Lexer.cpp

main.o:
	$(CC) -c main.cpp

test.o:
	$(CC) -c test.cpp