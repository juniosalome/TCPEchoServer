############################# Makefile ##########################
CC = g++
CFLAGS = -Wall -g
DEPS = client.h 
OBJ = main.o client.o
EXEC = client
%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	
$(EXEC):$(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)
	
clean: 
	rm -rf *.o $(EXEC)