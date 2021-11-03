file(GLOB srcs
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/model/*.cpp"
        )

add_library(common ${srcs})

# this makes the headers accessible for other projects which uses spa lib
target_include_directories(common PUBLIC include)
