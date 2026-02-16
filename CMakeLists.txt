cmake_minimum_required(VERSION 3.14)
project(ALL_BUILD)
set(CMAKE_CXX_STANDARD 23)

#DELETE THE OUT FOLDER AFTER CHANGING THIS BECAUSE VISUAL STUDIO DOESN'T SEEM TO RECOGNIZE THIS CHANGE AND REBUILD!
option(PRODUCTION_BUILD "Make this a production build" OFF)
#DELETE THE OUT FOLDER AFTER CHANGING THIS BECAUSE VISUAL STUDIO DOESN'T SEEM TO RECOGNIZE THIS CHANGE AND REBUILD!

###### Add Libraries ######

add_subdirectory(thirdparty/glad)           # OpenGL loader
add_subdirectory(thirdparty/glfw-3.3.2)     # window management
add_subdirectory(thirdparty/glm)            # math library
add_subdirectory(thirdparty/stb)            # texture read-write


##################################

add_subdirectory(LeoEngine)
add_subdirectory(Projects/TestProject)