cmake_minimum_required(VERSION 3.5)

# Project Name
PROJECT(gc_laboratorial)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Tiny-XML 2
file(GLOB XML_SOURCES libs/tinyxml2/*.cpp)
add_library(tinyxml STATIC ${XML_SOURCES}
		libs/tinyxml2/tinyxml2.cpp
		libs/tinyxml2/tinyxml2.h
)
target_include_directories(tinyxml PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/libs/tinyxml2 )

# Common
file(GLOB COMMON_SOURCES common/src/*.cpp )
add_library(common STATIC ${COMMON_SOURCES}
		common/src/model.cpp
		common/include/model.hpp
		common/src/point.cpp
		common/include/point.hpp
		common/src/utils.cpp
		common/include/utils.hpp
		common/include/state.h
		common/src/state.cpp
)
target_include_directories(common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/common/include )

# Generator
file(GLOB GENERATOR_SOURCES generator/src/*.cpp )
add_executable(generator ${GENERATOR_SOURCES} )
target_include_directories(generator PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/generator/include )

# Engine
file(GLOB ENGINE_SOURCES engine/src/*.cpp )
add_executable(engine ${ENGINE_SOURCES} )
target_include_directories(engine PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/engine/include )

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)

if  (WIN32)

	message(STATUS "Toolkits_DIR set to: " ${TOOLKITS_FOLDER})
	set(TOOLKITS_FOLDER "" CACHE PATH "Path to Toolkits folder")
	
	if (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")
		message(ERROR ": GLUT not found")
	endif (NOT EXISTS "${TOOLKITS_FOLDER}/glut/GL/glut.h" OR NOT EXISTS "${TOOLKITS_FOLDER}/glut/glut32.lib")	
	
	
	include_directories(${TOOLKITS_FOLDER}/glut )
	target_link_libraries(generator ${OPENGL_LIBRARIES}
										  ${TOOLKITS_FOLDER}/glut/glut32.lib)
	
	if (EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" )
		file(COPY ${TOOLKITS_FOLDER}/glut/glut32.dll DESTINATION ${CMAKE_BINARY_DIR})
	endif(EXISTS "${TOOLKITS_FOLDER}/glut/glut32.dll" )	
	
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT generator)
	
else (WIN32) #Linux and Mac

	set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} Wno-deprecated")
	find_package(GLUT REQUIRED)
	include_directories(${GLUT_INCLUDE_DIR})
	link_directories(${GLUT_LIBRARY_DIRS})
	add_definitions(${GLUT_DEFINITIONS})

	target_link_libraries(common tinyxml)
	target_link_libraries(generator common tinyxml)
	target_link_libraries(engine common tinyxml ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} )
	if(NOT GLUT_FOUND)
	   message(ERROR ": GLUT not found!")
	endif(NOT GLUT_FOUND)
	
endif(WIN32)








