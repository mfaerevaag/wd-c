CC ?= gcc

CFLAGS ?= -Wall -std=gnu99

prefix ?= /usr

NAME = wd

SDIR = src
ODIR = bin
TDIR = test
LDIR = lib
SRC = $(wildcard $(SDIR)/*.c)
OBJ = $(patsubst $(SDIR)/%.c, $(ODIR)/%.o, $(SRC))

TARGET   ?= $(ODIR)/$(NAME)
WRAPPERS ?= $(wildcard $(LDIR)/$(NAME).*)
CONFIG   ?= ~/.warprc

TTARGET ?= $(ODIR)/$(NAME)-test
TLIBS   ?= -lcmocka
TCONFIG ?= $(LDIR)/test_warprc
