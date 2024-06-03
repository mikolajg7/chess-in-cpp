cmake_minimum_required(VERSION 3.10)

# Set the project name
project(chess)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find the SFML package
find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)

# Add the executable
add_executable(chess
        ch.cpp
        ChessBoard.h
        main.cpp
        ChessBoard.cpp)

# Link SFML libraries
target_link_libraries(chess sfml-graphics sfml-window sfml-system)

# Copy images to build directory
add_custom_command(TARGET chess POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/images $<TARGET_FILE_DIR:chess>/images)