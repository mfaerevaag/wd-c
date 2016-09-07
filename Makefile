include config.mk
export

.PHONY: all build run clean debug install uninstall \
	build-test test test-config

all: clean build


# src

$(ODIR):
	mkdir -p $(ODIR)

build: $(ODIR)
build:
	cd $(SDIR) && $(MAKE) -f Makefile.src

debug: DEBUG = 1
debug: build


# test

build-test: build
	cd $(TDIR) && $(MAKE) -f Makefile.test

test-config:
	cp $(TCONFIG) $(ODIR)/

test: build-test test-config
test:
	./$(TTARGET)


# install

install: $(TARGET)
	install -Dm755 $(TARGET) $(prefix)/bin/_$(NAME)
	install -Dm644 -t $(prefix)/share/$(NAME)/ $(WRAPPERS)

uninstall: $(TARGET)
	rm -rf $(prefix)/bin/_$(NAME)
	rm -rf $(prefix)/share/$(NAME)


# misc

run:
	$(TARGET)

clean:
	rm -rf $(ODIR)/*
