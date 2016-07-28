CC ?= gcc

CFLAGS ?= -W -pedantic -Werror -Wall -std=gnu99 \
-fno-strict-aliasing -fno-common -Wno-unused-parameter \
-fstrict-aliasing -fstrict-overflow -fdiagnostics-color=always \
-Wno-return-local-addr

SRC = $(wildcard ./*.c)
OBJ = $(patsubst $(SRC)/%.c, ./%.o, $(SRC))

TARGET = wd

.PHONY: all build run clean release

all: build run

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

build: ./$(TARGET)

release: CFLAGS += -O3 -DNDEBUG
release: clean build

run:
	./$(TARGET)

clean:
	rm -rf ./$(TARGET)
