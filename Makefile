include config.mk
export

.PHONY: all build run clean release \
	install uninstall build-test test test-config

all: build

$(ODIR):
	mkdir -p $(ODIR)


# src

build: ARGS += --verbose
build: $(ODIR)
build: clean
	cd $(SDIR) && $(MAKE) -f Makefile.src

release: RELEASE = 1
release: clean build


# test

build-test: build
	cd $(TDIR) && $(MAKE) -f Makefile.test

test-config:
	cp $(TCONFIG) $(ODIR)/

test: build-test test-config
test:
	./$(TTARGET)


# misc

run:
	$(TARGET)

install: $(TARGET)
	install -m 0755 $(TARGET) $(PREFIX)/bin/_$(patsubst $(ODIR)/%,%,$(TARGET))

uninstall: $(TARGET)
	rm -rf $(PREFIX)/bin/$(TARGET)

clean:
	rm -rf $(ODIR)/*
