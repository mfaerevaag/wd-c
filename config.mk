CC ?= gcc
CFLAGS ?= -Wall -std=gnu99
UNAME := $(shell uname)

# set prefix according to OS
ifeq ($(UNAME),Darwin) # Mac OS X
PREFIX ?= /usr/local/bin
PREFIX_SHARE ?= /usr/local/share
PREFIX_BASH_COMP ?= /usr/local/etc/bash_completion.d
else # Linux
PREFIX ?= /usr/bin
PREFIX_SHARE ?= /usr/share
PREFIX_BASH_COMP ?= /usr/share/bash-completion/completions
endif

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
