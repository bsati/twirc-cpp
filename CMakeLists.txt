cmake_minimum_required (VERSION 3.8)

project ("twirc-cpp")

add_definitions("-std=c++20")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

add_executable (main "src/main.cpp")

target_link_libraries(main ${CONAN_LIBS})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET main PROPERTY CXX_STANDARD 20)
endif()