C++ = g++
CXXFLAGS=-std=c++11 -Wall -Wextra -O2 -D_DEFAULT_SOURCE -g
CXXFLAGS+=`pkg-config --cflags sdl2 SDL2_ttf SDL2_image SDL2_mixer`

INCLUDE_DIR = ./include
SRC_DIR = ./src
OBJS_DIR = ./obj
BIN_DIR = ./bin

LDLIBS=`pkg-config --libs sdl2 SDL2_ttf SDL2_image SDL2_mixer` -lm -lstdc++

$(BIN_DIR)/game: $(OBJS_DIR)/game.o $(OBJS_DIR)/engine.o $(OBJS_DIR)/error_handler.o $(OBJS_DIR)/event_handler.o $(OBJS_DIR)/image.o $(OBJS_DIR)/sprite.o $(OBJS_DIR)/character.o $(OBJS_DIR)/enemy.o $(OBJS_DIR)/collision_detector.o $(OBJS_DIR)/audio.o
	$(C++) $(OBJS_DIR)/engine.o $(OBJS_DIR)/error_handler.o $(OBJS_DIR)/game.o $(OBJS_DIR)/image.o $(OBJS_DIR)/event_handler.o $(OBJS_DIR)/sprite.o $(OBJS_DIR)/character.o $(LDLIBS) $(OBJS_DIR)/enemy.o $(OBJS_DIR)/audio.o $(OBJS_DIR)/collision_detector.o -o $(BIN_DIR)/game

$(OBJS_DIR)/game.o: $(SRC_DIR)/game.cpp
	$(C++) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $(SRC_DIR)/game.cpp -o $(OBJS_DIR)/game.o

$(OBJS_DIR)/engine.o: $(SRC_DIR)/engine.cpp
	$(C++) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $(SRC_DIR)/engine.cpp -o $(OBJS_DIR)/engine.o

$(OBJS_DIR)/error_handler.o: $(SRC_DIR)/error_handler.cpp
	$(C++) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $(SRC_DIR)/error_handler.cpp -o $(OBJS_DIR)/error_handler.o

$(OBJS_DIR)/image.o: $(SRC_DIR)/image.cpp
	$(C++) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $(SRC_DIR)/image.cpp -o $(OBJS_DIR)/image.o

$(OBJS_DIR)/sprite.o: $(SRC_DIR)/sprite.cpp
	$(C++) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $(SRC_DIR)/sprite.cpp -o $(OBJS_DIR)/sprite.o

$(OBJS_DIR)/character.o: $(SRC_DIR)/character.cpp
	$(C++) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $(SRC_DIR)/character.cpp -o $(OBJS_DIR)/character.o

$(OBJS_DIR)/enemy.o: $(SRC_DIR)/enemy.cpp
	$(C++) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $(SRC_DIR)/enemy.cpp -o $(OBJS_DIR)/enemy.o

$(OBJS_DIR)/event_handler.o: $(SRC_DIR)/event_handler.cpp
	$(C++) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $(SRC_DIR)/event_handler.cpp -o $(OBJS_DIR)/event_handler.o

$(OBJS_DIR)/collision_detector.o: $(SRC_DIR)/collision_detector.cpp
	$(C++) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $(SRC_DIR)/collision_detector.cpp -o $(OBJS_DIR)/collision_detector.o

$(OBJS_DIR)/audio.o: $(SRC_DIR)/audio.cpp
	$(C++) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $(SRC_DIR)/audio.cpp -o $(OBJS_DIR)/audio.o

game: $(BIN_DIR)/game

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)/* $(OBJS_DIR)/*
