# twirc-cpp

Twitch IRC client implementation in C++ that can be as a template used for building bots.

## Building

### Conan

This project uses Conan for dependency management. You can install Conan [here](https://docs.conan.io/en/latest/installation.html).
To install dependencies and generate a working CMake build, first run the following commands from the repositories root directory.
```bash
mkdir build
cd build
conan install ..
```

### CMake

Build project files / the project by using CMake with your desired build setup.

#### Windows Example (Visual Studio 2022)

Run the following commands from the repositories root directory:

```bash
cmake .. -G "Visual Studio 17"
```
Afterwards you can open the `.sln` file in the build directory and start writing code.
To run the program right click `main` in the projects explorer and select it as the startup project.
