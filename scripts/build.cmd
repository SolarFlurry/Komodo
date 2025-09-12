@echo off

echo Testing dependancies...
SET go_exists=0
SET cmake_exists=0

go version >NUL 2>&1
IF %ERRORLEVEL% NEQ 0 (
	echo -e "\x1b[31m! Golang is missing: add it to PATH or download from go.dev/dl\x1b[0m"
) ELSE (
	SET go_exists=1
	echo Golang exists
)
cmake -version >NUL 2>&1
IF %ERRORLEVEL% NEQ 0 (
	echo -e "\x1b[31m! CMake is missing: add it to PATH or download from cmake.org/download\x1b[0m"
) ELSE (
	SET cmake_exists=1
	echo CMake exists
)

IF %go_exists% NEQ 0 IF %cmake_exists% NEQ 0 (
	mkdir build
	cmake -B build
	cmake --build build
	if %ERRORLEVEL% NEQ 0 (
		echo -e "\x1b[31m! CMake Error: Make sure you have a C++ compiler\x1b[0m"
		exit 1
    )

	go build -o bin/komodo src/cmd/main.go
	echo Built target komodo
) ELSE (
	echo Build failed
)