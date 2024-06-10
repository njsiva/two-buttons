#!/usr/bin/make -f
# Makefile for two-buttons.lv2 #
# ----------------------- #
# Created by falkTX
#

include Makefile.mk

# --------------------------------------------------------------

PREFIX  ?= /usr/local
DESTDIR ?=

# --------------------------------------------------------------
# Default target is to build all plugins

all: build

# --------------------------------------------------------------
# two-buttons build rules

build: two-buttons.lv2/two-buttons$(LIB_EXT) two-buttons.lv2/manifest.ttl

two-buttons.lv2/two-buttons$(LIB_EXT): two-buttons.c
	$(CC) $^ $(BUILD_C_FLAGS) $(LINK_FLAGS) -lm $(SHARED) -o $@

two-buttons.lv2/manifest.ttl: two-buttons.lv2/manifest.ttl.in
	sed -e "s|@LIB_EXT@|$(LIB_EXT)|" $< > $@

# --------------------------------------------------------------

clean:
	rm -f two-buttons.lv2/two-buttons$(LIB_EXT) two-buttons.lv2/manifest.ttl

# --------------------------------------------------------------

install: build
	install -d $(DESTDIR)$(PREFIX)/lib/lv2/two-buttons.lv2

	install -m 644 two-buttons.lv2/*.so  $(DESTDIR)$(PREFIX)/lib/lv2/two-buttons.lv2/
	install -m 644 two-buttons.lv2/*.ttl $(DESTDIR)$(PREFIX)/lib/lv2/two-buttons.lv2/

# --------------------------------------------------------------
