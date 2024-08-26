cmake_minimum_required(VERSION 3.8 FATAL_ERROR)
project(spreadsheet)

set(CMAKE_CXX_STANDARD 17)
if(CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
  set(
    CMAKE_CXX_FLAGS_DEBUG
    "${CMAKE_CXX_FLAGS_DEBUG} /JMC"
  )
else()
  set(
    CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Werror -Wno-unused-parameter -Wno-implicit-fallthrough"
  )
endif()

set(STARTER_FILES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../starter_files)

set(ANTLR_EXECUTABLE ${STARTER_FILES_DIR}/antlr-4.7.2-complete.jar)
include(${STARTER_FILES_DIR}/FindANTLR.cmake)
 
add_definitions(
  -DANTLR4CPP_STATIC
  -D_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
)

set(WITH_STATIC_CRT OFF CACHE BOOL "Visual C++ static CRT for ANTLR" FORCE)
add_subdirectory(${STARTER_FILES_DIR}/antlr4_runtime antlr4_runtime)

antlr_target(FormulaParser ${STARTER_FILES_DIR}/Formula.g4 LEXER PARSER LISTENER)

include_directories(
  ${ANTLR4_INCLUDE_DIRS}
  ${ANTLR_FormulaParser_OUTPUT_DIR}
  ${STARTER_FILES_DIR}/antlr4_runtime/runtime/src
  ${STARTER_FILES_DIR}
  ${CMAKE_CURRENT_SOURCE_DIR}
)

file(GLOB starter_sources
  ${STARTER_FILES_DIR}/common.h
  ${STARTER_FILES_DIR}/formula.h
  ${STARTER_FILES_DIR}/test_runner.h
  ${STARTER_FILES_DIR}/main.cpp
)
file(GLOB sources
  *.cpp
  *.h
)

add_executable(
  spreadsheet
  ${ANTLR_FormulaParser_CXX_OUTPUTS}
  ${starter_sources}
  ${sources}
)

target_link_libraries(spreadsheet antlr4_static)

install(
  TARGETS spreadsheet
  DESTINATION bin
  EXPORT spreadsheet
)

set_directory_properties(PROPERTIES VS_STARTUP_PROJECT spreadsheet)
