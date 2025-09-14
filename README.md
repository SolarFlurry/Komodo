# Komodo

## Installing
WIP

## Building from source
### Dependencies:
- Golang
- CMake
- C++ Compiler (C++23 or later)

Command Line - Windows
```sh
git clone https://github.com/SolarFlurry/Komodo.git
go version
cmake -version
scripts\build.cmd
```

Command Line - Unix-like, Linux
```sh
git clone https://github.com/SolarFlurry/Komodo.git
go version
cmake -version
bash scripts/build.sh
```

After building, you must set the `KOMODO_ENV` environment variable to the path of `kmdlib` inside the project directory.