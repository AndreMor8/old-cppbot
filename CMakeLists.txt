cmake_minimum_required(VERSION 3.0.0)
project(cppbot VERSION 0.1.0)

include_directories(
# My includes
./include
# CPU-Stat
./lib/cpu-stat/src
)

add_executable(cppbot
./src/main.cpp
./src/andremor.cpp
./src/util.cpp

# CPU-Stat
./lib/cpu-stat/src/CPUData.cpp
./lib/cpu-stat/src/CPUSnapshot.cpp
)

set_target_properties(cppbot PROPERTIES
RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
)

find_package(PkgConfig)
find_package(Aegis REQUIRED)
add_subdirectory(./lib/cpp-dotenv)
target_link_libraries(cppbot PRIVATE ${Aegis_LIBRARIES} cpp_dotenv)
target_include_directories(cppbot PUBLIC ${CMAKE_SOURCE_DIR}/src ${Aegis_INCLUDE_DIRS})
target_compile_options(cppbot PUBLIC ${Aegis_CFLAGS_OTHER})