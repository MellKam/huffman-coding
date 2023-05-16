BUILD_DIR = build

init:
	if [ ! -d $(BUILD_DIR) ]; then mkdir $(BUILD_DIR); fi;
	cd build && cmake ..

run:
	cd build && make && ./huffman-coding