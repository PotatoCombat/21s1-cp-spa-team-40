#CMakeLists for query_processor/query_preprocessor/pql_model/clause

file(GLOB srcs "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
file(GLOB headers "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_library(pql-model-clause ${srcs} ${headers})
