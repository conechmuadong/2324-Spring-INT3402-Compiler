INCLUDES = -I ./include
CC = g++ -g -Wall
SOURCE = ./src
OBJECT = ./obj

all: $(OBJECT)/scanner.o $(OBJECT)/parser.o $(OBJECT)/main.o
	$(CC) $(OBJECT)/scanner.o $(OBJECT)/parser.o $(OBJECT)/main.o -o ./upl

$(OBJECT)/scanner.o: $(SOURCE)/scanner.cpp
	$(CC) $(INCLUDES) -c $(SOURCE)/scanner.cpp -o $(OBJECT)/scanner.o

$(OBJECT)/parser.o: $(SOURCE)/parser.cpp
	$(CC) $(INCLUDES) -c $(SOURCE)/parser.cpp -o $(OBJECT)/parser.o

$(OBJECT)/main.o: $(SOURCE)/main.cpp
	$(CC) $(INCLUDES) -c $(SOURCE)/main.cpp -o $(OBJECT)/main.o

phony: clean
clean:
	rm -f $(OBJECT)/*.o
	rm -f ./out/*
	rm -f ./upl
