.PHONY: build test proto


format:
	@find src -iname '*.h' -o -iname '*.cc' | xargs clang-format -style=google -i
	@find test -iname '*.h' -o -iname '*.cc' | xargs clang-format -style=google -i

proto:
	@/deps/bin/protoc --cpp_out=src/genproto --proto_path=src/proto src/proto/*.proto
	@/deps/bin/protoc --grpc_out=src/genproto --proto_path=src/proto --plugin=protoc-gen-grpc=/deps/bin/grpc_cpp_plugin src/proto/*.proto

build:
	@cd build && cmake -DCMAKE_BUILD_TYPE=Debug -DSTATIC_CHECK=OFF -DBUILD_TESTING=OFF .. && cmake --build . -j8 && cd ../

build-release:
	@cd build && cmake -DCMAKE_BUILD_TYPE=Release -DSTATIC_CHECK=OFF -DBUILD_TESTING=OFF .. && cmake --build . -j8 && cd ../

build-test:
	@cd build && cmake -DCMAKE_BUILD_TYPE=Debug -DSTATIC_CHECK=OFF -DBUILD_TESTING=ON .. && cmake --build . -j8 && cd ../

rebuild:
	@cd build && cmake --build . && cd ../

lint:
	@find src -iname '*.h' -o -iname '*.cc' | xargs cppcheck --enable=warning,performance,portability --force --language=c++ --std=c++20 --suppress=*:src/genproto/*
	@find test -iname '*.h' -o -iname '*.cc' | xargs cppcheck --enable=warning,performance,portability --force --language=c++ --std=c++20
	@"$(which cpplint)" --recursive --filter=-build/c++17 --exclude=build/* --exclude=src/database/sqlite3* --exclude=src/genproto/* .

check:
	@cd build && cmake -DCMAKE_BUILD_TYPE=Release -DSTATIC_CHECK=ON -DBUILD_TESTING=ON .. && cmake --build . -j8 && cd ../

run: 
	@./build/masquerade

test:
	@cd build && GTEST_COLOR=1 ctest -V && cd ../

# make new-pr branch=<branch_name>
new-pr:
	git checkout -b $(branch)
	git branch --set-upstream-to=origin/main $(branch)

dev-img:
	docker buildx build --push --platform linux/arm64,linux/amd64 --tag whuffman36/msqrd-rpc-env:latest .devcontainer

all:
	@make deep-clean
	@make format
	@make proto
	@make build-release
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