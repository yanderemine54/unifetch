# Copyright (c) 2022 Yanderemine54
# 
# This software is released under the MIT License.
# https://opensource.org/licenses/MIT

PREFIX=/usr/local
CC=clang
CFLAGS=-Wall
LDFLAGS=-lcpuid

all: unifetch

unifetch: src/main.c
	$(CC) $(CFLAGS) -o unifetch src/main.c $(LDFLAGS)

install: unifetch
	mkdir -p $(PREFIX)/bin
	cp unifetch $(PREFIX)/bin/unifetch
	chmod +x $(PREFIX)/bin/unifetch