cmake_minimum_required(VERSION 3.20)
project(WinBrush VERSION 1.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Collect source files
file(GLOB SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB HEADERS CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h")

# Create executable
add_executable(${PROJECT_NAME} WIN32 ${SOURCES} ${HEADERS})

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Compiler flags for MinGW
if(MINGW)
    target_compile_options(${PROJECT_NAME} PRIVATE -municode)
    target_link_options(${PROJECT_NAME} PRIVATE -mwindows -municode)
endif()

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    gdi32 comctl32 gdiplus ole32 oleaut32 uuid
)

# Set output properties
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}"
    OUTPUT_NAME "WinBrush"
)