.PHONY: build clean run

build:
	mkdir -p build && cd build && cmake .. && make -j$(nproc)

clean:
	rm -rf build

run:
	./build/realsense_node