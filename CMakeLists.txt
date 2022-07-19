cmake_minimum_required(VERSION 3.20)






set(CMAKE_CXX_STANDARD          20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

set(project_name     "BitGL")
set(project_source_files
    "Window.cpp"
    "DrawWindow.cpp"
    "main.cpp")



project(${project_name})

list(TRANSFORM project_source_files PREPEND "src/")



add_executable(${project_name} ${project_source_files})

target_include_directories(${project_name} PRIVATE "include/")

if(MSVC)
	target_compile_options(${project_name} PRIVATE "/W4" "/w14640" "/permissive-" "/WX")
else()
	target_compile_options(${project_name} PRIVATE "-Wall" "-Wextra" "-Wshadow" "-Wnon-virtual-dtor" "-Wpedantic" "-Wno-missing-field-initializers" "-Werror")
endif()
