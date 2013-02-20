TARGET      = kp1
SOURCEFILES = main.cpp
CC          = g++
CFLAGS      = -std=c++11 -c
RFLAGS      = -O3
DFLAGS      = -g -O0
INCLUDES    = -I/usr/local/include -L/usr/local/bin
LIBS        = -lginac -lcln -lgmp

.PHONY: all
.PHONY: debug
.PHONY: release
.PHONY: run

all: release

release:
	$(CC) $(CFLAGS) $(RFLAGS) $(INCLUDES) $(SOURCEFILES)
	$(CC) -o $(TARGET) $(SOURCEFILES:.cpp=.o) $(LIBS) &> compile_result.txt

debug:
	$(CC) $(CFLAGS) $(DFLAGS) $(INCLUDES) $(SOURCEFILES)
	$(CC) $(SOURCEFILES:.cpp=.o) $(LIBS) &> compile_result.txt

run: release
	./$(TARGET)

drun: debug
	gdb ./a
