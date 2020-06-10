CC = g++
CCFLAGS=-std=c++11
TARGET=snake
OBJECTS=snake.o

all:$(TARGET)

$(TARGET):$(OBJECTS)
				$(CC) $(CCFLAGS) -o $@ $^ -lncursesw -pthread

%.o : %.cpp
	$(CC) $(CCFLAGS) -c -o $@ $^

clean:
	rm -rf *.o
