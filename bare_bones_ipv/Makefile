COMPILER = g++
C_ARG = -std=c++17 -frtti -o

CURRENT_DIR = $(shell pwd)
SRC_DIR = $(CURRENT_DIR)/src
SRC_HEADER_DIR = $(SRC_DIR)
SRC_FILES =  $(SRC_DIR)/*/*.cpp
TEST_AST_PARSER = $(CURRENT_DIR)/tests/ast_parser.cpp
TARGET_DIR = $(CURRENT_DIR)/bin
#
ast_parser:$(TEST_AST_PARSER) $(SRC_FILES) 
	$(COMPILER) $(C_ARG) $(TARGET_DIR)/ast_parser $(TEST_AST_PARSER) $(SRC_FILES) -I$(SRC_HEADER_DIR)

clean:
	rm $(TARGET_DIR)/ast_parser
	