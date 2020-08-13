cmake_minimum_required(VERSION 3.10)

# Set the project name
project(WeatherIn)

set(CMAKE_CXX_FLAGS "-O3 -Wall -Wextra")

# Define files to be compiled
file(GLOB_RECURSE SourceFiles src/*)

add_executable(weatherin ${SourceFiles})
target_link_libraries(weatherin -lncurses -lcurl)
