.DEFAULT_GOAL := bin
USB ?= /dev/ttyUSB0

dep-arduion-cli:
	curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | BINDIR=/usr/local/bin sh
	arduino-cli config init
	arduino-cli config add board_manager.additional_urls https://raw.githubusercontent.com/sparkfun/Arduino_Apollo3/main/package_sparkfun_apollo3_index.json

deps: dependencies

dependencies:
	arduino-cli core --additional-urls https://raw.githubusercontent.com/sparkfun/Arduino_Apollo3/main/package_sparkfun_apollo3_index.json update-index
	arduino-cli core --additional-urls https://raw.githubusercontent.com/sparkfun/Arduino_Apollo3/main/package_sparkfun_apollo3_index.json install "Sparkfun:apollo3@1.2.1"
	arduino-cli lib update-index
	arduino-cli lib install "SparkFun u-blox GNSS v3"
	arduino-cli lib install "ArduinoJson"

docker-build:
	docker build -t omb-nano .

docker-run:
	cp Makefile src/
	docker run -it --rm --name sr -v $(shell pwd)/src:/tracker omb-nano

bin:
	arduino-cli compile -e --output-dir build --build-cache-path /tmp/build -v -b SparkFun:apollo3:artemis .

flash:
	python ../../../tools/svl/svl.py  -f build/omb-nano.ino.bin $(USB)

com:
	picocom -b 115000 $(USB)
