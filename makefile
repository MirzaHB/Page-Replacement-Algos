CC = gcc
CFLAGS = -Wall -g

# List of object files
OBJS = main.o fifo.o lru.o secondChance.o DLL.o fileReader.o

# Default target
all: pageReplacement

# Rule to build the executable
pageReplacement: $(OBJS)
	$(CC) $(CFLAGS) -o pageReplacement $(OBJS)

# Rules for object files
main.o: main.c fifo.h lru.h secondChance.h DLL.h fileReader.h
	$(CC) $(CFLAGS) -c main.c

fifo.o: fifo.c fifo.h DLL.h
	$(CC) $(CFLAGS) -c fifo.c

lru.o: lru.c lru.h DLL.h
	$(CC) $(CFLAGS) -c lru.c

secondChance.o: secondChance.c secondChance.h DLL.h
	$(CC) $(CFLAGS) -c secondChance.c

DLL.o: DLL.c DLL.h
	$(CC) $(CFLAGS) -c DLL.c

fileReader.o: fileReader.c fileReader.h DLL.h
	$(CC) $(CFLAGS) -c fileReader.c

# remove all obj files and the executable
clean:
	rm -f *.o pageReplacement
