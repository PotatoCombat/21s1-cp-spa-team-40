#CMakeLists for query_processor/query_preprocessor/pql_model/relation

file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_library(pql-model-relation ${srcs} ${headers})
