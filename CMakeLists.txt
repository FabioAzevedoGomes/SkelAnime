cmake_minimum_required (VERSION 2.8.12)

project (SkelAnime)

LINK_DIRECTORIES( ${CMAKE_SOURCE_DIR}/lib )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set (CMAKE_DEBUG_POSTFIX "_d")

find_package(OpenGL REQUIRED)

if(WIN32)
set(COMMON_LIBS ${OPENGL_LIBRARIES} optimized glfw debug glfw assimp)
elseif (UNIX)
set(COMMON_LIBS ${OPENGL_LIBRARIES} glfw ${GLFW_LIBRARIES})
else()
set(COMMON_LIBS)
endif()
set(COMMON_LIBS ${COMMON_LIBS} ${EXTRA_LIBS})

set(RUN_DIR ${PROJECT_SOURCE_DIR}/bin)

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SOURCES)

add_executable(SkelAnime
  ${SOURCES}
  lib/gl3w.c
  lib/imgui/imgui_demo.cpp
  lib/imgui/imgui_draw.cpp
  lib/imgui/imgui_tables.cpp
  lib/imgui/imgui_widgets.cpp
  lib/imgui/imgui.cpp
  lib/imgui/imgui_impl_glfw.cpp
  lib/imgui/imgui_impl_opengl3.cpp
  lib/FileBrowser/ImGuiFileBrowser.cpp
  src/shader_fragment.glsl
  src/shader_vertex.glsl
   "src/Animation.cpp")
set_property(TARGET SkelAnime PROPERTY DEBUG_POSTFIX _d)
target_link_libraries(SkelAnime ${COMMON_LIBS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DIMGUI_IMPL_OPENGL_LOADER_GL3W -DGLFW_EXPOSE_NATIVE_WGL")

# Set include directories
MACRO(HEADER_DIRECTORIES return_list)
    FILE(GLOB_RECURSE new_list *.hpp)
    SET(dir_list "")
    FOREACH(file_path ${new_list})
        GET_FILENAME_COMPONENT(dir_path ${file_path} PATH)
        SET(dir_list ${dir_list} ${dir_path})
    ENDFOREACH()
    LIST(REMOVE_DUPLICATES dir_list)
    SET(${return_list} ${dir_list})
ENDMACRO()

HEADER_DIRECTORIES( header_dir_list )
include_directories( ${header_dir_list} )

include_directories(lib)
include_directories(lib/imgui/)
include_directories(lib/FileBrowser)
include_directories(lib/FileBrower/Dirent)
include_directories(lib/glfw/include)
include_directories(lib/assimp-4.1.0/include)

target_link_libraries(SkelAnime imm32.lib)

option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
option(GLFW_BUILD_DOCS OFF)
set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)
add_subdirectory(lib/glfw)
add_subdirectory(lib/assimp-4.1.0)
