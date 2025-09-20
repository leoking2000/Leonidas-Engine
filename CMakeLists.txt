cmake_minimum_required(VERSION 3.14)
project(ALL_BUILD)
set(CMAKE_CXX_STANDARD 23)

#DELETE THE OUT FOLDER AFTER CHANGING THIS BECAUSE VISUAL STUDIO DOESN'T SEEM TO RECOGNIZE THIS CHANGE AND REBUILD!
option(PRODUCTION_BUILD "Make this a production build" OFF)
#DELETE THE OUT FOLDER AFTER CHANGING THIS BECAUSE VISUAL STUDIO DOESN'T SEEM TO RECOGNIZE THIS CHANGE AND REBUILD!

###### Add Libraries ######

add_subdirectory(thirdparty/raylib)         # core library
add_subdirectory(thirdparty/imgui)          # for easy debuging
add_subdirectory(thirdparty/glm)            # math library


##################################

add_subdirectory(LeoEngine)
add_subdirectory(Projects/SandBox)
add_subdirectory(Projects/Asteroids)