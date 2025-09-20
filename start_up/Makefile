COMPILER = g++
C_ARG = -std=c++17 -frtti -o

CURRENT_DIR = $(shell pwd)
SRC_DIR = $(CURRENT_DIR)/src
SRC_HEADER_DIR = $(SRC_DIR)
SRC_FILES =  $(SRC_DIR)/*.cpp
TARGET_DIR = $(CURRENT_DIR)/bin

Count:$(SRC_FILES)
	$(COMPILER) $(C_ARG) $(TARGET_DIR)/Count $(SRC_FILES)  -I$(SRC_HEADER_DIR)

clean:
	rm $(TARGET_DIR)/Count
	