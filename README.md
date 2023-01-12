# twirc-cpp

Twitch IRC client implementation in C++ that can be used as a template for building bots.

## Features

This implementation only supports basic IRC connectivity. 
* Message tags are not parsed / supported.
* SSL is only implemented experimentally and not tested. (add `#DEFINE SSL_SUPPORT` in `client.cpp`)
* Rate limiting has not been implemented. If you intend to use this project as a template for building a real bot that sends messages I urge you to implement rate limiting.

## Building

### Conan

This project uses Conan for dependency management. You can install Conan [here](https://docs.conan.io/en/latest/installation.html).
To install dependencies and generate a working CMake build, first run the following commands from the repositories root directory.
```bash
mkdir build
cd build
conan install .. -s build_type=<Debug/Release>
```

### CMake

Build project files / the project by using CMake with your desired build setup.

#### Windows Example (Visual Studio 2022)

Install the CMake tools for Visual Studio and open the project folder.
