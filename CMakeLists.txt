cmake_minimum_required(VERSION 3.16)
project(CppForFinance LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Debug)
endif()

if(MSVC)
  add_compile_options(/W4)
else()
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Code du projet (rendu) : tout ce qui est dans src/
file(GLOB PROJECT_SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Un exécutable par fichier de test dans tests/ (chacun a son propre main)
file(GLOB TEST_FILES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/tests/*.cpp)
foreach(test_file ${TEST_FILES})
  get_filename_component(test_name ${test_file} NAME_WE)
  add_executable(${test_name} ${test_file} ${PROJECT_SOURCES})
  target_include_directories(${test_name} PRIVATE ${CMAKE_SOURCE_DIR}/src)
endforeach()
