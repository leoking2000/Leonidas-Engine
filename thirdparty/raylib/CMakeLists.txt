cmake_minimum_required(VERSION 3.14)

# MultiThreaded compilation
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDebug$<$<CONFIG:Debug>:Debug>")
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Release>:Release>")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE) #link time optimization

# Create an imported library target for raylib
add_library(raylib STATIC IMPORTED GLOBAL)

# Set the include directory
set_target_properties(raylib PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Point to the .lib file
set_target_properties(raylib PROPERTIES
    IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/raylib.lib
)

# Link extra system libraries on Windows
if (WIN32)
    target_link_libraries(raylib INTERFACE
        winmm.lib
        gdi32.lib
        opengl32.lib
    )
endif()
