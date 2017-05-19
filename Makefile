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
	install -m755 $(TARGET) $(PREFIX)/_$(NAME)
	mkdir -p $(PREFIX_SHARE)/$(NAME)
	install -m644 -t $(PREFIX_SHARE)/$(NAME)/ $(WRAPPERS)
	touch $(CONFIG)

uninstall: $(TARGET)
	rm -rf $(PREFIX)/_$(NAME)
	rm -rf $(PREFIX_SHARE)/$(NAME)


# misc

run:
	$(TARGET)

clean:
	rm -rf $(ODIR)/*
