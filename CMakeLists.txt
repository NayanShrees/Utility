cmake_minimum_required(VERSION 4.1.6)

project(Utility VERSION 0.1)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(PROJECT_IS_TOP_LEVEL)
  if(CMAKE_EXPORT_COMPILE_COMMANDS)
    if(CMAKE_HOST_UNIX)
      execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink
        "${CMAKE_CURRENT_BINARY_DIR}/compile_commands.json"
        "${CMAKE_SOURCE_DIR}/build/compile_commands.json"
      )
    endif()
  endif()
endif()

option(DEVELOPMENT_MODE "Enable development mode" OFF)

set(UTILITY_LIB Utility)
if(NOT PROJECT_IS_TOP_LEVEL)
  set(UTILITY_LIB ${UTILITY_LIB} PARENT_SCOPE)
endif()

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/file.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/runtime.cpp
)

add_library(${UTILITY_LIB} SHARED ${SOURCES})
target_include_directories(${UTILITY_LIB} PUBLIC ${INCLUDE_DIR})

if(DEVELOPMENT_MODE)
  target_compile_definitions(${UTILITY_LIB} PRIVATE DEVELOPMENT_MODE)
endif()

install(TARGETS ${UTILITY_LIB}
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/utility
  ARCHIVE DESTINATION lib/utility
)
