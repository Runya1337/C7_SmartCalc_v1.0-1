CC = gcc
CFLAGS = -Wall -Werror -Wextra -g -std=c11
LINUX_FLAGS=-lsubunit -lrt -lpthread -lm
OBJ = backend/s21_stack.c backend/s21_smartcalc.c
GCOV_FLAGS=-fprofile-arcs -ftest-coverage
TESTFLAGS = -lcheck -coverage -lpthread -pthread  
LDFLAGS := -lcheck -lgcov -fprofile-arcs --coverage
TARGET := SmartCalc
ALL_FILES_TEST = $(shell find ./tests -name "*.c")
SRC= $(shell find ./backend -name "*.c")

OS := $(shell uname -s)

ifeq ($(OS),Linux)
	OPEN_CMD = xdg-open
endif
ifeq ($(OS),Darwin)
	OPEN_CMD = open
endif

TEST_SRC=$(wildcard ./tests//s21_*.c)
LIBS=-lcheck



all: clear install

install: uninstall
	mkdir build
	cd front/calc && qmake && make && make clean && rm Makefile

uninstall:
	rm -rf build

Smart_Calc.a: clean
	$(CC) $(CFLAGS) -c $(OBJ)
	ar -rv Smart_Calc.a *.o
	ranlib Smart_Calc.a

dvi:
	open README.md

dist:
	mkdir SmartCalc_v1.0-1/
	mkdir SmartCalc_v1.0-1/src
	cp -R front backend tests Makefile README.md SmartCalc_v1.0-1/src
	tar cvzf SmartCalc_v1.0-1.tgz SmartCalc_v1.0-1/
	rm -rf SmartCalc_v1.0-1

test:	Smart_Calc.a
ifeq ($(OS), Darwin)
	$(CC) $(CFLAGS) -o test_calc tests/s21_calc_test.c  -L. $(TESTFLAGS)  Smart_Calc.a
else
	$(CC) $(STDFLAGS) tests/s21_calc_test.c ${SRC} -o test_calc $(LIBS) $(LINUX_FLAGS)
endif
	./test_calc


gcov_report: Smart_Calc.a clean_test 
ifeq ($(OS), Darwin)
	$(CC) $(STDFLAGS)  $(GCOV_FLAGS) tests/s21_calc_test.c ${SRC}  -o test_gcov $(LIBS) -lm
else
	$(CC) $(STDFLAGS) $(GCOV_FLAGS) tests/s21_calc_test.c ${SRC} -o test_gcov $(LIBS) $(LINUX_FLAGS)
endif
	./test_gcov
	lcov -t "stest" -o s21_test.info --no-external --filter range -c -d .
	genhtml -o report s21_test.info
	$(OPEN_CMD) ./report/index.html

clean_test:
	rm -rf *.gcda
	rm -rf *.gcno
	rm -rf *.info
	rm -rf *test
	rm -rf report
	rm -rf unit_test

clean:
	rm -rf *.o *.gcno *.a *.gcda *.gcov *.info report test test.d test.dSYM a.out.dSYM dvi
	rm -rf test_calc test_valid

clang_check:
	clang-format -style=google -n backend/*.c 
	clang-format -style=google -n backend/*.h
	clang-format -style=google -n front/calc/*.cc
	clang-format -style=google -n front/calc/*.h

clang_final:
	clang-format -style=google -i backend/*.c 
	clang-format -style=google -i backend/*.h
	clang-format -style=google -i front/calc/*.cc
	clang-format -style=google -i front/calc/*.h