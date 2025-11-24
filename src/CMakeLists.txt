add_library(game ${CMAKE_SOURCE_DIR}/src/game.cpp ${CMAKE_SOURCE_DIR}/include/game.h)
target_include_directories(game PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(game PUBLIC raylib aabb render)

add_library(render ${CMAKE_SOURCE_DIR}/src/render.cpp ${CMAKE_SOURCE_DIR}/include/render.h)
target_include_directories(render PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(render PUBLIC raylib)

add_library(aabb ${CMAKE_SOURCE_DIR}/src/aabb.cpp ${CMAKE_SOURCE_DIR}/include/aabb.h)
target_include_directories(aabb PUBLIC ${CMAKE_SOURCE_DIR}/include)

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG_MODE)
    target_compile_definitions(game PRIVATE DEBUG_MODE)
    target_compile_definitions(render PRIVATE DEBUG_MODE)
endif()
