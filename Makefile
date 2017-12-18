all: mbed-os/ BUILD/NUCLEO_F411RE/GCC_ARM/io-oolong.bin

mbed-os/:
	mbed deploy

BUILD/NUCLEO_F411RE/GCC_ARM/io-oolong.bin: mbed-os/ main.cpp
	mbed compile --profile profile.json

.PHONY: clean-build clean-mbed clean clean-all

clean-build:
	rm -rf BUILD

clean: clean-build

clean-mbed:
	rm -rf mbed-os

clean-all: clean-build clean-mbed
