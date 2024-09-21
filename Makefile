# Variables
CC = gcc
CFLAGS = -Wall
LIBS = -lglfw -lGLEW -lGL
SRC = src/main.c
TARGET = opengl_app

# Default rule
all: $(TARGET)

# Linking and building the executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

# Clean rule to remove the executable
clean:
	rm -f $(TARGET)

# Phony targets
.PHONY: all clean
