CC ?= gcc

CFLAGS ?= -W -pedantic -Werror -Wall -std=gnu99 \
-fno-strict-aliasing -fno-common -Wno-unused-parameter \
-fstrict-aliasing -fstrict-overflow -Wno-return-local-addr

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
