CC=g++
CFLAGS=-std=c++17 -pthread
BIN_DIR=bin
OBJ_DIR=obj
SRC_DIR=src
EXAMPLES_DIR=examples

all: directories mandelbrot e0.bmp e1.bmp e2.bmp e3.bmp e4.bmp

directories:
	mkdir -p $(BIN_DIR) $(OBJ_DIR) $(EXAMPLES_DIR)

mandelbrot: directories $(BIN_DIR)/mandelbrot

$(BIN_DIR)/mandelbrot: $(OBJ_DIR)/mandelbrot.o $(OBJ_DIR)/bmp.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/mandelbrot $(OBJ_DIR)/mandelbrot.o $(OBJ_DIR)/bmp.o

$(OBJ_DIR)/mandelbrot.o: directories $(SRC_DIR)/mandelbrot.cpp $(SRC_DIR)/bmp.h $(SRC_DIR)/pixel.h
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/mandelbrot.o $(SRC_DIR)/mandelbrot.cpp

$(OBJ_DIR)/bmp.o: directories $(SRC_DIR)/bmp.cpp $(SRC_DIR)/bmp.h $(SRC_DIR)/pixel.h
	$(CC) $(CFLAGS) -c -o $(OBJ_DIR)/bmp.o $(SRC_DIR)/bmp.cpp

e0.bmp: directories
	./$(BIN_DIR)/mandelbrot -2 1 -1.5 1.5 $(EXAMPLES_DIR)/e0.bmp

e1.bmp: directories
	./$(BIN_DIR)/mandelbrot -0.5 0 0.3 1.2 $(EXAMPLES_DIR)/e1.bmp

e2.bmp: directories
	./$(BIN_DIR)/mandelbrot 0.3 0.4 0.6 0.7 $(EXAMPLES_DIR)/e2.bmp

e3.bmp: directories
	./$(BIN_DIR)/mandelbrot -0.2 0.0 -1.0 -0.9 $(EXAMPLES_DIR)/e3.bmp

e4.bmp: directories
	./$(BIN_DIR)/mandelbrot -0.05 -0.01 -1.01 -0.97 $(EXAMPLES_DIR)/e4.bmp

clean:
	rm -f $(BIN_DIR)/mandelbrot
	rm -f $(OBJ_DIR)/mandelbrot.o
	rm -f $(OBJ_DIR)/bmp.o
	rm -f $(EXAMPLES_DIR)/*.bmp
	rm -r -f html
	rm -r -f latex
