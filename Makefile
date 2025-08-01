.PHONY: build


format:
	@clang-format -style=google -i src/*.cc src/services/*.cc src/services/*.h src/test/*.cc

proto:
	@/deps/bin/protoc --cpp_out=src/genproto --proto_path=src/proto src/proto/*.proto
	@/deps/bin/protoc --grpc_out=src/genproto --proto_path=src/proto --plugin=protoc-gen-grpc=/deps/bin/grpc_cpp_plugin src/proto/*.proto

build:
	@cd build && cmake .. && cmake --build . && cd ../

rebuild:
	@cd build && cmake --build . && cd ../

run:
	@./build/masquerade

test:
	@./build/src/test/hello_test

all:
	@make deep-clean
	@make format
	@make proto
	@make build
	@make run

clean:
	@mv build/_deps .
	@rm -rf build
	@mkdir build
	@mv _deps build/
	@rm -rf .cache

deep-clean:
	@rm -rf build
	@mkdir build
	@rm -rf src/genproto/*.h src/genproto/*.cc
	@rm -rf .cache