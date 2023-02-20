PROG_NAME  = a.out
CC         = clang++
CFLAGS     = -std=c++20 -Wall -g -Werror -O3
LFLAGS     = -std=c++20 -Wall -g -Werror -O3
C_ASAN     = -fsanitize=address -fno-omit-frame-pointer
L_ASAN     = -fsanitize=address

SOURCE_DIR = src
OBJECT_DIR = obj

source    := $(wildcard $(SOURCE_DIR)/*.cpp)
object    := $(wildcard $(OBJECT_DIR)/*.o)

default: build

build: $(source) $(object)
	make compile
	make link

compile: $(source)
	$(CC) $(CFLAGS) $(C_ASAN) -c $(source)
	mv *.o $(OBJECT_DIR)

link: $(object)
	$(CC) $(LFLAGS) $(L_ASAN) $(object) -o $(PROG_NAME)

clean: $(object) $(PROG_NAME)
	rm $(object) $(PROG_NAME)

