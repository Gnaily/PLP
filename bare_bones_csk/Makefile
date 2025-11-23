COMPILER = g++
C_ARG = -std=c++17  -o

CURRENT_DIR = $(shell pwd)
SRC_DIR = $(CURRENT_DIR)/src
SRC_HEADER_DIR = $(SRC_DIR)
SRC_FILES =  $(SRC_DIR)/*/*.cpp
TEST_CSK = $(CURRENT_DIR)/tests/test-csk.cpp
TARGET_DIR = $(CURRENT_DIR)/bin
#
test-csk:$(TEST_CSK) $(SRC_FILES) 
	$(COMPILER) $(C_ARG) $(TARGET_DIR)/test-csk $(TEST_CSK) $(SRC_FILES) -I$(SRC_HEADER_DIR)

clean:
	rm $(TARGET_DIR)/test-csk
	