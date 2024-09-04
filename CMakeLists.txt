cmake_minimum_required(VERSION 3.5)

# Project name and language
project(LynxLog LANGUAGES C)

# Set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Add source files
add_executable(LynxLog
    src/main.c
    src/linxlog.c
    src/utils.c
    include/linxlog.h
    .gitignore
    # Add other source files as needed
)

# Include directories (if you have headers in a specific directory)
target_include_directories(LynxLog PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Optionally include directories for third-party libraries or dependencies
# target_include_directories(LynxLog PRIVATE /path/to/dependency/include)

# Optionally link to third-party libraries or dependencies
# target_link_libraries(LynxLog PRIVATE /path/to/dependency/lib)

# Enable testing if you plan to add tests
enable_testing()

# Add test directory if you have unit tests
# add_subdirectory(tests)

# Install targets
include(GNUInstallDirs)
install(TARGETS LynxLog
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    # LIBRARY and ARCHIVE DESTINATION are used for shared/static libraries
)

# Optionally, you can set the installation paths to default or custom directories
# set(CMAKE_INSTALL_PREFIX "/your/install/path")
