file(GLOB srcs
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/model/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/model/condition/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/model/expression/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/model/relation/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/model/statement/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/model/term/*.cpp"
        )

add_library(common ${srcs})

# this makes the headers accessible for other projects which uses spa lib
target_include_directories(common PUBLIC include)

#target_link_libraries(common spa)
