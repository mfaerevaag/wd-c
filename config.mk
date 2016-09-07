CC ?= gcc

CFLAGS ?= -W -pedantic -Werror -Wall -std=gnu99 \
-fno-strict-aliasing -fno-common -Wno-unused-parameter \
-fstrict-aliasing -fstrict-overflow -fdiagnostics-color=always \
-Wno-return-local-addr

PREFIX ?= /usr/local
DEBUG  ?= 0

SDIR = src
ODIR = bin
TDIR = test
LDIR = lib
SRC = $(wildcard $(SDIR)/*.c)
OBJ = $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(SRC))

TARGET  ?= $(ODIR)/wd
TTARGET ?= $(ODIR)/wd-test
TLIBS   ?= -lcmocka
TCONFIG ?= $(LDIR)/test_warprc
