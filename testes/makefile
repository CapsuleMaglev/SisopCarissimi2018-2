#
# Makefile para testes
# 

CC=gcc
LIB_DIR=../lib

all: testes main

testes: testes.c $(LIB_DIR)/libt2fs.a $(LIB_DIR)/bitmap2.o $(LIB_DIR)/apidisk.o
	$(CC) -o testes testes.c $(LIB_DIR)/libt2fs.a -Wall

main: main.c $(LIB_DIR)/libt2fs.a $(LIB_DIR)/bitmap2.o $(LIB_DIR)/apidisk.o
	$(CC) -o main main.c $(LIB_DIR)/libt2fs.a -Wall
clean:
	rm -rf  testes main *.o *~


