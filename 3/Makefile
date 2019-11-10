CC=g++
FLAGS = --std=c++17 -Wall -Wextra
LDFLAGS = -shared

test: build_test
	LD_LIBRARY_PATH=./ ./test

build_test: test.o parser_lib
	$(CC) $(FLAGS) test.o -L ./ -lparser_lib -o test

parser_lib: Parser.o
	$(CC) ${FLAGS}  ${LDFLAGS} -fPIC Parser.o -o libparser_lib.so

Parser.o: parser_lib/Parser.cpp
	$(CC) ${FLAGS} -c -fPIC parser_lib/Parser.cpp

test.o: test.cpp
	$(CC) ${FLAGS} -c test.cpp