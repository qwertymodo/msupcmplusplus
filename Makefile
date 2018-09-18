.PHONY: all

all: libsox msupcm++/msupcm

FORCE:

clean: FORCE
	$(MAKE) -C 3rdparty/sox -s clean
	$(MAKE) -C msupcm++ clean

3rdparty/sox/configure: 3rdparty/sox/Makefile.am
	autoreconf -i 3rdparty/sox

3rdparty/sox/Makefile: 3rdparty/sox/configure
	cd 3rdparty/sox && ./configure --without-png --without-gsm --without-lpc10 --without-id3tag --without-wavpack --disable-shared --enable-static

3rdparty/sox/soxconfig.h: 3rdparty/sox/Makefile
	$(MAKE) -C 3rdparty/sox -s

3rdparty/sox/src/.libs/libsox.a: 3rdparty/sox/Makefile
	$(MAKE) -C 3rdparty/sox -s

libsox: 3rdparty/sox/src/.libs/libsox.a 3rdparty/sox/soxconfig.h

msupcm++/msupcm: libsox msupcm++/msupcm.cpp
	$(MAKE) -C msupcm++ -s
