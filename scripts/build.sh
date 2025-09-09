#!/bin/bash

echo "Testing dependancies..."
go_exists=0
cmake_exists=0

go version > /dev/null 2>&1
if [ $? -ne 0 ]; then
	echo -e "\x1b[31m! Golang is missing: add it to PATH or download from go.dev/dl\x1b[0m"
else
	go_exists=1
	echo "Golang exists"
fi
cmake -version > /dev/null 2>&1
if [ $? -ne 0 ]; then
	echo -e "\x1b[31m! CMake is missing: add it to PATH or download from cmake.org/download\x1b[0m"
else
	cmake_exists=1
	echo "CMake exists"
fi

if [ $go_exists -ne 0 ] && [ $cmake_exists -ne 0 ]; then
	mkdir build
	cmake -B build
	cmake --build build
	if [ $? -ne 0 ]; then
		echo -e "\x1b[31m! CMake Error: Make sure you have a C++ compiler\x1b[0m"
		exit 1
	fi

	go build -o bin/komodo src/cmd/main.go
	echo "Built target komodo"
else
	echo "Build failed"
fi