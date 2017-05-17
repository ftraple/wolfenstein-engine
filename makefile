
CC = gcc
CFLAGS = -std=gnu99 -Wall -Wextra -shared -fPIC
LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lm
INCLUDE_DIR = include/
BINARY_DIR = bin/
BUILD_DIR = build/
SOURCE_DIR = source/

TARGET = Engine3D
SOURCE = $(wildcard source/*.c)
OBJECT = $(patsubst %,$(BUILD_DIR)%, $(notdir $(SOURCE:.c=.o)))

all: $(TARGET)

$(TARGET): directory $(OBJECT)
	@echo "Linking..."
	@echo "--------------------------------------------------------------------------------"
	$(CC) $(SOURCE) -I $(INCLUDE_DIR) $(CFLAGS) $(LDFLAGS) -o $(BINARY_DIR)lib$(TARGET).so
	@echo "--------------------------------------------------------------------------------"
	@echo "Finished!\n"

$(BUILD_DIR)%.o: $(SOURCE_DIR)%.c
	@echo "Compiling..."
	@echo "--------------------------------------------------------------------------------"
	$(CC) -I $(INCLUDE_DIR) -c $< -o $@

directory: 
	mkdir -p $(BINARY_DIR) $(BUILD_DIR)


TEST_CFLAGS = -std=gnu99 -Wall -Wextra -shared -fPIC
TEST_LDFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lm -l$(TARGET)
TEST_DIR = test/
TEST_BUILD_DIR = build/test/

test: $(TARGET)
	mkdir -p $(TEST_BUILD_DIR)
	$(CC) -I $(INCLUDE_DIR) -c $(TEST_DIR)Main.c -o $(TEST_BUILD_DIR)Main.o
	$(CC) $(TEST_BUILD_DIR)Main.o -L$(BINARY_DIR) $(TEST_LDFLAGS) -o $(BINARY_DIR)Test 

install: $(TARGET)
	@echo "Installing..."
	@echo "--------------------------------------------------------------------------------"
	sudo cp bin/lib$(TARGET).so /usr/lib/.

run: test install
	@echo "Running..."
	@echo "--------------------------------------------------------------------------------"
	./bin/Test


help: 
	@echo "target: $(TARGET)"
	@echo "source: $(SOURCE)"
	@echo "object: $(OBJECT)"

clean:
	rm -rf $(BINARY_DIR) $(BUILD_DIR)
