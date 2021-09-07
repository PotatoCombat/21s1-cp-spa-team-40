file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp")

add_library(spa ${srcs})

add_subdirectory(src/common)
add_subdirectory(src/pkb)
add_subdirectory(src/query_processor)

target_link_libraries(spa
        common
        pkb
        query_processor
        )
