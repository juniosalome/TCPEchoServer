############################# Makefile ##########################
CC = g++
CFLAGS = -Wall -g
DEPS = server.h 
OBJ = main.o server.o
EXEC = server
%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(EXEC):$(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
	
clean: 
	rm -rf *.o $(EXEC)