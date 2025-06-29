set(CMAKE_CXX_STANDARD 20)

add_executable(tiny-rasterizer source/main.cpp)
find_package(SDL2 REQUIRED)

file(GLOB_RECURSE RASTERIZER_HEADERS "include/*.hpp")
file(GLOB_RECURSE RASTERIZER_SOURCES "source/*.cpp")

add_executable(tiny-rasterizer ${RASTERIZER_HEADERS} ${RASTERIZER_SOURCES})
target_include_directories(tiny-rasterizer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include" "${SDL2_INCLUDE_DIRS}")
target_link_libraries(tiny-rasterizer PUBLIC ${SDL2_LIBRARIES})
