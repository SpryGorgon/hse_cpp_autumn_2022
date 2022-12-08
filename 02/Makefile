CC=g++
FLAGS=-std=c++2a -Wall -pedantic -Werror -Wextra
TEST_LIB=-lgtest -lgtest_main -lpthread

all: parser test_bin

parser: parser.cpp parser.hpp
	$(CC) $(FLAGS) -c parser.cpp

test_bin: parser.o tests.cpp
	$(CC) $(FLAGS) parser.o tests.cpp -o test_bin $(TEST_LIB)

test:
	./test_bin

clean:
	rm -f *.o test_bin
