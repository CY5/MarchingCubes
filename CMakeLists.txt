cmake_minimum_required(VERSION 3.16)

function(set_output_directories DIR)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DIR} PARENT_SCOPE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${DIR} PARENT_SCOPE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${DIR} PARENT_SCOPE)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${DIR} PARENT_SCOPE)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${DIR} PARENT_SCOPE)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${DIR} PARENT_SCOPE)
endfunction(set_output_directories)

project(mc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

enable_testing() # needed on top-level CMakeLists.txt
include (CTest)

set(IMGUI_DIR "src/Vendor/imgui")
file(GLOB IMGUI_SRC "${IMGUI_DIR}/*.cpp")
add_library(imgui STATIC
            ${IMGUI_SRC})
target_include_directories(imgui PUBLIC ${IMGUI_DIR})
target_compile_features(imgui PUBLIC cxx_std_11)


set(EXECUTABLE_DIR ${CMAKE_BINARY_DIR}/MarchingCubes)

file( COPY ${CMAKE_CURRENT_SOURCE_DIR}/res DESTINATION ${EXECUTABLE_DIR} )


#GLM
set(GLM_LIB_NAME src/Vendor/glm)
set(GLM_TEST_ENABLE OFF CACHE BOOL "" FORCE)
add_subdirectory(${GLM_LIB_NAME})


add_test(example_test mc)

if (CMAKE_COMPILER_IS_GNUCC)
    add_custom_target(test_memcheck
    COMMAND ${CMAKE_CTEST_COMMAND} 
        --force-new-ctest-process --test-action memcheck
    COMMAND cat "${CMAKE_BINARY_DIR}/Testing/Temporary/MemoryChecker.*.log"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")
endif()

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
    message("Glew_Found :" ${GLEW_FOUND})
    message("GLEW_INCLUDE_DIRS :" ${GLEW_INCLUDE_DIRS})
    message("GLEW_LIBRARIES :" ${GLEW_LIBRARIES})
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

find_package(glfw3 3.3 REQUIRED)

file (GLOB headerDir
        "src/*.h" "src/helper/*.h" "src/Vendor/*.h" 
        "src/Vendor/glm/*.h" "src/Vendor/imgui/*.h"
        "src/Vendor/stb_image/*.h" "res/data/*h"
)
file (GLOB res  "${CMAKE_CURRENT_SOURCE_DIR}/res/")


include_directories(
        ${OPENGL_INCLUDE_DIRS}
        headerDir
)

file(GLOB srcDir
        "src/*.cpp" "src/helper/*.cpp" "src/Vendor/*.cpp"
        "src/Vendor/glm/*.cpp" "src/Vendor/imgui/*.cpp"
        "src/Vendor/stb_image/*.cpp"
        )

add_library(objects OBJECT ${headerDir} ${srcDir}  main.cpp)


set_output_directories(${EXECUTABLE_DIR})
add_executable(mc $<TARGET_OBJECTS:objects>)

# add_executable(mc
#         ${srcDir}
#         main.cpp
#         src/Renderer.cpp
#         src/Renderer.h
#         src/VertexBuffer.cpp
#         src/VertexBuffer.h
#         src/IndexBuffer.cpp
#         src/IndexBuffer.h
#         src/VertexArray.cpp
#         src/VertexArray.h
#         src/VertexBufferLayout.h
#         src/VertexBufferLayout.cpp
#         src/Shader.cpp
#         src/Shader.h
#         src/Texture.cpp
#         src/Texture.h
#         src/Vendor/stb_image/stb_image.cpp
#         src/Vendor/stb_image/stb_image.h
#         )

target_link_libraries(mc
        ${OPENGL_LIBRARIES}
        ${GLEW_LIBRARIES}
        glfw
        imgui
        glm)
