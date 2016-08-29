CC ?= gcc

CFLAGS ?= -W -pedantic -Werror -Wall -std=gnu99 \
-fno-strict-aliasing -fno-common -Wno-unused-parameter \
-fstrict-aliasing -fstrict-overflow -fdiagnostics-color=always \
-Wno-return-local-addr

PREFIX ?= /usr/local

SDIR = src
ODIR = bin
SRC = $(wildcard $(SDIR)/*.c)
OBJ = $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(SRC))

TARGET ?= wd

.PHONY: all build run clean release install uninstall

all: build

$(ODIR):
	mkdir -p $(ODIR)

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(ODIR)/$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

build: $(ODIR)
build: $(ODIR)/$(TARGET)

release: CFLAGS += -O3 -DNDEBUG
release: clean build

run:
	$(ODIR)/$(TARGET)

install: $(ODIR)/$(TARGET)
	install -m 0755 $(ODIR)/$(TARGET) $(PREFIX)/bin/_$(TARGET)

uninstall: $(ODIR)/$(TARGET)
	rm -rf $(PREFIX)/bin/$(TARGET)

clean:
	rm -rf $(ODIR)/*
