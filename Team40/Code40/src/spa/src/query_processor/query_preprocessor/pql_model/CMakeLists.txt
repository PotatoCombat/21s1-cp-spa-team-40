#CMakeLists for query_processor/query_preprocessor/pql_model

file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_subdirectory(relation)
add_subdirectory(clause)

add_library(pql-model ${srcs} ${headers})

target_include_directories(pql-model PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} relation/ clause/)

target_link_libraries(pql-model pql-model-relation pql-model-clause)
