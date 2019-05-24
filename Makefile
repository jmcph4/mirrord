PROJ_NAME=mirrord

SRC_DIR=src
BIN_DIR=bin
LIB_DIR=lib
INC_DIR=include
DOCS_DIR=docs

CC=gcc
CFLAGS=-Wall -Wextra -Wshadow -pedantic -std=c99 -g3

$(BIN_DIR)/$(PROJ_NAME): $(SRC_DIR)/*.c
	$(CC) $? $(CFLAGS) -o $@
	cp $@ .

.PHONY: clean
clean:
	rm -rf $(BIN_DIR)/*
	rm $(PROJ_NAME)

