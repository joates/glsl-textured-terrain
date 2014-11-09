CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(APP_NAME demo)

PROJECT(${APP_NAME})

SET(SOURCE_FILES 
    src/main.cpp
    src/LTexturedPolygonProgram2D.cpp
    src/LShaderProgram.cpp
    src/LTexture.cpp
    src/LUtil.cpp
)

LINK_DIRECTORIES( ${PROJECT_SOURCE_DIR}/lib )
#INCLUDE_DIRECTORIES( ${PROJECT_SOURCE_DIR}/include )

ADD_EXECUTABLE(../${APP_NAME} ${SOURCE_FILES})

#Find DevIL
find_package(DevIL REQUIRED)
if(IL_FOUND)
    include_directories(${PROJECT_SOURCE_DIR}/include ${IL_INCLUDE_DIR})
    SET(LIBS GL GLU IL ILU glut GLEW)
    target_link_libraries(../${APP_NAME} ${LIBS} ${IL_LIBRARY} ${ILU_LIBRARY} ${ILUT_LIBRARY})
endif(IL_FOUND)
