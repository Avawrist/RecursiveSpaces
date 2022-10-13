cmake_minimum_required(VERSION 3.0)
set(CMAKE_CXX_STANDARD 11)
project(FirstGame)

# Set Source Directory:
set(CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR} "../")

# Link Libs:
link_directories(${CMAKE_SOURCE_DIR}/libs)

# Set find___.cmake directory:
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Find & add third party libs:
#find_package(OPENGL REQUIRED)
#find_package(GLM REQUIRED)
find_package(GLFW3 REQUIRED)
set(LIBS glfw3 opengl32)

#add_library(STB_IMAGE "${CMAKE_SOURCE_DIR}/src/stb_image.cpp")
#set(LIBS ${LIBS} STB_IMAGE)

#add_library(GLAD "${CMAKE_SOURCE_DIR}/src/glad.c")
#set(LIBS ${LIBS} GLAD)

# Create & Link Executable to Libs
set(SOURCES "${CMAKE_SOURCE_DIR}/src/game.cpp"
            "${CMAKE_SOURCE_DIR}/src/glad.c"
	    "${CMAKE_SOURCE_DIR}/src/shader.cpp"
	    "${CMAKE_SOURCE_DIR}/src/camera.cpp"
	    "${CMAKE_SOURCE_DIR}/src/cursor.cpp"
	    "${CMAKE_SOURCE_DIR}/src/mdcla.cpp"
	    "${CMAKE_SOURCE_DIR}/src/entity.cpp"
	    "${CMAKE_SOURCE_DIR}/src/asset.cpp")	
add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} ${LIBS})

# Include directories 
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/glad)
include_directories(${CMAKE_SOURCE_DIR}/include/GLFW)
include_directories(${CMAKE_SOURCE_DIR}/include/GLFW/glfw)
include_directories(${CMAKE_SOURCE_DIR}/include/glm)
include_directories(${CMAKE_SOURCE_DIR}/include/glm/glm)
include_directories(${CMAKE_SOURCE_DIR}/include/glm/cmake)
include_directories(${CMAKE_SOURCE_DIR}/include/KHR)
