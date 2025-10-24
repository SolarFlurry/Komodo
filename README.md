# Komodo

Reasons I made Komodo:
- learning compilers
- learning Golang
- to make command stuff

## Installing
WIP

## Building from source
### Dependencies:
- Golang
- CMake
- C++ Compiler (C++14 or later)

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

## TODO:
- Constant variables
- Tick functions
- Float and boolean runtime type support
- Comparison and logic operators
- Arithmetic constant optimisations
- Selector type support
- more but i cant think right now

## some stuff
```diff
+ good 
- annoying to type `/"..."` for every command raw
```