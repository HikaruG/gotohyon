
all: | clean configure build run

configure:
	@mkdir -p build 
	@cd build && cmake ..

build:
	@make -s -j4 -C build

clean:
	@rm -rf bin build

run:
	./bin/run

test:
	docker build -t plt-initial -f docker/plt-initial .
	docker build -t plt-build -f docker/plt-build .
	./docker/run_docker_x11.sh plt-build

start-kit:
	@tar -czvf ../plt-start-kit.tar.gz *

.PHONY: configure build clean run test start-kit
