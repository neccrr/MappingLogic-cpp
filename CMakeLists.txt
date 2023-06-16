cmake_minimum_required(VERSION 3.25)
project(MappingLogic)

include(FetchContent)

# Set the C++ standard to C++23
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set the build output directory for the executable
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/builds)

#############################################
##              Dependencies               ##
#############################################

# RapidJSON library
# Download and install RapidJSON
include(FetchContent)
FetchContent_Declare(
        rapidjson
        GIT_REPOSITORY https://github.com/Tencent/rapidjson.git
        GIT_TAG        master
)
FetchContent_GetProperties(rapidjson)
FetchContent_Populate(rapidjson)

#############################################
##              Executable                 ##
#############################################

# Build the executable
file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(MappingLogic ${SOURCES})

# Link the executable with the libraries
target_include_directories(MappingLogic PRIVATE ${rapidjson_SOURCE_DIR}/include)
target_link_libraries(MappingLogic PRIVATE ${rapidjson_LIBRARIES})
