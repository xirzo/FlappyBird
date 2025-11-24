add_executable(${PROJECT_NAME} main.cpp)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG_MODE)
    target_compile_definitions(${PROJECT_NAME} PRIVATE DEBUG_MODE)
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE game)
