COMPILER = g++
C_ARG = -std=c++17  -o

CURRENT_DIR = $(shell pwd)
SRC_DIR = $(CURRENT_DIR)/src
SRC_HEADER_DIR = $(SRC_DIR)
SRC_FILES =  $(SRC_DIR)/*/*.cpp
TEST_CSK = $(CURRENT_DIR)/tests/test-valid.cpp
TARGET_DIR = $(CURRENT_DIR)/bin
#
test-valid:$(TEST_CSK) $(SRC_FILES) 
	$(COMPILER) $(C_ARG) $(TARGET_DIR)/test-valid $(TEST_CSK) $(SRC_FILES) -I$(SRC_HEADER_DIR)

clean:
	rm $(TARGET_DIR)/test-valid
	