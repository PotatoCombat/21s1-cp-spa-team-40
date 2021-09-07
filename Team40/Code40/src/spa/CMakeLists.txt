file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(spa ${srcs})

add_subdirectory(src/common)
add_subdirectory(src/pkb)
add_subdirectory(src/source_processor)

target_include_directories(spa PUBLIC include)

target_link_libraries(spa
        common
        pkb
        source_processor
        )
